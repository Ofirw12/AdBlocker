//
// Created by ofir on 08/09/2025.
//

#ifndef ILRD_RD1645_ADBLOCKER_UDPSOCKET_HPP
#define ILRD_RD1645_ADBLOCKER_UDPSOCKET_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace adblocker
{
class UDPSocket
{
public:
    UDPSocket();
    ~UDPSocket();

    UDPSocket(const UDPSocket& other) = delete;
    UDPSocket& operator=(const UDPSocket& other) = delete;
    UDPSocket(UDPSocket&& other) = delete;
    UDPSocket& operator=(UDPSocket&& other) = delete;

    bool Bind(const std::string& ip, uint16_t port);
    ssize_t SendTo(const std::vector<uint8_t>& data,
                                const std::string& dest_ip, uint16_t dest_port);
    ssize_t ReceiveFrom(std::vector<uint8_t>& buffer,
                                std::string& sender_ip, uint16_t& sender_port);
    bool SetNonBlocking(bool nonBlocking);
    [[nodiscard]] int GetFD() const;
    [[nodiscard]] bool IsValid() const;
private:
    int m_fd;
}; // class UDPSocket
} // namespace adblocker

#endif //ILRD_RD1645_ADBLOCKER_UDPSOCKET_HPP