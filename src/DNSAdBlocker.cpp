
#include "DNSAdBlocker.hpp"

#include <iostream>

using namespace boost::asio::ip;

adblocker::DNSAdBlocker::DNSAdBlocker(boost::asio::io_context& io,
    const std::string& listen_ip, unsigned short listen_port,
    const std::string& upstream_ip, unsigned short upstream_port,
    const std::string& blocklist_path)
    : m_listen_socket(io, udp::endpoint(make_address(listen_ip), listen_port)),
    m_upstream_socket(io),
      m_upstream_endpoint(make_address(upstream_ip), upstream_port),
      m_buffer(), m_blocklist(blocklist_path)
{
    m_upstream_socket.open(udp::v4());
    m_upstream_socket.bind(udp::endpoint(address_v4::any(), 0));
}

void adblocker::DNSAdBlocker::Run()
{
    m_blocklist.Load();
    StartReceive();
}

void adblocker::DNSAdBlocker::StartReceive()
{
    auto client = std::make_shared<udp::endpoint>();

    m_listen_socket.async_receive_from(boost::asio::buffer(m_buffer), *client,
        [this, client](const boost::system::error_code& ec, std::size_t bytes)
        {
            if (!ec && bytes > 0)
            {
                HandleReceive(bytes, *client);
            }
            else
            {
                std::cerr << "Receive error: " << ec.message() << std::endl;
                StartReceive();
            }
        });
}

static std::vector<uint8_t> BuildNXDomainResponse(const uint8_t* query, size_t len)
{
    if (len < 12) return {}; // invalid DNS query

    std::vector<uint8_t> resp(len);

    // Copy the ID
    resp[0] = query[0];
    resp[1] = query[1];

    // Set flags: QR=1 (response), OPCODE=0, AA=0, TC=0, RD=copy, RA=0, RCODE=3 (NXDOMAIN)
    resp[2] = 0x81; // QR=1, RD=1 (copy from query)
    resp[3] = 0x83; // RA=0, RCODE=3

    // Copy QDCOUNT from request
    resp[4] = query[4];
    resp[5] = query[5];

    // ANCOUNT = 0
    resp[6] = 0;
    resp[7] = 0;

    // NSCOUNT = 0
    resp[8] = 0;
    resp[9] = 0;

    // ARCOUNT = 0
    resp[10] = 0;
    resp[11] = 0;

    // Copy the question section as-is
    if (len > 12)
        std::copy(query + 12, query + len, resp.begin() + 12);

    return resp;
}


void adblocker::DNSAdBlocker::HandleReceive(std::size_t bytes_transferred,
    const udp::endpoint& client)
{
    std::string domain = adblocker::DNSParser::ParseQuery(m_buffer.data(), bytes_transferred);

    if (m_blocklist.IsValid(domain))
    {
        ForwardQuery(m_buffer.data(), bytes_transferred, client);
    }
    else
    {
        std::cout << "Blocked: " << domain << std::endl;
        // TODO: build proper NXDOMAIN
        auto fake_response = BuildNXDomainResponse(m_buffer.data(), bytes_transferred);
        SendResponse(fake_response, client);
    }
}

void adblocker::DNSAdBlocker::ForwardQuery(const uint8_t* data, size_t len,
                                                const udp::endpoint& client)
{
    auto reply_buf = std::make_shared<std::array<uint8_t, 1500>>();
    auto upstream_ep = std::make_shared<udp::endpoint>();

    m_upstream_socket.async_receive_from( boost::asio::buffer(*reply_buf), *upstream_ep,
            [this, client, reply_buf, upstream_ep](const boost::system::error_code& ec, std::size_t bytes)
            {
                if (!ec)
                {
                    const std::vector<uint8_t> response(reply_buf->begin(),
                                                  reply_buf->begin() + bytes);
                    SendResponse(response, client);
                }
                else
                {
                    std::cerr << "Upstream receive error: " << ec.message() << std::endl;
                }
            });

    m_upstream_socket.async_send_to(
    boost::asio::buffer(data, len), m_upstream_endpoint,
    [](const boost::system::error_code& ec, std::size_t bytes)
    {
        if (ec)
            std::cerr << "Upstream send error: " << ec.message() << std::endl;
    });
}

void adblocker::DNSAdBlocker::SendResponse(const std::vector<uint8_t>& response,
                                                    const udp::endpoint& client)
{
    m_listen_socket.async_send_to(boost::asio::buffer(response), client,
        [this](const boost::system::error_code& ec, std::size_t bytes)
        {
            std::cout << "responded" << std::endl;
            StartReceive();
        });
}