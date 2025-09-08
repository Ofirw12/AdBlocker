
#include <iostream>

#include "DNSAdBlocker.hpp"
#include "Logger.hpp"

adblocker::DNSAdBlocker::DNSAdBlocker(const std::string& upstream_ip, unsigned short upstream_port,
                                      const std::string& blocklist_path)
    : m_upstream_ip(upstream_ip), m_upstream_port(upstream_port),
      m_buffer(BUFFER_SIZE), m_blocklist(blocklist_path), m_isRunning(false)
{}

adblocker::DNSAdBlocker::~DNSAdBlocker()
{
    m_isRunning = false;
}

void adblocker::DNSAdBlocker::Run(const std::string& listen_ip, uint16_t listen_port)
{
    if (!m_listen_socket.Bind(listen_ip, listen_port))
    {
        // TODO add log
        throw std::runtime_error("Failed to bind listen socket");
    }

    if (!m_upstream_socket.Bind("0.0.0.0", 0))
    {
        // TODO add log
        throw std::runtime_error("Failed to bind upstream socket");
    }
    m_blocklist.Load();
    m_thread = std::jthread(&DNSAdBlocker::ThreadFunc, this);
}

void adblocker::DNSAdBlocker::ThreadFunc()
{
    m_isRunning = true;
    Endpoint client;
    //TODO toggle off somewhere
    while (m_isRunning)
    {
        ssize_t bytes = m_listen_socket.ReceiveFrom(m_buffer, client.endpoint_ip, client.endpoint_port);
        if (bytes > 0)
        {
            HandleReceive(static_cast<size_t>(bytes), client);
        }
    }
}

std::vector<uint8_t> adblocker::DNSAdBlocker::BuildNXDomainResponse(const uint8_t* query, size_t len)
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
    const Endpoint& client)
{
    std::string domain = DNSParser::ParseQuery(m_buffer.data(), bytes_transferred);

    if (m_blocklist.IsValid(domain))
    {
        ForwardQuery(m_buffer.data(), bytes_transferred, client);
    }
    else
    {
        std::cout << "Blocked: " << domain << std::endl;
        const auto fake_response = BuildNXDomainResponse(m_buffer.data(), bytes_transferred);
        SendResponse(fake_response, client);
    }
}

void adblocker::DNSAdBlocker::ForwardQuery(const uint8_t* data, size_t len,
    const Endpoint& client)
{
    std::vector<uint8_t> upstream_buf(1500);
    ssize_t n = m_upstream_socket.SendTo(std::vector<uint8_t>(data, data+len), m_upstream_ip, m_upstream_port);
    if (n < 0)
    {
        //TODO add log
        std::cerr << "Failed to send to upstream DNS" << std::endl;
        return;
    }

    Endpoint upstream;
    n = m_upstream_socket.ReceiveFrom(upstream_buf,
                            upstream.endpoint_ip, upstream.endpoint_port);
    if (n > 0)
    {
        std::vector<uint8_t> response(upstream_buf.begin(), upstream_buf.begin() + n);
        SendResponse(response, client);
    }
    else
    {
        std::cerr << "No response from upstream DNS" << std::endl;
    }
}

void adblocker::DNSAdBlocker::SendResponse(const std::vector<uint8_t>& response,
    const Endpoint& client)
{
    m_listen_socket.SendTo(response, client.endpoint_ip, client.endpoint_port);
}