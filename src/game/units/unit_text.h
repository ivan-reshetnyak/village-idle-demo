#pragma once

#include <string>

#include "game/unit.h"
#include "render/prim/trimesh.h"
#include "math/common.h"


namespace units {
/// @brief Text output. Defined by the bounds of its first letter.
/// Expects a monospace bitmap ASCII font.
class text : public unit {
public:
    using ptr = std::weak_ptr<text>;


    struct font {
        const std::string texturePath;
        const char firstRenderableChar;
        const int numRenderableChars;
    };


    static font FONT_SPLEEN;


    void Render() override;

    text(const rect_opengl& firstLetterBound, const font& mFont, const std::string& text,
         const color& textColor = color(1.f, 1.f), const color& backgroundColor = color(0.f, 0.f));
    void Update(const std::string&);

protected:
    static constexpr const char* SHADER_PATH = PATHS_SHADERS "text/";


    bool isInitialized = false;
    rect_opengl firstLetterBound;
    font mFont;
    prim::trimesh mPrim;
    color textColor;
    color backgroundColor;
};
}
