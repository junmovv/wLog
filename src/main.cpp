#include "logger.h"

int main()
{
    Wlogger *p = Wlogger::get_instance();
    LOG_INFO << "hello";
    return 0;
}