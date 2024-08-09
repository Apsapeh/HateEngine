#pragma once
#include <string>

#define HATE_DEBUG(msg) HateEngine::Log::Debug(msg, __FILE__, __LINE__);
#define HATE_INFO(msg) HateEngine::Log::Info(msg, __FILE__, __LINE__);
#define HATE_WARNING(msg) HateEngine::Log::Warning(msg, __FILE__, __LINE__);
#define HATE_ERROR(msg) HateEngine::Log::Error(msg, __FILE__, __LINE__);
#define HATE_FATAL(msg) HateEngine::Log::Fatal(msg, __FILE__, __LINE__);

#define HATE_DEBUG_F(msg, ...)                                                                     \
    HateEngine::Log::Debug(HateEngine::LogFn::__log_format(msg, __VA_ARGS__), __FILE__, __LINE__);


namespace HateEngine {
    namespace Log {
        void Debug(std::string msg, std::string file = "", unsigned int line = 0);
        void Info(std::string msg, std::string file = "", unsigned int line = 0);
        void Warning(std::string msg, std::string file = "", unsigned int line = 0);
        void Error(std::string msg, std::string file = "", unsigned int line = 0);
        void Fatal(std::string msg, std::string file = "", unsigned int line = 0);
    }; // namespace Log

    namespace LogFn {
        std::string __log_message_template(
                const std::string& level, const std::string& msg, const std::string& file,
                unsigned int line
        );

        std::string __log_format(const std::string msg, ...);

    } // namespace LogFn
} // namespace HateEngine
