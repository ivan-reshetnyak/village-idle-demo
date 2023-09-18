#include "pch.h"

#include "render/resource/shader.h"
#include "render/resource/tex/tex_img.h"
#include "render/geometry.h"
#include "render/vertex.h"
#include "unit_map.h"
#include "unit_button.h"
#include "unit_billboard.h"


namespace units {
bool map::input_listener::ProcessMouseEvent(controls* ctrls, controls::input_event evType)
{
    if (!isDragging) {
        if (evType == controls::input_event::MOUSE_L_DOWN) {
            isDragging = true;
            return true;
        }
        return false;
    }

    if (evType == controls::input_event::MOUSE_L_UP) {
        isDragging = false;
        return true;
    }

    if (evType == controls::input_event::MOUSE_MOVE) {
        pCamera->Move(ctrls->GetMouse().GetDeltaOpenGL());
    }

    return false;
}


bool map::input_listener::ProcessKeyboardEvent(controls*, controls::input_event evType, unsigned key)
{
    if (evType == controls::input_event::KEYBOARD_KEY_DOWN
        && key == 'G') {
        for (auto& pGrid : mapData->grids) {
            pGrid->ToggleGridVisible();
        }
    }
    return false;
}


map::map(const std::string& xmlFile,
         controls* ctrls, timer* pTimer,
         player_info* pInfo, friends* pFriends,
         const window::info& winInfo)
    : winInfo(winInfo), pControls(ctrls)
    , pFriends(pFriends), playerInfo(pInfo)
    , pTimer(pTimer), xmlFile(xmlFile)
{
    mCamera.SetZoomLevels({1.0, 1.5, 2.0});
}


void map::OnAdd(unit_manager* unitManager)
{
    if (!mData.Load(xmlFile, GetLayer(), unitManager, winInfo, pControls,
                    &mCamera, playerInfo, pFriends, pTimer)) {
        return;
    }

    auto mapBB = std::make_shared<billboard>("MAP_BACKGROUND_BILLBOARD",
        rect_opengl(-1.0f, 1.0f, 1.0f, -1.0f), mData.backgroundTexture);
    mapBB->Update(&mCamera);
    unitManager->Add(GetLayer(), mapBB);
    AddChild(mapBB);

    /* Initialize zoom buttons */
    auto buttonZoomIn = std::make_shared<button>("MapUnitZoomInButton",
        coord_opengl(0.9f, 0.1f), 32, 32,
        mData.zoomInTexture, winInfo, pControls,
        [this](){ mCamera.ZoomIn(); });
    unitManager->Add(GetLayer() + 5, buttonZoomIn);
    AddChild(buttonZoomIn);

    auto buttonZoomOut = std::make_shared<button>("MapUnitZoomOutButton",
        coord_opengl(0.9f, -0.1f), 32, 32,
        mData.zoomOutTexture, winInfo, pControls,
        [this](){ mCamera.ZoomOut(); });
    unitManager->Add(GetLayer() + 5, buttonZoomOut);
    AddChild(buttonZoomOut);

    /* Initialize drag listener */
    dragListener = std::make_shared<input_listener>(&mCamera, &mData);
    pControls->Add(GetLayer(), dragListener);
}


map::~map()
{
    mData.Save(xmlFile, playerInfo, pFriends);
}
}
