#include "logger.h"

int main()
{
    auto start = std::chrono::steady_clock::now();
    int n = 10;
    while (n--)
    {
        LOG_ERROR << "hello world";
        LogInfo("nihao shijie\n");
    }
    auto end = std::chrono::steady_clock::now();
    LOG_INFO << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    LogInfo("nihao shijie\n");

    return 0;
}