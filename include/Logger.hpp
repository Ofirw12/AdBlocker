
#ifndef ADBLOCKER_LOGGER_HPP
#define ADBLOCKER_LOGGER_HPP

#include <fstream>
#include <string>
#include <vector>

namespace adblocker
{

enum LogLevel
{
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    CRITICAL = 4
};

class Logger
{
public:
    Logger(const std::string& path);
    ~Logger();
    void Log(LogLevel level, const std::string& message);
private:
    std::ofstream m_file;
    std::vector<std::string> m_levelToStr;
}; // class Logger
} // namespace adblocker

#endif //ADBLOCKER_LOGGER_HPP