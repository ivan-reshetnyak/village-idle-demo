#include "pch.h"

#include "unit_progress_bar.h"
#include "render/vertex.h"


namespace units {
progress_bar::progress_bar(const rect_opengl& bounds, const color& backgroundColor,
                           const color& emptyColor, const color& fullColor)
{
    milestones.push_back({0.f, emptyColor});
    milestones.push_back({1.f, fullColor});
    CreatePrim(bounds, backgroundColor);
}


progress_bar::progress_bar(const rect_opengl& bounds, const color& backgroundColor,
                           const color& fillColor)
{
    milestones.push_back({1.f, fillColor});
    CreatePrim(bounds, backgroundColor);
}


progress_bar::progress_bar(const rect_opengl& bounds, const color& backgroundColor,
                           const std::vector<std::pair<float, color>>& milestones)
    : milestones(milestones)
{
    CreatePrim(bounds, backgroundColor);
}


color progress_bar::GetColor(float t) const
{
    for (int i = 0; i < milestones.size() - 1; ++i) {
        if (milestones[i].first <= t && t <= milestones[i + 1].first) {
            float mixMul = (t - milestones[i].first) / (milestones[i + 1].first - milestones[i].first);
            return color::mix(milestones[i].second, milestones[i + 1].second, mixMul);
        }
    }
    return milestones.back().second;
}


class progress_bar_vertex : public vertex {
public:
    coord topLeft, bottomRight;
    color backgroundColor;


    std::vector<location> GetLayout() const override
    {
        std::vector<location> layout;
        layout.push_back(location({false, 0, 2, GL_FLOAT, GetSize(), OFFSET(progress_bar_vertex, topLeft)}));
        layout.push_back(location({false, 1, 2, GL_FLOAT, GetSize(), OFFSET(progress_bar_vertex, bottomRight)}));
        layout.push_back(location({false, 2, 4, GL_FLOAT, GetSize(), OFFSET(progress_bar_vertex, backgroundColor)}));
        return layout;
    }


    int GetSize() const override
    {
        return sizeof(progress_bar_vertex);
    }
};


void progress_bar::CreatePrim(const rect_opengl& bounds, const color& backgroundColor)
{
    bool needInitMat = !material::manager.Exists(MTL_NAME);
    auto wMtl = material::manager.Get(MTL_NAME);
    if (needInitMat) {
        wMtl.lock()->Set(shader::manager.Get(SHADER_PATH));
    }
    mPrim.Set(wMtl);

    progress_bar_vertex* vertx = new progress_bar_vertex[1];
    vertx[0].topLeft = bounds.TopLeft();
    vertx[0].bottomRight = bounds.BottomRight();
    vertx[0].backgroundColor = backgroundColor;
    int* indx = new int[1]{0};
    mPrim.Set(std::make_shared<geometry>(1, vertx, 1, indx));
}


void progress_bar::Render()
{
    auto pMat = mPrim.GetMaterial().lock();
    pMat->SetUniform("FillColor", GetColor(progress));
    pMat->SetUniform("Progress", progress);
    mPrim.Draw();
}
}
