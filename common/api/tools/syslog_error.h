#pragma once

#include <iostream>
#include <sys/syslog.h>

inline void make_syslog_error(const std::string& message)
{
 std::cout << message << std::endl;
 syslog(LOG_ERR, "%s\n", message.c_str());
}