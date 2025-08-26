
#ifndef ADBLOCKER_DNSBLOCKLIST_HPP
#define ADBLOCKER_DNSBLOCKLIST_HPP

#include <string>
#include <unordered_set>

namespace adblocker
{
class DNSBlockList
{
public:
    explicit DNSBlockList() = default;
    ~DNSBlockList() = default;

    bool Load(const std::string& path);
    [[nodiscard]] bool IsValid(const std::string& domain) const;

    DNSBlockList(const DNSBlockList& other) = delete;
    DNSBlockList& operator=(const DNSBlockList& other) = delete;
    DNSBlockList(DNSBlockList&& other) = delete;
    DNSBlockList& operator=(DNSBlockList&& other) = delete;

private:
    std::unordered_set<std::string> m_domains;
}; // class DNSBlockList
} // namespace adblocker

#endif //ADBLOCKER_DNSBLOCKLIST_HPP