#pragma once

#include <array>
#include <memory>
#include <unordered_map>

#include "paths.h"
#include "unit_map_building.h"
#include "unit_map_grid.h"
#include "game/camera.h"
#include "game/controls.h"
#include "game/friends.h"
#include "game/player_info.h"
#include "game/timer.h"
#include "game/unit.h"
#include "render/resource/material.h"
#include "render/prim/points.h"
#include "win/window.h"


namespace units {
class map : public unit {
public:
    using ptr = std::weak_ptr<map>;


    map(const std::string& xmlFile, controls*, timer*,
        player_info*, friends*, const window::info&);
    ~map();

    class data {
    public:
        texture::ptr backgroundTexture;
        texture::ptr zoomInTexture;
        texture::ptr zoomOutTexture;
        std::vector<std::shared_ptr<map_grid>> grids;


        data() = default;
        ~data() = default;
        /// @return true if load successful, false otherwise
        bool Load(const std::string& filename,
            int layer, unit_manager*, window::info,
            controls*, camera*, player_info*, friends*, timer*);
        void Save(const std::string& filename, player_info*, friends*);

    private:
        std::string bgTexName, zoomInTexName, zoomOutTexName;
    } mData;

    camera mCamera;

private:
    static constexpr const char* SHADER_PATH = PATHS_SHADERS "map/";


    class input_listener : public controls::listener {
    public:
        using ptr = std::shared_ptr<input_listener>;


        input_listener(camera* pCamera, data* mapData)
            : pCamera(pCamera), mapData(mapData)
        {
        }


        bool ProcessMouseEvent(controls*, controls::input_event) override;
        bool ProcessKeyboardEvent(controls*, controls::input_event, unsigned key) override;

    private:
        coord anchor{0, 0};
        bool isDragging = false;
        camera* pCamera;
        data* mapData;
    };


    input_listener::ptr dragListener;
    window::info winInfo;
    controls* pControls;
    friends* pFriends;
    player_info* playerInfo;
    timer* pTimer;
    std::string xmlFile;


    void OnAdd(unit_manager*) override;
};
}
