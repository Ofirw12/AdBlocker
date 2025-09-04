
#include "Logger.hpp"

#include <iostream>
#include <sstream>

adblocker::Logger::Logger(const std::string& path)
: m_file(path, std::ios::app),
    m_levelToStr({"DEBUG", "INFO",
                                    "WARNING", "ERROR", "CRITICAL"})
{
    if (!m_file.is_open())
    {
        std::cerr << "Failed to open log file " << path << std::endl;
    }
}

adblocker::Logger::~Logger()
{
    m_file.close();
}

void adblocker::Logger::Log(LogLevel level, const std::string& message)
{
    const time_t now = time(nullptr);
    tm* timeinfo = localtime(&now);
    std::vector<char> buffer(20);
    strftime(buffer.data(), buffer.size(),
                                        "%Y/%m/%d %H:%M:%S", timeinfo);
    std::ostringstream entry;
    entry << "[" << buffer.data() << "] " << m_levelToStr[level]
                                << " " << message << std::endl;

    m_file << entry.str();
    m_file.flush();
}