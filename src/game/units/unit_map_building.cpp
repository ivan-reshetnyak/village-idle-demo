#include "pch.h"

#include "unit_map_building.h"


namespace units {
map_building::desc::manager_t map_building::desc::manager;


map_building::desc::ptr map_building::desc::manager_t::Add(const std::string& name, storage_type pDesc)
{
    if (!Exists(name)) {
        storage[name] = pDesc;
    }
    return storage[name];
}


map_building::desc::desc(const std::string& type)
    : type(type)
{
}


material::ptr map_building::CreateMaterial(const std::string& name, texture::ptr wTex)
{
    bool needInit = !(material::manager.Exists(name + "_BUILDING_MATERIAL"));
    auto wMat = material::manager.Get(name + "_BUILDING_MATERIAL");

    if (needInit) {
        wMat.lock()->Set(shader::manager.Get(SHADER_PATH));
        wMat.lock()->Add("Texture", wTex);
    }
    return wMat;
}


map_building::map_building(desc::ptr wDesc, const camera* pCamera,
                           player_info* playerInfo, const timer* pTimer)
    : mDesc(wDesc), pCamera(pCamera)
    , playerInfo(playerInfo), pTimer(pTimer)
{
    auto pDesc = wDesc.lock();
    if (!pDesc) {
        return;
    }
    prevGenTime = pTimer->time;

    mPrim.Set(std::make_shared<geometry>(1, new vertex[1]{vertex()}, 1, new int[1]{0}));
    mPrim.Set(CreateMaterial(wDesc.lock()->GetName(), wDesc.lock()->GetTexture()));
}


map_building::~map_building()
{
    if (pControls) {
        pControls->Remove(wUI);
    }
}


void map_building::Update()
{
    float timePassed = pTimer->time - prevGenTime;
    if (timePassed < UPDATE_PERIOD) {
        return;
    }

    int ticksPassed = (int)(timePassed / UPDATE_PERIOD);
    mDesc->Update(playerInfo, ticksPassed);
    prevGenTime += ticksPassed * UPDATE_PERIOD;
}


void map_building::Render()
{
    auto pMtl = mPrim.GetMaterial().lock();
    if (!pMtl) {
        return;
    }
    pMtl->SetUniform("Position", topLeft * pCamera->GetZoom() + pCamera->GetOffset());
    pMtl->SetUniform("Sizes", sizes * pCamera->GetZoom());
    mPrim.Draw();
}


rect_opengl map_building::GetBounds() const
{
    return rect_opengl(topLeft * pCamera->GetZoom() + pCamera->GetOffset(),
        topLeft * pCamera->GetZoom() + pCamera->GetOffset() + sizes * pCamera->GetZoom());
}


rect_opengl map_building::GetBoundsDefault() const
{
    return rect_opengl(topLeft, topLeft + sizes);
}


bool map_building::CheckBounds(controls* pControls) const
{
    return GetBounds().Has((coord_opengl)pControls->GetMouse().GetPosRelative());
}


void map_building::SetCallback(controls* pControls, const std::function<void()>& callback)
{
    this->pControls = pControls;

    auto pUI = wUI.lock();
    if (pUI) {
        // Clean up previous ui element if it existed
        pControls->Remove(wUI);
    }

    pUI = std::make_shared<ui::clickable>(
        [this](controls* pControls) -> bool { return CheckBounds(pControls); },
        callback);
    wUI = pUI;
    pControls->Add(GetLayer(), pUI);
}
}
