#pragma once

#include <vector>

#include "paths.h"
#include "game/unit.h"
#include "render/prim/points.h"
#include "math/common.h"


namespace units {
/// @brief Progress bar with fixed screenspace boundaries.
class progress_bar : public unit {
public:
    using ptr = std::weak_ptr<progress_bar>;


    /// @brief Progress bar lerping from emptyColor to fullColor.
    /// @param bounds OpenGL screen coordinates
    progress_bar(const rect_opengl& bounds, const color& backgroundColor,
        const color& emptyColor, const color& fullColor);
    /// @brief Progress bar filling with single color.
    /// @param bounds OpenGL screen coordinates
    progress_bar(const rect_opengl& bounds, const color& backgroundColor,
        const color& fillColor);
    /// @brief Progress bar lerping between all milestones.
    /// @param bounds OpenGL screen coordinates
    /// @param milestones Must be sorted,
    progress_bar(const rect_opengl& bounds, const color& backgroundColor,
        const std::vector<std::pair<float, color>>& milestones);


    void SetProgress(float newProgress) { progress = newProgress; }


    void Render() override;

protected:
    static constexpr const char* SHADER_PATH = PATHS_SHADERS "progress_bar/";
    static constexpr const char* MTL_NAME = "PROGRESS_BAR_MATERIAL";


    float progress = 0.f;
    std::vector<std::pair<float, color>> milestones;
    prim::points mPrim;


    color GetColor(float t) const;
    void CreatePrim(const rect_opengl& bounds, const color& backgroundColor);
};
}
