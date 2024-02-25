#include "log.h"

#include <iostream>
#include <iomanip>

namespace logger
{

static void write(const char* text, const char* type)
{
    time_t t = std::time(nullptr);
    tm time = *std::localtime(&t);
    std::cout << "[" << std::put_time(&time, "%Y-%m-%d %H-%M-%S") << "] " << "[" << type << "] " << text << std::endl;
}

void info(const char* text)
{
    write(text, "info");
}

void error(const char* text)
{
    write(text, "error");
}

}