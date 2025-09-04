//
// Created by ofir on 26/08/2025.
//

#ifndef ADBLOCKER_DNSPARSER_HPP
#define ADBLOCKER_DNSPARSER_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace adblocker
{

class DNSParser
{
public:
    /// @brief Parse the DNS query and search domain
    /// @param data DNS query
    /// @param len number of bytes transferred
    /// @return domain name
    static std::string ParseQuery(const uint8_t* data, size_t len);

    /// @brief 
    /// @param query 
    /// @param qlen 
    /// @param q 
    /// @return 
    static std::vector<uint8_t> BuildBlockResponse(const uint8_t* query,
                                            size_t qlen, const std::string& q);

    DNSParser() = delete;
    DNSParser(const DNSParser& other) = delete;
    DNSParser& operator=(const DNSParser& other) = delete;
    DNSParser(DNSParser&& other) = delete;
    DNSParser& operator=(DNSParser&& other) = delete;
}; // class DNSParser
} // namespace adblocker

#endif //ADBLOCKER_DNSPARSER_HPP