#pragma once
#include <string>

#define HATE_INFO(msg) HateEngine::Log::Info(msg, __FILE__, __LINE__);
#define HATE_WARNING(msg) HateEngine::Log::Warning(msg, __FILE__, __LINE__);
#define HATE_ERROR(msg) HateEngine::Log::Error(msg, __FILE__, __LINE__);
#define HATE_FATAL(msg) HateEngine::Log::Fatal(msg, __FILE__, __LINE__);

//#define HATE_INFO_B std::cout << termcolor::green << "[INFO] "

namespace HateEngine {
    namespace Log {
        void Info(std::string msg, std::string file = "", unsigned int line = 0);
        void Warning(std::string msg, std::string file = "", unsigned int line = 0);
        void Error(std::string msg, std::string file = "", unsigned int line = 0);
        void Fatal(std::string msg, std::string file = "", unsigned int line = 0);
    };
}