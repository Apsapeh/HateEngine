#include <iostream>
#include <HateEngine/Log.hpp>
#include <termcolor/termcolor.hpp>
#include <sys/time.h>

using namespace HateEngine;

//Example: 09-08-2024 18:04:15.115 [INFO] [src/main.cpp@10] Hello World!

std::string log_format(const std::string& level, const std::string& msg, const std::string& file, unsigned int line) {
    // date and time
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%d-%m-%Y %H:%M:%S", ltm);
    //miliseconds
    timeval tv;
    gettimeofday(&tv, NULL);
    int milliseconds = tv.tv_usec / 1000;
    //format
    sprintf(buffer, "%s.%03d", buffer, milliseconds);
    
    std::string datetime(buffer );
    
    return datetime + " [" + level + "] [" + file + "@" + std::to_string(line) + "] " + msg;
}

void Log::Info(std::string msg, std::string file, unsigned int line) {
    std::cout << termcolor::reset << log_format("INFO", msg, file, line) << termcolor::reset << "\n";
}

void Log::Warning(std::string msg, std::string file, unsigned int line) {
    std::cerr << termcolor::reset << termcolor::yellow << log_format("WARNING", msg, file, line) << termcolor::reset << "\n";
}

void Log::Error(std::string msg, std::string file, unsigned int line) {
    std::cerr << termcolor::reset << termcolor::red << log_format("ERROR", msg, file, line) << termcolor::reset << "\n";
}

void Log::Fatal(std::string msg, std::string file, unsigned int line) {
    std::cerr << termcolor::reset << termcolor::on_red << log_format("FATAL", msg, file, line) << termcolor::reset << "\n";
    exit(1);
}

