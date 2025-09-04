
#include "Logger.hpp"

using namespace adblocker;

int main()
{
    Logger logger("test.log");
    logger.Log(WARNING, "Test is Best");
    logger.Log(CRITICAL, "Test is Bad");
    logger.Log(INFO, "Test is OK");
}
