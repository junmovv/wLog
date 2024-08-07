#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <ios>
#include <iomanip>
#include <mutex>
#include <queue>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
// 文字颜色
#define ANSI_COLOR_RESET "\e[0m"
#define ANSI_COLOR_BLACK "\e[30m"
#define ANSI_COLOR_RED "\e[31m"
#define ANSI_COLOR_GREEN "\e[32m"
#define ANSI_COLOR_YELLOW "\e[33m"
#define ANSI_COLOR_BLUE "\e[34m"
#define ANSI_COLOR_MAGENTA "\e[35m"
#define ANSI_COLOR_CYAN "\e[36m"
#define ANSI_COLOR_WHITE "\e[37m"
enum class LogLevel : unsigned char
{
    Error,
    Warn,
    Info,
    Debug,
    Trace
};

static const char *leverStr[5] =
    {"[ERROR]", "[WARN] ", "[INFO] ", "[DEBUG]", "[TRACE]"};

struct Logger
{
    std::string logFileSwitch;          // 是否写入文件
    std::string logTerminalSwitch;      // 是否打印到终端
    std::string logFileQueueSwitch;     // 是否开启队列策略
    std::string logName;                // 日志文件名字
    std::string logFilePath;            // 日志文件保存路径
    std::string logMixSize;             // 日志文件最大大小
    std::string logBehavior;            // 日志文件达到最大大小行为
    std::string logOutputLevelFile;     // 日志输出等级(file)
    std::string logOutputLevelTerminal; // 日志输出等级
};

class Wlogger
{
public:
    static Wlogger *get_instance();
    void log(LogLevel level, const char *fmt, ...);

private:
    void init_log_config();
    void use_default_config();
    void print_config_info();

    std::string get_log_file_switch() const;
    std::string get_log_terminal_switch() const;

    std::string get_file_path_name() const;
    void open_log_file();
    void set_file_terminal_type();
    bool get_file_type(LogLevel fileLevel) const;
    bool get_terminal_type(LogLevel terminalLevel) const;

    void write_log_to_file(const char *fmt, va_list args);

    void insert_queue(const char *buf);

    std::string get_log_time();

private:
    std::mutex fileLock_;
    Logger logger;

    std::ofstream file_;
    unsigned int fileType_;
    unsigned int terminalType_;

    std::queue<std::string> messageQueue_;

private:
    Wlogger();
    ~Wlogger();
    Wlogger(const Wlogger &other) = delete;
    Wlogger &operator=(const Wlogger &other) = delete;
};

class LogCapture
{
public:
    LogCapture(const LogLevel level, const std::string &file, const uint32_t line, const std::string &function,
               const std::string &check_expression = "");
    ~LogCapture();

public:
    std::ostringstream &stream();

private:
    std::string get_file_line_func();

private:
    std::ostringstream sstream_;

    LogLevel level_;
    std::string file_;
    uint32_t line_;
    std::string func_;
    std::string check_expression_;
};
