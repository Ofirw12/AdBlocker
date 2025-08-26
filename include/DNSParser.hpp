//
// Created by ofir on 26/08/2025.
//

#ifndef ADBLOCKER_DNSPARSER_HPP
#define ADBLOCKER_DNSPARSER_HPP
#include <string>
#include <vector>

namespace adblocker
{

struct DnsQuestion {
    std::string qname;
    uint16_t qtype;
    uint16_t qclass;
};

class DNSParser
{
public:
    explicit DNSParser() = default;
    ~DNSParser() = default;

    static bool ParseQuery(const uint8_t* data, size_t len, DnsQuestion& out);
    static std::vector<uint8_t> BuildBlockResponse(const uint8_t* query,
                                            size_t qlen, const DnsQuestion& q);

    DNSParser(const DNSParser& other) = delete;
    DNSParser& operator=(const DNSParser& other) = delete;
    DNSParser(DNSParser&& other) = delete;
    DNSParser& operator=(DNSParser&& other) = delete;
private:
}; // class DNSParser
} // namespace adblocker

#endif //ADBLOCKER_DNSPARSER_HPP