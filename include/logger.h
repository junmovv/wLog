
#include "wLog.h"

#define LoggerCout(level)                                            \
    do                                                               \
    {                                                                \
        LogCapture(level, __FILE__, __LINE__, __FUNCTION__).stream() \
    } while (0);

#define LOG_INFO LoggerCout(LogLevel::Info)
