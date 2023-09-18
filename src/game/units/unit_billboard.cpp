#include "pch.h"

#include "unit_billboard.h"
#include "render/resource/tex/tex_img.h"


namespace units {
std::vector<vertex::location> billboard::vertex::GetLayout() const
{
    std::vector<location> layout;
    layout.push_back(location({false, 0, 2, GL_FLOAT, sizeof(vertex), OFFSET(vertex, topLeft)}));
    layout.push_back(location({false, 1, 2, GL_FLOAT, sizeof(vertex), OFFSET(vertex, bottomRight)}));
    return layout;
}


billboard::billboard(const std::string& mtlName, const rect_opengl& bounds,
                     texture::ptr mTexture, const color& mColor)
    : mColor(mColor)
{
    bool needInit = !material::manager.Exists(mtlName);
    auto pMtl = material::manager.Get(mtlName).lock();
    if (needInit) {
        pMtl->Add("Texture", mTexture);
        pMtl->Set(shader::manager.Get(SHADER_PATH));
    }
    CreatePrim(bounds.TopLeft(), bounds.BottomRight(), pMtl);
}


billboard::billboard(const std::string& mtlName, const rect_opengl& bounds,
                     const color& mColor)
    : mColor(mColor)
{
    bool needInit = !material::manager.Exists(mtlName);
    auto pMtl = material::manager.Get(mtlName).lock();
    if (needInit) {
        bool needLoad = !texture::manager.Exists(DEFAULT_TEXTURE);
        if (needLoad) {
            auto pTex = std::make_shared<tex::img>();
            pTex->Load(DEFAULT_TEXTURE);
            pMtl->Add("Texture", texture::manager.Add(DEFAULT_TEXTURE, pTex));
        } else {
            pMtl->Add("Texture", texture::manager.Get(DEFAULT_TEXTURE));
        }
        
        pMtl->Set(shader::manager.Get(SHADER_PATH));
    }
    CreatePrim(bounds.TopLeft(), bounds.BottomRight(), pMtl);
}


void billboard::CreatePrim(const coord& topLeft, const coord& bottomRight, material::ptr wMtl)
{
    mPrim.Set(wMtl);

    auto vertx = new vertex[1];
    vertx[0].bottomRight = (coord_opengl)bottomRight;
    vertx[0].topLeft = (coord_opengl)topLeft;
    mPrim.Set(std::make_shared<geometry>(1, vertx, 1, new int[1]{0}));
}


void billboard::Move(const coord_opengl& delta)
{
    offset += delta;
}


void billboard::Update(texture::ptr newTexture)
{
    if (auto pMtl = mPrim.GetMaterial().lock()) {
        pMtl->Add("Texture", newTexture);
    }
}


void billboard::Render()
{
    if (auto pMtl = mPrim.GetMaterial().lock()) {
        pMtl->SetUniform("Color", mColor);
        if (pCamera) {
            pMtl->SetUniform("Offset", pCamera->GetOffset() + offset);
            pMtl->SetUniform("Scale", pCamera->GetZoom());
        } else {
            pMtl->SetUniform("Offset", offset);
            pMtl->SetUniform("Scale", 1.f);
        }
    }
    mPrim.Draw();
}


billboard_clickable::billboard_clickable(
    const std::string& mtlName, const rect_opengl& bounds,
    texture::ptr mTexture, const color& mColor)
    : billboard(mtlName, bounds, mTexture, mColor)
    , bounds(bounds)
{
}


billboard_clickable::billboard_clickable(
    const std::string& mtlName, const rect_opengl& bounds,
    const color& mColor)
    : billboard(mtlName, bounds, mColor)
    , bounds(bounds)
{
}


billboard_clickable::~billboard_clickable()
{
    if (pControls) {
        pControls->Remove(wUI);
    }
}


void billboard_clickable::SetVisible(bool newVisibility)
{
    if (auto pUI = wUI.lock()) {
        pUI->SetActive(newVisibility);
    }
    billboard::SetVisible(newVisibility);
}


void billboard_clickable::SetCallbacks(controls* pControls,
                                       const std::function<void()>& callbackLeft,
                                       const std::function<void()>& callbackRight)
{
    this->pControls = pControls;

    auto pUI = wUI.lock();
    if (pUI) {
        // Clean up previous ui element if it existed
        pControls->Remove(wUI);
    }

    pUI = std::make_shared<ui::clickable>(
        [this](controls* pControls) -> bool { return CheckBounds(pControls); },
        callbackLeft, callbackRight);
    wUI = pUI;
    pControls->Add(GetLayer(), pUI);
}


bool billboard_clickable::CheckBounds(controls* pControls) const
{
    if (pCamera) {
        rect_opengl boundsShifted(
            bounds.TopLeft() * pCamera->GetZoom() + pCamera->GetOffset(),
            bounds.BottomRight() * pCamera->GetZoom() + pCamera->GetOffset());
        return boundsShifted.Has((coord_opengl)pControls->GetMouse().GetPosRelative());
    } else {
        return bounds.Has((coord_opengl)pControls->GetMouse().GetPosRelative());
    }
}
}
