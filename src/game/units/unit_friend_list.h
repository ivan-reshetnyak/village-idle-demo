#pragma once

#include <memory>
#include <vector>
#include <string>

#include "paths.h"
#include "unit_billboard.h"
#include "unit_text.h"
#include "game/controls.h"
#include "game/friends.h"
#include "game/unit.h"
#include "render/resource/texture.h"
#include "math/common.h"
#include "win/window.h"


namespace units {
class friend_profile : public unit {
public:
    using ptr = std::weak_ptr<friend_profile>;

    friend_profile(const friends::profile*, const std::string& mtlNamePrefix, const rect_opengl& bounds);

    virtual void SetVisible(bool newVisibility = true) override;
    virtual void OnAdd(unit_manager*) override;

    void Update(const friends::profile*);

private:
    static constexpr const float PORTRAIT_HEIGHT_PERCENTAGE = 0.5f;
    static constexpr const float LETTER_WIDTH_PERCENTAGE = 0.075f;


    const friends::profile* pProfile;
    rect_opengl bounds;
    std::string prefix;
    billboard::ptr wPortrait;
    text::ptr wNameText, wLevelText;
};

class friend_list : public unit {
public:
    friend_list(friends*,
        const rect_opengl& backgroundBounds, const color& backgroundColor,
        const rect_opengl& firstFriendBounds, float spacing, int friendsPerPage,
        const window::info&, controls*,
        const coord_opengl& leftButtonAnchor,
        const coord_opengl& rightButtonAnchor,
        int buttonSizePx);
    virtual ~friend_list() = default;

    virtual void OnAdd(unit_manager*) override;

    void NextPage();
    void PrevPage();
    void UpdateProfiles();

private:
    static constexpr const char* LEFT_BUTTON_TEXTURE_PATH = PATHS_IMG "previous.png";
    static constexpr const char* RIGHT_BUTTON_TEXTURE_PATH = PATHS_IMG "next.png";


    friends* pFriends;
    controls* pControls;
    int friendsPerPage, buttonSizePx, currentPage = 0;
    float spacing;
    color backgroundColor;
    rect_opengl backgroundBounds, firstFriendBounds;
    coord_opengl leftButtonAnchor, rightButtonAnchor;
    window::info winInfo;
    std::vector<friend_profile::ptr> profiles;
};
}
