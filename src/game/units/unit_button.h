#pragma once

#include <memory>

#include "paths.h"
#include "game/controls.h"
#include "game/unit.h"
#include "game/ui/ui_clickable.h"
#include "win/window.h"
#include "render/resource/texture.h"
#include "render/resource/material.h"
#include "render/prim/points.h"
#include "math/common.h"


namespace units {
class button : public unit {
public:
    button(const std::string mtlName, const coord_opengl& anchor,
           int width, int height, texture::ptr, const window::info&,
           controls*, const std::function<void()>& callback);
    virtual ~button();

    virtual void SetVisible(bool newVisibility = true) override;

protected:
    static constexpr const char* SHADER_PATH = PATHS_SHADERS "button/";

    int width, height;
    controls* pControls;
    coord_opengl anchor;
    coord_winapi anchorWin;
    ui::clickable::ptr listener;
    prim::points primPoint;
    material::ptr wMtl;
    window::info winInfo;
    std::function<void()> callback;


    void OnAdd(unit_manager*) override;
    void Update() override;
    void Render() override;
    bool CheckBounds(controls*) const;
};
}
