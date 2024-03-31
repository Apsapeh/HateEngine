#pragma once
#include <string>

namespace HateEngine {
    namespace Error {
        void throwError(std::string msg, bool terminate = true);
        void throwWarning(std::string msg);
    };
}