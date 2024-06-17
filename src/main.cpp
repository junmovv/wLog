#include "logger.h"

int main()
{
    auto start = std::chrono::steady_clock::now();
    int n = 1000 * 1000;
    while (n--)
    {
        LOG_INFO << "hello world";
    }

    auto end = std::chrono::steady_clock::now();
    LOG_INFO << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return 0;
}