
#include "wLog.h"

#define LoggerCout(level) LogCapture(level, __FILE__, __LINE__, __FUNCTION__).stream()

#define LOG_INFO LoggerCout(LogLevel::Info)
