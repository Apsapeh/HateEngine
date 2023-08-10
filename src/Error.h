#pragma once
#include <string>

namespace Error {
    void throwError(std::string msg, bool terminate = true);
};
