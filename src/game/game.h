#pragma once

#include "win/window.h"
#include "render/render.h"
#include "camera.h"
#include "controls.h"
#include "friends.h"
#include "unit.h"
#include "timer.h"
#include "player_info.h"


class game : private window {
public:
    using ptr = std::shared_ptr<game>;


    game() = default;
    ~game() = default;


    int Run();

private:
    controls mControls;
    friends mFriends;
    render mRender;
    timer mTimer;
    player_info playerInfo;
    unit_manager unitManager;


    bool OnCreate(CREATESTRUCT*) override;
    void OnDestroy() override;
    void OnPaint() override;
    void OnSize(UINT width, UINT height) override;

    void OnMouseMove  (INT x, INT y) override { mControls.OnMouseMove(x, y); }
    void OnLButtonDown(INT x, INT y) override { mControls.OnMouseLDown(); }
    void OnMButtonDown(INT x, INT y) override { mControls.OnMouseMDown(); }
    void OnRButtonDown(INT x, INT y) override { mControls.OnMouseRDown(); }
    void OnLButtonUp  (INT x, INT y) override { mControls.OnMouseLUp(); }
    void OnMButtonUp  (INT x, INT y) override { mControls.OnMouseMUp(); }
    void OnRButtonUp  (INT x, INT y) override { mControls.OnMouseRUp(); }

    void OnKeyDown(UINT key) override { mControls.OnKeyDown(key); }
    void OnKeyUp  (UINT key) override { mControls.OnKeyUp(key); }
};
