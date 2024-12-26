#pragma once

// Abstract resource class
namespace HateEngine {
    class Resource {
    protected:
        bool is_loaded = false;

    public:
        bool isLoaded() const {
            return is_loaded;
        }
    };
} // namespace HateEngine
