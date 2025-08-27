//
// Created by ofir on 26/08/2025.
//
#include "DNSParser.hpp"

bool adblocker::DNSParser::ParseQuery(const uint8_t* data, size_t len,
    DNSQuestion& out)
{
}

std::vector<uint8_t> adblocker::DNSParser::BuildBlockResponse(
    const uint8_t* query, size_t qlen, const DNSQuestion& q)
{
}
