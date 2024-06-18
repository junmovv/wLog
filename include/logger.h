
#include "wLog.h"

#define LoggerCout(level) LogCapture(level, __FILE__, __LINE__, __FUNCTION__).stream()

#define LOG_ERROR LoggerCout(LogLevel::Error)
#define LOG_WARN LoggerCout(LogLevel::Warn)
#define LOG_INFO LoggerCout(LogLevel::Info)
#define LOG_DEBUG LoggerCout(LogLevel::Debug)
#define LOG_TRACE LoggerCout(LogLevel::Trace)
