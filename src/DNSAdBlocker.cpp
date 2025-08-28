
#include "DNSAdBlocker.hpp"

#include <iostream>

using namespace boost::asio::ip;

adblocker::DNSAdBlocker::DNSAdBlocker(boost::asio::io_context& io,
    const std::string& listen_ip, unsigned short listen_port,
    const std::string& upstream_ip, unsigned short upstream_port,
    const std::string& blocklist_path)
    : m_listen_socket(io, udp::v4()),
m_upstream_socket(io, udp::v4()),
      m_upstream_endpoint(make_address(upstream_ip), upstream_port),
m_client(make_address(listen_ip), listen_port),
      m_buffer(),
      m_blocklist(blocklist_path)
{
    m_listen_socket.open(udp::v4());
}

void adblocker::DNSAdBlocker::Run()
{
    m_blocklist.Load();
    StartReceive();
}

void adblocker::DNSAdBlocker::StartReceive()
{
    m_listen_socket.async_receive_from(boost::asio::buffer(m_buffer), m_client,
        [this](const boost::system::error_code& ec, std::size_t bytes)
        {
            if (!ec && bytes > 0)
            {
                HandleReceive(bytes, m_client);
            }
            else
            {
                std::cerr << "Receive error: " << ec.message() << std::endl;
                StartReceive();
            }
        });
}

void adblocker::DNSAdBlocker::HandleReceive(std::size_t bytes_transferred,
    const udp::endpoint& remote)
{
    // TODO: parse DNS question properly
    std::string domain = "example.com"; // placeholder

    if (m_blocklist.IsValid(domain))
    {
        ForwardQuery(m_buffer.data(), bytes_transferred);
    }
    else
    {
        std::cout << "⛔ Blocked: " << domain << std::endl;
        // TODO: build proper NXDOMAIN
        std::vector<uint8_t> fake_response = {/* ... */};
        SendResponse(fake_response);
    }
}

void adblocker::DNSAdBlocker::ForwardQuery(const uint8_t* data, size_t len)
{
    m_upstream_socket.async_send_to(boost::asio::buffer(data, len), m_upstream_endpoint,
        [this](const boost::system::error_code& ec, std::size_t bytes)
        {
            if (!ec)
            {
                m_upstream_socket.async_receive_from(boost::asio::buffer(m_buffer), m_upstream_endpoint,
                    [this](const boost::system::error_code& ec, std::size_t bytes)
                    {
                        if (!ec)
                        {
                            std::vector<uint8_t> response(m_buffer.begin(),
                              m_buffer.begin() + bytes);
                            SendResponse(response);
                        }
                    });
            }
        });
}

void adblocker::DNSAdBlocker::SendResponse(const std::vector<uint8_t>& response)
{
    m_listen_socket.async_send_to(boost::asio::buffer(response), m_client,
        [this](const boost::system::error_code& ec, std::size_t bytes)
        {
            StartReceive();
        });
}