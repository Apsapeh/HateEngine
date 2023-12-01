#pragma once

#include <Old3DEngine/Utilities/UUID_Generator.hpp>

using namespace Old3DEngine;

// Dirty hack to check if glad is initialized (used in Texture.hpp)
extern bool glad_is_initialized;
extern UUID_Generator global_uuid_generator;