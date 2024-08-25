#include <sys/time.h>

#include <HateEngine/Log.hpp>
#include <cstdarg>
#include <iostream>
#include <termcolor/termcolor.hpp>

using namespace HateEngine;

// Example: 09-08-2024 18:04:15.115 [INFO] [src/main.cpp@10] Hello World!

static char* buffer = new char[4096];
std::string LogFn::__log_format(const std::string msg, ...) {
    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 4096, msg.c_str(), args);
    va_end(args);
    return std::string(buffer);
}

std::string LogFn::__log_message_template(
        const std::string& level, const std::string& msg, const std::string& file, unsigned int line
) {

    // date and time
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%d-%m-%Y %H:%M:%S", ltm);
    // miliseconds
    timeval tv;
    gettimeofday(&tv, NULL);
    int milliseconds = tv.tv_usec / 1000;
    // format
    sprintf(buffer, "%s.%03d", buffer, milliseconds);

    std::string datetime(buffer);

    return datetime + " [" + level + "] [" + file + "@" + std::to_string(line) + "] " + msg;
}

void Log::Debug(std::string msg, std::string file, unsigned int line) {
    // #ifdef __HATE_ENGINE_DEBUG
    std::cout << termcolor::reset << termcolor::green
              << LogFn::__log_message_template("DEBUG", msg, file, line) << termcolor::reset
              << "\n";
    // #endif
}

void Log::Info(std::string msg, std::string file, unsigned int line) {
    std::cout << termcolor::reset << LogFn::__log_message_template("INFO", msg, file, line)
              << termcolor::reset << "\n";
}

void Log::Warning(std::string msg, std::string file, unsigned int line) {
    std::cerr << termcolor::reset << termcolor::yellow
              << LogFn::__log_message_template("WARNING", msg, file, line) << termcolor::reset
              << "\n";
}

void Log::Error(std::string msg, std::string file, unsigned int line) {
    std::cerr << termcolor::reset << termcolor::red
              << LogFn::__log_message_template("ERROR", msg, file, line) << termcolor::reset
              << "\n";
}

void Log::Fatal(std::string msg, std::string file, unsigned int line) {
    std::cerr << termcolor::reset << termcolor::on_red
              << LogFn::__log_message_template("FATAL", msg, file, line) << termcolor::reset
              << "\n";
    exit(1);
}
