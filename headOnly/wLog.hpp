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

inline Wlogger *Wlogger::get_instance()
{
    static Wlogger singleObject;
    return &singleObject;
}

inline Wlogger::Wlogger() : fileType_(0), terminalType_(0)
{
    this->init_log_config();
}

inline Wlogger::~Wlogger()
{
    while (!messageQueue_.empty())
    {
        file_ << messageQueue_.front();
        messageQueue_.pop();
    }

    if (file_.is_open())
    {
        file_.close();
    }
}

inline void Wlogger::init_log_config()
{
    char strc[128] = {0};
    std::map<std::string, std::string *> logConfInfo;
    logConfInfo["logFileSwitch"] = &this->logger.logFileSwitch;
    logConfInfo["logTerminalSwitch"] = &this->logger.logTerminalSwitch;
    logConfInfo["logFileQueueSwitch"] = &this->logger.logFileQueueSwitch;
    logConfInfo["logName"] = &this->logger.logName;
    logConfInfo["logFilePath"] = &this->logger.logFilePath;
    logConfInfo["logMixSize"] = &this->logger.logMixSize;
    logConfInfo["logBehavior"] = &this->logger.logBehavior;
    logConfInfo["logOutputLevelFile"] = &this->logger.logOutputLevelFile;
    logConfInfo["logOutputLevelTerminal"] = &this->logger.logOutputLevelTerminal;

    std::ifstream file("./config/logConf.conf");
    if (!file.is_open())
    {

        std::cout << "[" << __FILE__ << ":" << __LINE__ << "]" << "[" << __FUNCTION__ << "] " << "File open failed  use default config  " << std::endl;
        use_default_config();
    }
    else
    {
        std::string str;
        while (std::getline(file, str))
        {
            if (!str.length())
            {
                continue;
            }

            std::string str_copy = str;
            str_copy.erase(std::remove(str_copy.begin(), str_copy.end(), ' '), str_copy.end());
            if (str_copy[0] != '#')
            {
                sscanf(str_copy.data(), "%[^=]", strc);
                auto iter = logConfInfo.find(strc);
                if (iter != logConfInfo.end())
                {
                    sscanf(str_copy.data(), "%*[^=]=%s", strc);
                    *iter->second = strc;
                }
            }
        }
        file.close();
        open_log_file();
    }

    set_file_terminal_type();
    print_config_info();
}

inline void Wlogger::use_default_config()
{
    logger.logTerminalSwitch = "on";
    logger.logOutputLevelTerminal = "0";
}

inline void Wlogger::open_log_file()
{
    int ret = mkdir(logger.logFilePath.c_str(), S_IRWXU | S_IRWXG | S_IXOTH);
    if (ret != 0 && errno != EEXIST)
    {
        printf("mkdir fail, dir:%s,err:%s \n", logger.logFilePath.c_str(), strerror(errno));
    }
    file_.open(get_file_path_name());
}
inline void Wlogger::set_file_terminal_type()
{
    std::vector<int> terminalState;
    std::istringstream ss1(logger.logOutputLevelTerminal);
    std::string token1;
    while (std::getline(ss1, token1, ','))
    {
        terminalState.push_back(std::stoi(token1));
    }
    for (int index : terminalState)
    {
        terminalType_ |= (1 << index);
    }

    std::vector<int> fileState;
    std::istringstream ss2(logger.logOutputLevelFile);
    std::string token2;
    while (std::getline(ss2, token2, ','))
    {
        fileState.push_back(std::stoi(token2));
    }
    for (int index : fileState)
    {
        fileType_ |= (1 << index);
    }
}

inline bool Wlogger::get_file_type(LogLevel fileLevel) const
{
    return fileType_ & (1 << static_cast<int>(fileLevel));
}
inline bool Wlogger::get_terminal_type(LogLevel terminalLevel) const
{
    return terminalType_ & (1 << static_cast<int>(terminalLevel));
}

inline void Wlogger::print_config_info()
{

    for (int i = 0; i < logger.logFilePath.size() + 15; i++)
    {
        std::cout << ANSI_COLOR_GREEN << "-";
        if (i == (logger.logFilePath.size() + 15) / 2)
        {
            std::cout << "Wlogger";
        }
    }
    std::cout << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  文件输出开关" << logger.logFileSwitch << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  终端输出开关" << logger.logTerminalSwitch << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  文件输出等级" << logger.logOutputLevelFile << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  终端输出等级" << logger.logOutputLevelTerminal << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  日志队列策略" << logger.logFileQueueSwitch << ANSI_COLOR_RESET << std::endl;
    std::string logName;
    if (!logger.logName.empty())
    {
        logName = logger.logName + ".log";
    }
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  日志文件名称" << logName << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  日志保存路径" << logger.logFilePath << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  日志文件大小" << logger.logMixSize << ANSI_COLOR_RESET << std::endl;
    for (int i = 0; i < logger.logFilePath.size() + 25; i++)
    {
        std::cout << ANSI_COLOR_GREEN << "-";
    }
    std::cout << ANSI_COLOR_RESET << std::endl;
}

inline std::string Wlogger::get_log_time()
{
    struct timeval now;
    ::gettimeofday(&now, nullptr);
    struct tm tm_now;
    ::localtime_r(&now.tv_sec, &tm_now);
    char time_str[100] = {0};
    snprintf(time_str, sizeof(time_str), "[%04d-%02d-%02d %02d:%02d:%02d.%03ld][%d]", tm_now.tm_year + 1900,
             tm_now.tm_mon + 1, tm_now.tm_mday, tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec, now.tv_usec / 1000, ::getpid());
    return time_str;
}

inline std::string Wlogger::get_log_file_switch() const
{
    return logger.logFileSwitch;
}

inline std::string Wlogger::get_log_terminal_switch() const
{
    return logger.logTerminalSwitch;
}

inline std::string Wlogger::get_file_path_name() const
{
    return logger.logFilePath + "/" + logger.logName + ".log";
}

inline void Wlogger::write_log_to_file(const char *fmt, va_list args)
{
    char buf[4096] = {0};
    vsnprintf(buf, sizeof(buf), fmt, args);

    if (logger.logFileQueueSwitch == "on")
    {
        insert_queue(buf);
    }
    else
    {

        std::lock_guard<std::mutex> locker(fileLock_);
        if (file_.is_open())
        {
            file_ << buf;
        }
    }
}

inline void Wlogger::insert_queue(const char *buf)
{
    std::lock_guard<std::mutex> locker(fileLock_);
    messageQueue_.push(buf);
    if (messageQueue_.size() > 5000)
    {

        while (!messageQueue_.empty() && file_.is_open())
        {
            file_ << messageQueue_.front();
            messageQueue_.pop();
        }
    }
}
inline void Wlogger::log(LogLevel level, const char *fmt, ...)
{
    std::string str = leverStr[static_cast<int>(level)] + get_log_time() + fmt + '\n';
    va_list args;

    if (get_log_terminal_switch() == "on" && get_terminal_type(level))
    {
        va_start(args, fmt);
        vprintf(str.c_str(), args);
        va_end(args);
    }

    if (get_log_file_switch() == "on" && get_file_type(level))
    {
        va_start(args, fmt);
        write_log_to_file(str.c_str(), args);
        va_end(args);
    }
}
inline LogCapture::LogCapture(const LogLevel level, const std::string &file, const uint32_t line,
                              const std::string &function, const std::string &check_expression)
    : level_(level), file_(file), line_(line), func_(function), check_expression_(check_expression)
{
}

inline LogCapture::~LogCapture()
{
    Wlogger::get_instance()->log(level_, "[%s:%d][%s] %s", file_.c_str(), line_, func_.c_str(), sstream_.str().c_str());
}

inline std::ostringstream &LogCapture::stream()
{
    return sstream_;
}

inline std::string LogCapture::get_file_line_func()
{
    std::ostringstream ostr;
    ostr << "[" << file_ << ":" << line_ << "]" << "[" << func_ << "] ";
    return ostr.str();
}

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
