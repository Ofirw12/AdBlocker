//
// Created by ofir on 26/08/2025.
//
#include "DNSAdBlocker.hpp"

adblocker::DNSAdBlocker::DNSAdBlocker(boost::asio::io_context& io,
    const std::string& listen_ip, unsigned short listen_port,
    const std::string& upstream_ip, unsigned short upstream_port,
    const std::string& blocklist_path)
{
}

void adblocker::DNSAdBlocker::Run()
{
}

void adblocker::DNSAdBlocker::StartReceive()
{
}

void adblocker::DNSAdBlocker::HandleReceive(std::size_t bytes_transferred,
    const boost::asio::ip::udp::endpoint& remote)
{
}

void adblocker::DNSAdBlocker::ForwardQuery(const uint8_t* data, size_t len,
    const boost::asio::ip::udp::endpoint& client)
{
}

void adblocker::DNSAdBlocker::SendResponse(const std::vector<uint8_t>& response,
    const boost::asio::ip::udp::endpoint& client)
{
}
