#include "wLog.h"

Wlogger *Wlogger::get_instance()
{
    static Wlogger singleObject;
    return &singleObject;
}

Wlogger::Wlogger()
{
    this->init_log_config();
}

void Wlogger::init_log_config()
{
    char strc[128] = {0};
    std::map<std::string, std::string *> logConfInfo;
    logConfInfo["logSwitch"] = &this->logger.logSwitch;
    logConfInfo["logFileSwitch"] = &this->logger.logFileSwitch;
    logConfInfo["logTerminalSwitch"] = &this->logger.logTerminalSwitch;
    logConfInfo["logFileQueueSwitch"] = &this->logger.logFileQueueSwitch;
    logConfInfo["logName"] = &this->logger.logName;
    logConfInfo["logFilePath"] = &this->logger.logFilePath;
    logConfInfo["logMixSize"] = &this->logger.logMixSize;
    logConfInfo["logBehavior"] = &this->logger.logBehavior;
    logConfInfo["logOutputLevelFile"] = &this->logger.logOutputLevelFile;
    logConfInfo["logOutputLevelTerminal"] = &this->logger.logOutputLevelTerminal;
    std::ifstream file("../config/logConf.conf");

    if (!file.is_open())
    {
        std::cout << "File open failed" << std::endl;
        return;
    }
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
    print_config_info();
}

void Wlogger::print_config_info()
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
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  日志开关　　" << logger.logSwitch << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  文件输出　　" << logger.logFileSwitch << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  终端输出开关" << logger.logTerminalSwitch << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  文件输出等级" << logger.logOutputLevelFile << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  终端输出等级" << logger.logOutputLevelTerminal << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  日志队列策略" << logger.logFileQueueSwitch << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  日志文件名称" << logger.logName << ".log" << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  日志保存路径" << logger.logFilePath << ANSI_COLOR_RESET << std::endl;
    std::cout << ANSI_COLOR_GREEN << std::left << std::setw(25) << "  日志文件大小" << logger.logMixSize << "M" << ANSI_COLOR_RESET << std::endl;
    for (int i = 0; i < logger.logFilePath.size() + 25; i++)
    {
        std::cout << ANSI_COLOR_GREEN << "-";
    }
    std::cout << ANSI_COLOR_RESET << std::endl;
}

LogCapture::LogCapture(const LogLevel level, const std::string &file, const uint32_t line,
                       const std::string &function, const std::string &check_expression)
    : level_(level), file_(file), line_(line), func_(function), check_expression_(check_expression)
{
}
std::ostringstream &LogCapture::stream()
{
    return sstream_;
}

std::string LogCapture::get_log_time()
{
    struct timeval now;
    ::gettimeofday(&now, nullptr);
    struct tm tm_now;
    ::localtime_r(&now.tv_sec, &tm_now);
    char time_str[100] = {0};
    snprintf(time_str, sizeof(time_str), "[%04d-%02d-%02d %02d:%02d:%02d.%06ld][%d]", tm_now.tm_year + 1900,
             tm_now.tm_mon + 1, tm_now.tm_mday, tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec, now.tv_usec, ::getpid());
    return time_str;
}

std::string LogCapture::get_file_line_func()
{
    std::ostringstream ostr;
    ostr << "[" << file_ << ":" << line_ << "]" << "[" << func_ << "]";
    return ostr.str();
}

LogCapture::~LogCapture()
{
    sstream_ << "[INFO]" << get_log_time() << get_file_line_func() << std::endl;
    // Logger::Instance()->Log(level_, "[%s:%d][%s] %s", file_.c_str(), line_, function_.c_str(), sstream_.str().c_str());
    std::cout << sstream_.str();
}