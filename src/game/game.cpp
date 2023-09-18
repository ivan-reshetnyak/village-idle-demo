#include "pch.h"

#include "game.h"
#include "units/unit_map.h"
#include "units/unit_player_stats.h"
#include "units/unit_friend_list.h"
#include "units/unit_shop.h"


bool game::OnCreate(CREATESTRUCT*)
{
    if (!mRender.Init(GetHandle())) {
        return false;
    }

    mControls.Init(GetInfo());

    auto unitMap = std::make_shared<units::map>(PATHS_XML "current_map.xml", &mControls, &mTimer,
                                                &playerInfo, &mFriends, GetInfo());
    unitManager.Add(0, unitMap);

    auto unitPlayerInfo = std::make_shared<units::player_stats>(&playerInfo);
    unitManager.Add(10, unitPlayerInfo);

    auto unitFriendList = std::make_shared<units::friend_list>(&mFriends,
        rect_opengl(-1.f, -0.6f, 1.f, -1.f), color(0.9843f, 0.5333f, 0.2588f),
        rect_opengl(-0.9f, -0.6f, -0.6f, -1.f), 0.025f, 4,
        GetInfo(), &mControls,
        coord_opengl(-0.95f, -0.8f), coord_opengl(0.375f, -0.8f), 32);
    unitManager.Add(10, unitFriendList);

    auto unitShop = std::make_shared<units::shop>(
        rect_opengl(-0.9f, 0.9f, 1.0f, -1.0f), &mControls,
        GetInfo(), &mTimer, &unitMap->mCamera, unitMap, &playerInfo);
    unitManager.Add(15, unitShop);

    return true;
}


void game::OnDestroy()
{
    mRender.Destroy();
    window::OnDestroy();
}


void game::OnPaint()
{
    mTimer.Update();
    unitManager.Update();

    mRender.StartFrame();

    unitManager.Render();

    mRender.EndFrame();
    mRender.CopyFrame();

    mTimer.IncrFrameCount();
}


void game::OnSize(UINT width, UINT height)
{
    window::OnSize(width, height);
    mRender.Resize(width, height);
}


int game::Run()
{
    Init("Castle Builder Main Window", "Castle Builder", 640, 480);
    return window::Run();
}
