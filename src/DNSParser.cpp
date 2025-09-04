//
// Created by ofir on 26/08/2025.
//
#include <iostream>
#include "DNSParser.hpp"

std::string adblocker::DNSParser::ParseQuery(const uint8_t* data, size_t len)
{
    if (len < 12)
    {
        throw std::runtime_error("data size small for DNS query");
    }
    // uint8_t packet[] = {
    //     0xaa,0xaa, 0x01,0x00, 0x00,0x01, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    //     0x07,'s','a','n','j','a','g', 'h',
    //     0x03,'c','o','m',
    //     0x00,
    //     0x00,0x01, // QTYPE = A
    //     0x00,0x01  // QCLASS = IN
    // };
    std::string domain;
    size_t offset = 12;

    while(offset < len)
    {
        auto sectionLen = data[offset++];
        if(sectionLen == 0) break;

        if (!domain.empty()) domain.push_back('.');
        if (offset + sectionLen > len) throw std::runtime_error("Invalid DNS name");

        domain.append(reinterpret_cast<const char*>(&data[offset]), sectionLen);
        offset += sectionLen;
    }

    return domain;
}
