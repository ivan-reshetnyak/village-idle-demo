#include "pch.h"

#include <cmath>

#include "unit_button.h"
#include "render/geometry.h"
#include "render/vertex.h"
#include "math/common.h"


namespace units {
class button_vertex : public vertex {
public:
    coord v;


    button_vertex(const coord& v)
        : v(v)
    {
    }


    std::vector<location> GetLayout() const override
    {
        std::vector<location> layout;
        layout.push_back(location({false, 0, 2, GL_FLOAT, GetSize(), OFFSET(button_vertex, v)}));
        return layout;
    }


    int GetSize() const override { return sizeof(button_vertex); }
};


button::button(const std::string mtlName, const coord_opengl& anchor,
               int width, int height, texture::ptr tex, const window::info& info,
               controls* ctrls,  const std::function<void()>& callback)
    : anchor(anchor), width(width), height(height)
    , winInfo(info), pControls(ctrls)
    , callback(callback)
{
    /* Init primitive */
    wMtl = material::manager.Get(mtlName);
    primPoint.Set(wMtl);
    auto pMtl = wMtl.lock();

    pMtl->Set(shader::manager.Get(SHADER_PATH));

    tex.lock()->SetSampler(0);
    pMtl->Add("ButtonTexture", tex);

    primPoint.Set(std::make_shared<geometry>(1,
        new button_vertex[1]{button_vertex{coord(anchor.x, anchor.y)}},
        1, new int[1]{0}));

    /* Remap anchor from GL coord system to WINAPI coord system */
    anchorWin = (coord_winapi)anchor;
}


button::~button()
{
    pControls->Remove(listener);
}


void button::SetVisible(bool newVisibility)
{
    if (auto pListener = listener.lock()) {
        pListener->SetActive(newVisibility);
    }
    unit::SetVisible(newVisibility);
}


void button::OnAdd(unit_manager*)
{
    /* Init controls element */
    auto pListener = std::make_shared<ui::clickable>(
        [this](controls* ctrls) -> bool { return CheckBounds(ctrls); },
        callback);
    listener = pListener;
    pControls->Add(GetLayer(), pListener);
}


bool button::CheckBounds(controls* ctrls) const
{
    point anchorClient = winInfo.RelativeToClient(anchorWin);
    point mouseClient = ctrls->GetMouse().GetPos();

    return std::abs(anchorClient.x - mouseClient.x) <= width / 2 &&
        std::abs(anchorClient.y - mouseClient.y) <= height / 2;
}


void button::Update()
{
    if (auto pMtl = wMtl.lock()) {
        point anchorClient = winInfo.RelativeToClient(anchorWin);
        point topRightClient = anchorClient + point(width / 2, -height / 2);
        coord_opengl topRightRel = winInfo.ClientToRelative(topRightClient);

        pMtl->SetUniform("Sizes", topRightRel - anchor);
    }
}


void button::Render()
{
    primPoint.Draw();
}
}
