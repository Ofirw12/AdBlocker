
#ifndef ADBLOCKER_LOGGER_HPP
#define ADBLOCKER_LOGGER_HPP

#include <fstream>
#include <mutex>
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

    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    Logger(Logger&& other) = delete;
    Logger& operator=(Logger&& other) = delete;

    static Logger& GetInstance();

    void Init(const std::string& path);
    void Log(LogLevel level, const std::string& message);
private:
    Logger();
    ~Logger();

    std::ofstream m_file;
    std::vector<std::string> m_levelToStr;
    std::mutex m_mutex;
    bool m_isInitialized;
}; // class Logger
} // namespace adblocker

#endif //ADBLOCKER_LOGGER_HPP