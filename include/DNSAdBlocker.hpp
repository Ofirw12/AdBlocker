//
// Created by ofir on 26/08/2025.
//

#ifndef ADBLOCKER_DNSADBLOCKER_HPP
#define ADBLOCKER_DNSADBLOCKER_HPP

#include <array>
#include <utility>
#include <vector>
#include <boost/asio.hpp>

#include "DNSBlockList.hpp"

namespace adblocker
{
class DNSAdBlocker
{
public:
    DNSAdBlocker(boost::asio::io_context& io, const std::string& listen_ip,
                    unsigned short listen_port,const std::string& upstream_ip,
                    unsigned short upstream_port,const std::string& blocklist_path);
    ~DNSAdBlocker() = default;

    void Run();

    DNSAdBlocker(const DNSAdBlocker& other) = delete;
    DNSAdBlocker& operator=(const DNSAdBlocker& other) = delete;
    DNSAdBlocker(DNSAdBlocker&& other) = delete;
    DNSAdBlocker& operator=(DNSAdBlocker&& other) = delete;
private:
    void StartReceive();
    void HandleReceive(std::size_t bytes_transferred,
                        const boost::asio::ip::udp::endpoint& client);
    void ForwardQuery(const uint8_t* data, size_t len, const boost::asio::ip::udp::endpoint& client);
    void SendResponse(const std::vector<uint8_t>& response, const boost::asio::ip::udp::endpoint& client);

    boost::asio::ip::udp::socket m_listen_socket; //listen to UDP 53
    boost::asio::ip::udp::socket m_upstream_socket; // forward to 8.8.8.8 UDP 53
    boost::asio::ip::udp::endpoint m_upstream_endpoint; // ?
    // boost::asio::ip::udp::endpoint m_client; // user
    std::array<uint8_t, 1500> m_buffer;
    DNSBlockList m_blocklist;
}; // class DNSAdBlocker
} // namespace adblocker

#endif //ADBLOCKER_DNSADBLOCKER_HPP