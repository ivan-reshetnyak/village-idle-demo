#pragma once

#include <memory>
#include <string>

#include "paths.h"
#include "game/camera.h"
#include "game/unit.h"
#include "game/ui/ui_clickable.h"
#include "render/resource/texture.h"
#include "render/vertex.h"
#include "render/prim/points.h"
#include "math/common.h"


namespace units {
/// @brief Textured and colored billboard fixed in screnspace coordinates.
/// Optionally accounts for camera movement.
class billboard : public unit {
public:
    using ptr = std::weak_ptr<billboard>;


    class vertex : public ::vertex {
    public:
        coord_opengl topLeft, bottomRight;


        virtual std::vector<location> GetLayout() const;
        virtual int GetSize() const { return sizeof(vertex); }
    };


    /// @brief Class constructor.
    /// @param mtlName Must be unique (unless the texture is shared).
    /// @param bounds
    /// @param mTexture Texture to use.
    /// @param mColor Additional color (white by default).
    billboard(const std::string& mtlName, const rect_opengl& bounds,
        texture::ptr mTexture, const color& mColor = color(1.f, 1.f));
    /// @brief Class constructor.
    /// @param mtlName Unique (or from other untextured billboard).
    /// @param bounds
    /// @param mColor Additional color (white by default).
    billboard(const std::string& mtlName, const rect_opengl& bounds,
        const color& mColor = color(1.f, 1.f));
    virtual ~billboard() = default;

    void Move(const coord_opengl& delta);
    void Update(texture::ptr);
    /// @param newCamera nullptr to disable camera tracking.
    void Update(camera* newCamera) { pCamera = newCamera; }
    void Update(const color& newColor) { mColor = newColor; }

protected:
    static constexpr const char* SHADER_PATH = PATHS_SHADERS "billboard/";
    static constexpr const char* DEFAULT_TEXTURE = PATHS_IMG "white_pixel.png";

    prim::points mPrim;
    color mColor;
    camera* pCamera = nullptr;
    coord_opengl offset{0, 0};

    void CreatePrim(const coord& topLeft, const coord& bottomRight, material::ptr);
    void Render() override;
};


/// @brief Billoard with mouse interaction.
class billboard_clickable : public billboard {
public:
    using ptr = std::weak_ptr<billboard_clickable>;


    /// @brief Class constructor.
    /// @param mtlName Must be unique (unless the texture is shared).
    /// @param bounds
    /// @param mTexture Texture to use.
    /// @param mColor Additional color (white by default).
    billboard_clickable(const std::string& mtlName, const rect_opengl& bounds,
        texture::ptr mTexture, const color& mColor = color(1.f, 1.f));
    /// @brief Class constructor.
    /// @param mtlName Unique (or from other untextured billboard).
    /// @param bounds
    /// @param mColor Additional color (white by default).
    billboard_clickable(const std::string& mtlName, const rect_opengl& bounds,
        const color& mColor = color(1.f, 1.f));
    ~billboard_clickable();

    virtual void SetVisible(bool newVisibility = true) override;

    void SetCallbacks(controls*,
        const std::function<void()>& callbackLeft,
        const std::function<void()>& callbackRight = [](){});

protected:
    rect_opengl bounds;
    controls* pControls = nullptr;
    ui::clickable::ptr wUI;


    bool CheckBounds(controls*) const;
};
}
