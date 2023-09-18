#pragma once

#include "paths.h"
#include "unit_map.h"
#include "game/controls.h"
#include "game/unit.h"
#include "math/common.h"
#include "win/window.h"


namespace units {
class shop : public unit {
public:
    class listener : public controls::listener {
    public:
        using ptr = std::weak_ptr<listener>;


        listener(unit_manager*, map::ptr,
            camera*, player_info*, timer*);
        virtual ~listener() = default;

        void Update(billboard::ptr newBB, map_building::desc::ptr newDesc)
        {
            previewBB = newBB;
            previewDesc = newDesc;
        }

    private:
        billboard::ptr previewBB;
        map_building::desc::ptr previewDesc;
        unit_manager* unitManager;
        map::ptr wMap;
        camera* pCamera;
        player_info* playerInfo;
        timer* pTimer;


        bool ProcessMouseEvent(controls*, controls::input_event) override;
    };


    shop(const rect_opengl& bounds, controls*,
        window::info, timer*, camera*, map::ptr,
        player_info*);
    ~shop();

private:
    static constexpr const char* ENTRANCE_TEX_PATH = PATHS_IMG "shop.png";


    timer* pTimer;
    controls* pControls;
    rect_opengl bounds;
    window::info winInfo;
    listener::ptr wListener;
    map::ptr wMap;
    camera* pCamera;
    player_info* playerInfo;


    void OnAdd(unit_manager*) override;
};
}
