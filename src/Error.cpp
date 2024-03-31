#include <iostream>
#include <HateEngine/Error.hpp>
using namespace HateEngine;

void Error::throwError(std::string msg, bool terminate) {
    std::cerr << msg << "\n";
    if (terminate)
        exit(1);
}

void Error::throwWarning(std::string msg) {
    std::cerr << msg << "\n";
}
