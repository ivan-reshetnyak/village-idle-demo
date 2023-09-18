#include "pch.h"

#include "camera.h"


void camera::Move(const coord_opengl& delta)
{
    offset += delta;
    CheckOffsetValid();
}


void camera::CheckOffsetValid()
{
    if (!isBounded) {
        return;
    }

    float halfSize = 1.f * GetZoom();
    if (offset.x + halfSize < 1.f) {
        offset.x = 1.f - halfSize;
    }
    if (offset.x - halfSize > -1.f) {
        offset.x = -1.f + halfSize;
    }
    if (offset.y + halfSize < 1.f) {
        offset.y = 1.f - halfSize;
    }
    if (offset.y - halfSize > -1.f) {
        offset.y = -1.f + halfSize;
    }
}


void camera::SetZoomLevels(const std::vector<float>& newLevels)
{
    if (newLevels.size() < 1) {
        return;
    }

    zoomLevels = newLevels;
    currentZoomLevel = 0;

    CheckOffsetValid();
}


bool camera::ZoomIn()
{
    if (currentZoomLevel >= zoomLevels.size() - 1) {
        return false;
    }
    ++currentZoomLevel;
    CheckOffsetValid();
    return true;
}


bool camera::ZoomOut()
{
    if (currentZoomLevel <= 0) {
        return false;
    }
    --currentZoomLevel;
    CheckOffsetValid();
    return true;
}
