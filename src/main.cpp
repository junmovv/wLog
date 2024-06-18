#include "logger.h"

int main()
{
    auto start = std::chrono::steady_clock::now();
    int n = 10;
    while (n--)
    {
        LOG_ERROR << "hello world";
        LOG_WARN << "hello world";
        LOG_INFO << "hello world";
        LOG_DEBUG << "hello world";
        LOG_TRACE << "hello world";
    }

    auto end = std::chrono::steady_clock::now();
    LOG_INFO << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return 0;
}