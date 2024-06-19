
#include "wLog.h"

#define logger_cout(level) LogCapture(level, __FILE__, __LINE__, __FUNCTION__).stream()

#define LOG_ERROR logger_cout(LogLevel::Error)
#define LOG_WARN logger_cout(LogLevel::Warn)
#define LOG_INFO logger_cout(LogLevel::Info)
#define LOG_DEBUG logger_cout(LogLevel::Debug)
#define LOG_TRACE logger_cout(LogLevel::Trace)

#define LogerCout(level, fmt, ...) Wlogger::get_instance()->log(level, "[%s:%d][%s] " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

// 格式化日志  这两种方式 都一样
#define LogError(fmt, ...) LogerCout(LogLevel::Error, fmt, ##__VA_ARGS__)

#define LogWarn(fmt, args...) LogerCout(LogLevel::Warn, fmt, ##args)
#define LogInfo(fmt, args...) LogerCout(LogLevel::Info, fmt, ##args)
#define LogDebug(fmt, args...) LogerCout(LogLevel::Debug, fmt, ##args)

#define LogTrace(fmt, ...) LogerCout(LogLevel::Trace, fmt, ##__VA_ARGS__)

