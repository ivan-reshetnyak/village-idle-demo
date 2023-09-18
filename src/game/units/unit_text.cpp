#include "pch.h"

#include "paths.h"
#include "unit_text.h"
#include "render/geometry.h"
#include "render/vertex.h"
#include "render/resource/material.h"
#include "render/resource/shader.h"
#include "render/resource/tex/tex_img.h"


namespace units {
text::font text::FONT_SPLEEN{PATHS_IMG "spleen/font.png", ' ', 95};


text::text(const rect_opengl& firstLetterBound, const font& mFont, const std::string& txt,
           const color& textColor, const color& backgroundColor)
    : firstLetterBound(firstLetterBound)
    , mFont(mFont)
    , textColor(textColor)
    , backgroundColor(backgroundColor)
{
    bool needInitMat = !material::manager.Exists("UNIT_TEXT_" + mFont.texturePath);
    auto wMat = material::manager.Get("UNIT_TEXT_" + mFont.texturePath);
    if (needInitMat) {
        auto pTex = std::make_shared<tex::img>();
        pTex->Load(mFont.texturePath);
        wMat.lock()->Add("Texture", texture::manager.Add("UNIT_TEXT_" + mFont.texturePath, pTex));
        wMat.lock()->Set(shader::manager.Get(SHADER_PATH));
    }
    mPrim.Set(wMat);

    Update(txt);
}


class txt_vertex : public vertex {
public:
    coord pos, tex;

    virtual std::vector<location> GetLayout() const override
    {
        std::vector<location> layout;
        layout.push_back(location({false, 0, 2, GL_FLOAT, sizeof(txt_vertex), OFFSET(txt_vertex, pos)}));
        layout.push_back(location({false, 1, 2, GL_FLOAT, sizeof(txt_vertex), OFFSET(txt_vertex, tex)}));
        return layout;
    }

    virtual int GetSize() const override { return sizeof(txt_vertex); }
};


void text::Update(const std::string& txt)
{
    if (txt.size() <= 0) {
        isInitialized = false;
        return;
    }

    float stepX = firstLetterBound.right - firstLetterBound.left;
    float stepY = firstLetterBound.bottom - firstLetterBound.top;

    txt_vertex* vertx = new txt_vertex[txt.size() * 4];
    int* indx = new int[txt.size() * 6];
    for (int letterNum = 0; letterNum < txt.size(); ++letterNum) {
        /* Top left */
        vertx[letterNum * 4 + 0].pos.x = firstLetterBound.left + stepX * letterNum;
        vertx[letterNum * 4 + 0].pos.y = firstLetterBound.top;
        vertx[letterNum * 4 + 0].tex.x = 1.f / mFont.numRenderableChars * (txt[letterNum] - mFont.firstRenderableChar);
        vertx[letterNum * 4 + 0].tex.y = 0.f;
        indx[letterNum * 6 + 0] = letterNum * 4 + 0;

        /* Bottom left */
        vertx[letterNum * 4 + 1].pos.x = firstLetterBound.left + stepX * letterNum;
        vertx[letterNum * 4 + 1].pos.y = firstLetterBound.top + stepY;
        vertx[letterNum * 4 + 1].tex.x = 1.f / mFont.numRenderableChars * (txt[letterNum] - mFont.firstRenderableChar);
        vertx[letterNum * 4 + 1].tex.y = 1.f;
        indx[letterNum * 6 + 1] = letterNum * 4 + 1;

        /* Top Right */
        vertx[letterNum * 4 + 2].pos.x = firstLetterBound.left + stepX * (letterNum + 1);
        vertx[letterNum * 4 + 2].pos.y = firstLetterBound.top;
        vertx[letterNum * 4 + 2].tex.x = 1.f / mFont.numRenderableChars * (txt[letterNum] - mFont.firstRenderableChar + 1);
        vertx[letterNum * 4 + 2].tex.y = 0.f;
        indx[letterNum * 6 + 2] = letterNum * 4 + 2;

        /* Bottom right */
        vertx[letterNum * 4 + 3].pos.x = firstLetterBound.left + stepX * (letterNum + 1);
        vertx[letterNum * 4 + 3].pos.y = firstLetterBound.top + stepY;
        vertx[letterNum * 4 + 3].tex.x = 1.f / mFont.numRenderableChars * (txt[letterNum] - mFont.firstRenderableChar + 1);
        vertx[letterNum * 4 + 3].tex.y = 1.f;
        indx[letterNum * 6 + 3] = letterNum * 4 + 3;

        /* Top Right */
        indx[letterNum * 6 + 4] = letterNum * 4 + 2;
        /* Bottom left */
        indx[letterNum * 6 + 5] = letterNum * 4 + 1;
    }
    mPrim.Set(std::make_shared<geometry>((int)txt.size() * 4, vertx, (int)txt.size() * 6, indx));
    isInitialized = true;
}


void text::Render()
{
    if (!isInitialized) {
        return;
    }

    auto pMat = mPrim.GetMaterial().lock();
    if (pMat) {
        pMat->SetUniform("TextColor", textColor);
        pMat->SetUniform("BackgroundColor", backgroundColor);
    }
    mPrim.Draw();
}
}
