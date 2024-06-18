
#include "wLog.h"

#define LoggerCout(level) LogCapture(level, __FILE__, __LINE__, __FUNCTION__).stream()

#define LOG_ERROR LoggerCout(LogLevel::Error)
#define LOG_WARN LoggerCout(LogLevel::Warn)
#define LOG_INFO LoggerCout(LogLevel::Info)
#define LOG_DEBUG LoggerCout(LogLevel::Debug)
#define LOG_TRACE LoggerCout(LogLevel::Trace)

// 格式化日志  这两种方式 都一样
#define LogError(fmt, ...) Wlogger::get_instance()->log(LogLevel::Error, fmt, ##__VA_ARGS__)

#define LogWarn(fmt, args...) Wlogger::get_instance()->log(LogLevel::Warn, fmt, ##args)
#define LogInfo(fmt, args...) Wlogger::get_instance()->log(LogLevel::Info, fmt, ##args)
#define LogDebug(fmt, args...) Wlogger::get_instance()->log(LogLevel::Debug, fmt, ##args)

#define LogTrace(fmt, ...) Wlogger::get_instance()->log(LogLevel::Trace, fmt, ##__VA_ARGS__)
