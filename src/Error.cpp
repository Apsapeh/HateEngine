#include "Error.h"
#include <iostream>

void Error::throwError(std::string msg, bool terminate) {
    std::cerr << msg << "\n";
    if (terminate)
        exit(1);
}