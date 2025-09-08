#include <cstring>
#include <fcntl.h>
#include <memory>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "UDPSocket.hpp"
#include "Logger.hpp"

adblocker::UDPSocket::UDPSocket() : m_fd(-1)
{}

adblocker::UDPSocket::~UDPSocket()
{
    if (m_fd >= 0)
    {
        close(m_fd);
    }
}

bool adblocker::UDPSocket::Bind(const std::string& ip, uint16_t port)
{
    if (m_fd < 0)
    {
        m_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (m_fd < 0)
        {
            //TODO add log
            // perror("socket");
            return false;
        }
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
    {
        //TODO add logger log
        // perror("inet_pton");
        return false;
    }

    int opt = 1;
    if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        //TODO add logger log
        // perror("setsockopt");
        return false;
    }

    if (bind(m_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        //TODO add logger log
        // perror("bind");
        return false;
    }

    return true;
}

ssize_t adblocker::UDPSocket::SendTo(const std::vector<uint8_t>& data,
    const std::string& dest_ip, uint16_t dest_port)
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(dest_port);
    if (inet_pton(AF_INET, dest_ip.c_str(), &addr.sin_addr) <= 0)
    {
        //TODO add logger log
        // perror("inet_pton");
        return -1;
    }

    return sendto(m_fd, data.data(), data.size(), 0,
                    reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
}

ssize_t adblocker::UDPSocket::ReceiveFrom(std::vector<uint8_t>& buffer,
    std::string& sender_ip, uint16_t& sender_port)
{
    sockaddr_in addr{};
    socklen_t addr_len = sizeof(addr);
    const ssize_t n = recvfrom(m_fd, buffer.data(), buffer.size(),
                        0, reinterpret_cast<sockaddr*>(&addr), &addr_len);
    if (n >= 0)
    {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
        sender_ip = ip;
        sender_port = ntohs(addr.sin_port);
    }
    return n;
}

bool adblocker::UDPSocket::SetNonBlocking(const bool nonBlocking)
{
    int flags = fcntl(m_fd, F_GETFL, 0);
    if (flags < 0)
    {
        return false;
    }
    if (nonBlocking)
    {
        flags |= O_NONBLOCK;
    }
    else
    {
        flags &= ~O_NONBLOCK;
    }
    return fcntl(m_fd, F_SETFL, flags) >= 0;
}

int adblocker::UDPSocket::GetFD() const
{
    return m_fd;
}

bool adblocker::UDPSocket::IsValid() const
{
    return m_fd >= 0;
}
