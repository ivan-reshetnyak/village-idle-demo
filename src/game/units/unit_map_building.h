#pragma once

#include <functional>

#include "RapidXml/rapidxml.hpp"

#include "paths.h"
#include "game/camera.h"
#include "game/unit.h"
#include "game/player_info.h"
#include "game/timer.h"
#include "game/ui/ui_clickable.h"
#include "render/resource/material.h"
#include "render/prim/points.h"
#include "win/window.h"


namespace units {
class map_building : public unit {
public:
    using ptr = std::weak_ptr<map_building>;
    using storage_type = std::shared_ptr<map_building>;


    class desc {
    private:
        friend resource_manager_named<desc>;
        friend class manager_t;
        class manager_t : public resource_manager_named<desc> {
        public:
            ptr Add(const std::string& name, storage_type);
        };

    public:
        using ptr = manager_t::ptr;
        using storage_type = manager_t::storage_type;


        static manager_t manager;


        ~desc() = default;
        const std::string& GetName() const { return name; }
        const std::string& GetDescription() const { return description; }
        const std::string& GetType() const { return type; }
        texture::ptr GetTexture() { return mTexture; }
        int GetCost() const { return cost; }
        int GetMinLevel() const { return minLevel; }
        const std::string& GetPath() const { return path; }

        /// @return true if load successful, false otherwise
        virtual bool Load(const std::string &filename)
        {
            path = filename;
            return false;
        }


        virtual void OnBuild(player_info* pInfo)
        {
            pInfo->AddXP(experience);
            pInfo->AddGold((float)-cost);
        }


        virtual void OnDemolish(player_info*) {}
        virtual void Update(player_info*, int ticksPassed) {}

    protected:
        std::string name, description;
        std::string type;  /// Used for id on higher level
        std::string path;
        int cost = 0, minLevel = 0;
        float experience = 0.f;  /// Gained on construction
        texture::ptr mTexture;


        desc(const std::string& type = "invalid");
    };


    map_building(desc::ptr, const camera*, player_info*, const timer*);
    virtual ~map_building();

    desc::ptr GetDesc() { return mDesc; }

    void SetTopLeft(const coord_opengl& newTopLeft) { topLeft = newTopLeft; }
    void SetSizes(const coord_opengl& newSizes) { sizes = newSizes; }

    void SetCallback(controls*, const std::function<void()>&);

    rect_opengl GetBounds() const;
    rect_opengl GetBoundsDefault() const;

protected:
    static constexpr const char* SHADER_PATH = PATHS_SHADERS "building/";
    static constexpr float UPDATE_PERIOD = 0.25f;

    float prevGenTime;
    desc::storage_type mDesc;
    const camera* pCamera;
    const timer* pTimer;
    controls* pControls = nullptr;
    player_info* playerInfo;
    coord_opengl topLeft, sizes;
    prim::points mPrim;
    ui::clickable::ptr wUI;


    virtual void Update() override;
    virtual void Render() override;

    bool CheckBounds(controls*) const;
    material::ptr CreateMaterial(const std::string& name, texture::ptr);
};
}
