#pragma once

#include "paths.h"
#include "unit_text.h"
#include "unit_progress_bar.h"
#include "game/unit.h"
#include "game/player_info.h"


namespace units {
class player_stats : public unit {
public:
    player_stats(player_info*);
    ~player_stats() = default;

    void OnAdd(unit_manager*) override final;
    void Update() override final;

private:
    static constexpr const char* SMILEY_TEXTURE_PATH = PATHS_IMG "Smiley.png";
    static constexpr const char* GOLD_TEXTURE_PATH = PATHS_IMG "coin.png";


    player_info* pInfo;
    units::text::ptr
        wPlayerNameText, wPlayerLevelText,
        wPlayerXPText, wPlayerGoldText;
    units::progress_bar::ptr
        wXPBar, wHappyBar;
};
}
