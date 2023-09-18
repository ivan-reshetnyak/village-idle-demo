#pragma once

#include <cstdio>

#include "render/resource/texture.h"


namespace tex {
class img : public texture {
public:
    img() = default;
    virtual ~img() = default;

    bool Load(const std::string& fileName);

protected:
    int width = 0, height = 0, channels = 0;

    bool LoadJPEG(const std::string& fileName, std::unique_ptr<uint8_t[]>& buffer);
    bool LoadPNG(const std::string& fileName, std::unique_ptr<uint8_t[]>& buffer);
};
}
