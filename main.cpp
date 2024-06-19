#include "wLog.hpp"

extern void print();

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

        LogError("nihao");
        LogWarn("nihao");
        LogInfo("nihao");
        LogDebug("nihao");
        LogTrace("nihao");
        print();
    }
    auto end = std::chrono::steady_clock::now();
    // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    return 0;
}