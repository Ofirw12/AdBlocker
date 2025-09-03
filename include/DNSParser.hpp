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

struct DNSQuestion
{
    std::string qname;
    uint16_t qtype;
    uint16_t qclass;
};

class DNSParser
{
public:
    explicit DNSParser() = default;
    ~DNSParser() = default;

    static bool ParseQuery(const uint8_t* data, size_t len, DNSQuestion& out);
    static std::vector<uint8_t> BuildBlockResponse(const uint8_t* query,
                                            size_t qlen, const DNSQuestion& q);

    DNSParser(const DNSParser& other) = delete;
    DNSParser& operator=(const DNSParser& other) = delete;
    DNSParser(DNSParser&& other) = delete;
    DNSParser& operator=(DNSParser&& other) = delete;
}; // class DNSParser
} // namespace adblocker

#endif //ADBLOCKER_DNSPARSER_HPP