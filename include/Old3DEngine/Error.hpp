#pragma once
#include <string>

namespace Old3DEngine {
    namespace Error {
        void throwError(std::string msg, bool terminate = true);
        void throwWarning(std::string msg);
    };
}