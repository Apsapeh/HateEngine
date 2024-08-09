#include <HateEngine/Utilities/UUID.hpp>

using namespace HateEngine;

uint64_t UUID::counter = 0;
std::mutex UUID::mutex;

UUID::UUID() {
    // FIXME: Почему-то мьютексы не работают на винде с мсвц 15
    // std::lock_guard<std::mutex> guard(this->mutex);
    this->id = counter++;
}

UUID::UUID(const uint64_t& id) {
    this->id = id;
}

const uint64_t UUID::getU64() const {
    return this->id;
}

bool UUID::operator==(const UUID& other) const {
    return this->id == other.id;
}
