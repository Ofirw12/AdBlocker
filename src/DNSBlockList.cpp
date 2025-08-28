
#include <fstream>

#include "DNSBlockList.hpp"

adblocker::DNSBlockList::DNSBlockList(const std::string& path)
: m_path(path)
{}

bool adblocker::DNSBlockList::Load()
{
    std::ifstream file(m_path);
    if (!file.is_open())
    {
        return false;
    }

    std::string s;

    while (file.good())
    {
        std::getline(file, s);
        m_domains.insert(s);
    }

    return true;
}

bool adblocker::DNSBlockList::IsValid(const std::string& domain) const
{
    return !m_domains.contains(domain);
}