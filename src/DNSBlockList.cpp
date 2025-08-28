
#include <fstream>

#include "DNSBlockList.hpp"

bool adblocker::DNSBlockList::Load(const std::string& path)
{
    std::ifstream file(path);
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
