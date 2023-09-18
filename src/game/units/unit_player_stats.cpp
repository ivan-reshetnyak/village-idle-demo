#include "pch.h"

#include "unit_billboard.h"
#include "unit_player_stats.h"
#include "unit_progress_bar.h"
#include "render/resource/tex/tex_img.h"


namespace units {
player_stats::player_stats(player_info* pInfo)
    : pInfo(pInfo)
{
}


void player_stats::OnAdd(unit_manager* unitManager)
{
    auto pPlayerNameText =
        std::make_shared<text>(rect_opengl(-1.f, 1.f, -0.95f, 0.90f), text::FONT_SPLEEN, pInfo->GetName(),
            color(0.f, 0.f, 0.f, 1.f), color(0.3f, 0.3f, 0.3f, 0.3f));
    wPlayerNameText = pPlayerNameText;
    unitManager->Add(GetLayer() + 1, pPlayerNameText);
    AddChild(pPlayerNameText);

    auto pPlayerLevelText =
        std::make_shared<text>(rect_opengl(-1.f, 0.90f, -0.95f, 0.80f), text::FONT_SPLEEN,
            std::string("Level: ") + std::to_string(pInfo->GetLevel()),
            color(0.f, 0.f, 0.f, 1.f), color(0.3f, 0.3f, 0.3f, 0.3f));
    wPlayerLevelText = pPlayerLevelText;
    unitManager->Add(GetLayer() + 1, pPlayerLevelText);
    AddChild(pPlayerLevelText);

    auto pPlayerXPText =
        std::make_shared<text>(rect_opengl(-1.f, 0.80f, -0.95f, 0.70f), text::FONT_SPLEEN,
            std::string("XP: ") + std::to_string(pInfo->GetXP())
                + "/" + std::to_string(pInfo->GetXPRequired()),
            color(0.f, 0.f, 0.f, 1.f), color(0.3f, 0.3f, 0.3f, 0.3f));
    wPlayerXPText = pPlayerXPText;
    unitManager->Add(GetLayer() + 1, pPlayerXPText);
    AddChild(pPlayerXPText);

    auto pXPBar =
        std::make_shared<progress_bar>(rect_opengl(-1.f, 0.80f, -0.55f, 0.70f),
            color(0, 0, 0), color(0.9843f, 0.5333f, 0.2588f));
    wXPBar = pXPBar;
    unitManager->Add(GetLayer(), pXPBar);
    AddChild(pXPBar);

    auto pGoldTexture = std::make_shared<tex::img>();
    pGoldTexture->Load(GOLD_TEXTURE_PATH);
    auto pCoin = std::make_shared<billboard>("UNIT_PLAYER_STATS_GOLD_COIN",
        rect_opengl(-1.0f, 0.7f, -0.9f, 0.6f), texture::manager.Add(GOLD_TEXTURE_PATH, pGoldTexture));
    unitManager->Add(GetLayer(), pCoin);
    AddChild(pCoin);

    auto pPlayerGoldText =
        std::make_shared<text>(rect_opengl(-0.9f, 0.7f, -0.85f, 0.60f), text::FONT_SPLEEN,
            std::to_string(pInfo->GetGold()),
            color(0.f, 0.f, 0.f, 1.f), color(0.3f, 0.3f, 0.3f, 0.3f));
    wPlayerGoldText = pPlayerGoldText;
    unitManager->Add(GetLayer() + 1, pPlayerGoldText);
    AddChild(pPlayerGoldText);

    auto pHappyBar =
        std::make_shared<progress_bar>(rect_opengl(-0.3f, 1.0f, 0.3f, 0.9f), color(0, 0, 0, 0.5f),
            std::vector<std::pair<float, color>>{
                {0.00f, color(0.9216f, 0.3647f, 0.2196f)}, {0.25f, color(0.9765f, 0.7020f, 0.2902f)},
                {0.50f, color(1.0000f, 0.8862f, 0.4745f)}, {0.75f, color(0.6980f, 0.9451f, 0.4745f)},
                {1.00f, color(0.6313f, 0.8706f, 0.9922f)}});
    wHappyBar = pHappyBar;
    unitManager->Add(GetLayer(), pHappyBar);
    AddChild(pHappyBar);

    auto pSmileyTexture = std::make_shared<tex::img>();
    pSmileyTexture->Load(SMILEY_TEXTURE_PATH);
    auto pSmileyFace = std::make_shared<billboard>("UNIT_PLAYER_STATS_SMILEY",
        rect_opengl(-0.4f, 1.0f, -0.3f, 0.9f), texture::manager.Add(SMILEY_TEXTURE_PATH, pSmileyTexture));
    unitManager->Add(GetLayer(), pSmileyFace);
    AddChild(pSmileyFace);
}


void player_stats::Update()
{
    if (!pInfo->WasUpdated()) {
        return;
    }

    wPlayerNameText.lock()->Update(pInfo->GetName());
    wPlayerLevelText.lock()->Update(std::string("Level: ") + std::to_string(pInfo->GetLevel()));
    wPlayerXPText.lock()->Update(std::string("XP: ") + std::to_string(pInfo->GetXP())
        + "/" + std::to_string(pInfo->GetXPRequired()));
    wXPBar.lock()->SetProgress((float)pInfo->GetXP() / pInfo->GetXPRequired());
    wPlayerGoldText.lock()->Update(std::to_string(pInfo->GetGold()));

    float progress = std::min(std::max(0.f, pInfo->GetHappiness() / 100.f), 1.f);
    wHappyBar.lock()->SetProgress(progress);

    pInfo->ResetUpdated();
}
}
