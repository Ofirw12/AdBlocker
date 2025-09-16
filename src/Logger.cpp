
#include <iostream>
#include <sstream>

#include "Logger.hpp"

adblocker::Logger::Logger()
:m_levelToStr({"DEBUG", "INFO","WARNING",
    "ERROR", "CRITICAL"}), m_isInitialized(false)
{}

adblocker::Logger::~Logger()
{
    if (m_file.is_open())
    {
        m_file.close();
    }
}

adblocker::Logger& adblocker::Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

void adblocker::Logger::Init(const std::string& path)
{
    std::unique_lock lock(m_mutex);
    if (!m_isInitialized)
    {
        m_file.open(path, std::ios::app);
        if (!m_file.is_open())
        {
            std::cerr << "Failed to open file " << path << std::endl;
        }
        else
        {
            m_isInitialized = true;
        }
    }
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

    {
        std::unique_lock lock(m_mutex);
        if (m_file.is_open())
        {
            m_file << entry.str();
            m_file.flush();
        }
    }
}
