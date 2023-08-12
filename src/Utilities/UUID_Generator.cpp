#include "UUID_Generator.h"

using namespace Old3DEngine;

UUID_Generator::UUID UUID_Generator::gen() {
    std::lock_guard<std::mutex> mut(this->getMutex);
    uint64_t timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();

    if (timeMillis > prevTime) {
        prevTime = timeMillis;
        idTimeAdder = 0;
    }
    else
        idTimeAdder += 1;

    return {timeMillis, idTimeAdder};
}
