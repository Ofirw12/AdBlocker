//
// Created by ofir on 26/08/2025.
//

#ifndef ADBLOCKER_DNSADBLOCKER_HPP
#define ADBLOCKER_DNSADBLOCKER_HPP

#include <thread>
#include <utility>
#include <vector>

#include "DNSBlockList.hpp"
#include "DNSParser.hpp"
#include "UDPSocket.hpp"

namespace adblocker
{

struct Endpoint
{
    std::string endpoint_ip;
    uint16_t endpoint_port;
};

class DNSAdBlocker
{
public:
    DNSAdBlocker(const std::string& upstream_ip, unsigned short upstream_port,
                                            const std::string& blocklist_path);
    ~DNSAdBlocker();

    void Run(const std::string& listen_ip, uint16_t listen_port);

    DNSAdBlocker(const DNSAdBlocker& other) = delete;
    DNSAdBlocker& operator=(const DNSAdBlocker& other) = delete;
    DNSAdBlocker(DNSAdBlocker&& other) = delete;
    DNSAdBlocker& operator=(DNSAdBlocker&& other) = delete;
private:
    void HandleReceive(std::size_t bytes_transferred,
                            const Endpoint& client);
    void ForwardQuery(const uint8_t* data, size_t len,
                            const Endpoint& client);
    void SendResponse(const std::vector<uint8_t>& response,
                            const Endpoint& client);
    void ThreadFunc();
    static std::vector<uint8_t> BuildNXDomainResponse(const uint8_t* query,
                                                                    size_t len);

    UDPSocket m_listen_socket; //listen to UDP 53
    UDPSocket m_upstream_socket; // forward to 8.8.8.8 UDP 53
    std::string m_upstream_ip;
    uint16_t m_upstream_port;
    const size_t BUFFER_SIZE = 1500;
    std::vector<uint8_t> m_buffer;
    DNSBlockList m_blocklist;
    std::jthread m_thread;
    bool m_isRunning;
}; // class DNSAdBlocker
} // namespace adblocker

#endif //ADBLOCKER_DNSADBLOCKER_HPP