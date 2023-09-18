#pragma once

#include "math/common.h"


/// @brief Allows 2d movement and discrete zoom.
class camera {
public:
    camera(bool bounded = true)
        : isBounded(bounded)
    {
    }
    ~camera() = default;

    const coord_opengl& GetOffset() const { return offset; }
    void Move(const coord_opengl& delta);

    void SetZoomLevels(const std::vector<float>&);
    float GetZoom() const { return zoomLevels[currentZoomLevel]; }
    /// @return true if zoom level changed
    bool ZoomIn();
    /// @return true if zoom level changed
    bool ZoomOut();

private:
    bool isBounded;
    int currentZoomLevel = 0;
    coord_opengl offset{0, 0};
    std::vector<float> zoomLevels = {1.0f};


    void CheckOffsetValid();
};
