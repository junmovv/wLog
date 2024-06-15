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
    std::ifstream file("../../conf/fdoglogconf.conf");

    if (!file.is_open())
    {
        std::cout << "File open failed" << std::endl;
    }
}