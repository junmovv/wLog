#include <iostream>
#include <string>
#include <map>
#include <fstream>
const char *const RED = "\e[1;31m";
const char *const BLUE = "\e[1;34m";
const char *const GREEN = "\e[1;32m";
const char *const WHITE = "\e[1;37m";
const char *const PURPLE = "\e[1;35m";

struct Logger
{
    std::string logSwitch;              // 日志开关
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
    void init_log_config();

private:
    Logger logger;

private:
    Wlogger();
    ~Wlogger();
};
