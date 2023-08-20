#include <iostream>
#include <Old3DEngine/Error.hpp>
using namespace Old3DEngine;

void Error::throwError(std::string msg, bool terminate) {
    std::cerr << msg << "\n";
    if (terminate)
        exit(1);
}

void Error::throwWarning(std::string msg) {
    std::cerr << msg << "\n";
}
