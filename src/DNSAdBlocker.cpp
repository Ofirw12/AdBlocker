
#include "DNSAdBlocker.hpp"

#include <iostream>

using namespace boost::asio::ip;

adblocker::DNSAdBlocker::DNSAdBlocker(boost::asio::io_context& io,
    const std::string& listen_ip, unsigned short listen_port,
    const std::string& upstream_ip, unsigned short upstream_port,
    const std::string& blocklist_path)
    : m_listen_socket(io, udp::endpoint(make_address(listen_ip), listen_port)),
    m_upstream_socket(io, udp::v4()),
      m_upstream_endpoint(make_address(upstream_ip), upstream_port),
      m_buffer(), m_blocklist(blocklist_path)
{}

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
        std::cout << "⛔ Blocked: " << domain << std::endl;
        // TODO: build proper NXDOMAIN
        // std::vector<uint8_t> fake_response = {/* ... */};
        std::vector<uint8_t> fake_response = {0x00, 0x01, 0x00, 0x00};
        SendResponse(fake_response, client);
    }
}

void adblocker::DNSAdBlocker::ForwardQuery(const uint8_t* data, size_t len,
                                                const udp::endpoint& client)
{
    m_upstream_socket.async_send_to(boost::asio::buffer(data, len), m_upstream_endpoint,
        [this, client](const boost::system::error_code& ec, std::size_t bytes)
        {
            if (!ec)
            {
                m_upstream_socket.async_receive_from(boost::asio::buffer(m_buffer), m_upstream_endpoint,
                    [this, client](const boost::system::error_code& ec, std::size_t bytes)
                    {
                        if (!ec)
                        {
                            std::vector<uint8_t> response(m_buffer.begin(),
                              m_buffer.begin() + bytes);
                            SendResponse(response, client);
                        }
                    });
            }
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