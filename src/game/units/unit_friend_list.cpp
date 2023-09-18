#include "pch.h"

#include "unit_friend_list.h"
#include "unit_button.h"
#include "render/resource/tex/tex_img.h"


namespace units {
friend_profile::friend_profile(const friends::profile* pProfile, const std::string& mtlNamePrefix, const rect_opengl& bounds)
    : bounds(bounds), prefix(mtlNamePrefix), pProfile(pProfile)
{
}


void friend_profile::OnAdd(unit_manager* unitManager)
{
    coord size = bounds.GetSize();

    auto pPortrait = std::make_shared<billboard>(prefix + "_PORTRAIT",
        rect_opengl(bounds.TopLeft(), coord(bounds.right, bounds.top + size.y * PORTRAIT_HEIGHT_PERCENTAGE)));
    wPortrait = pPortrait;
    unitManager->Add(GetLayer(), pPortrait);
    AddChild(pPortrait);

    auto pNameText = std::make_shared<text>(
        rect_opengl(bounds.left, bounds.top + size.y * PORTRAIT_HEIGHT_PERCENTAGE,
                    bounds.left + size.x * LETTER_WIDTH_PERCENTAGE, bounds.top + size.y * (0.5f * PORTRAIT_HEIGHT_PERCENTAGE + 0.5f)),
        text::FONT_SPLEEN, "");
    wNameText = pNameText;
    unitManager->Add(GetLayer(), pNameText);
    AddChild(pNameText);

    auto pLevelText = std::make_shared<text>(
        rect_opengl(bounds.left, bounds.top + size.y * (0.5f * PORTRAIT_HEIGHT_PERCENTAGE + 0.5f),
                    bounds.left + size.x * LETTER_WIDTH_PERCENTAGE, bounds.top + size.y),
        text::FONT_SPLEEN, "");
    wLevelText = pLevelText;
    unitManager->Add(GetLayer(), pLevelText);
    AddChild(pLevelText);

    Update(pProfile);
}


void friend_profile::Update(const friends::profile* pProfile)
{
    this->pProfile = pProfile;
    if (pProfile == nullptr) {
        SetVisible(false);
        return;
    }
    SetVisible(true);
    wPortrait.lock()->Update(pProfile->texture);
    wNameText.lock()->Update(pProfile->name);
    wLevelText.lock()->Update("lvl " + std::to_string(pProfile->level));
}


void friend_profile::SetVisible(bool newVisibility)
{
    unit::SetVisible(newVisibility);
    if (auto pPortrait = wPortrait.lock()) {
        pPortrait->SetVisible(newVisibility);
    }
    if (auto pName = wNameText.lock()) {
        pName->SetVisible(newVisibility);
    }
    if (auto pLevel = wLevelText.lock()) {
        pLevel->SetVisible(newVisibility);
    }
}


friend_list::friend_list(friends* pFriends,
                         const rect_opengl& backgroundBounds, const color& backgroundColor,
                         const rect_opengl& firstFriendBounds, float spacing, int friendsPerPage,
                         const window::info& winInfo, controls* pControls,
                         const coord_opengl& leftButtonAnchor, const coord_opengl& rightButtonAnchor,
                         int buttonSizePx)
    : friendsPerPage(friendsPerPage), pFriends(pFriends)
    , firstFriendBounds(firstFriendBounds), spacing(spacing)
    , backgroundBounds(backgroundBounds), backgroundColor(backgroundColor)
    , buttonSizePx(buttonSizePx)
    , leftButtonAnchor(leftButtonAnchor), rightButtonAnchor(rightButtonAnchor)
    , winInfo(winInfo), pControls(pControls)
{
}


void friend_list::OnAdd(unit_manager* unitManager)
{
    /* Create background */
    auto pBackground = std::make_shared<billboard>("FRIEND_LIST_BACKGROUND", backgroundBounds, backgroundColor);
    unitManager->Add(GetLayer(), pBackground);

    /* Create profile displays */
    float friendWidth = firstFriendBounds.right - firstFriendBounds.left;
    for (int i = 0; i < friendsPerPage; ++i) {
        rect_opengl bound = firstFriendBounds;
        bound.left += friendWidth * i + spacing * (i - 1);
        bound.right += friendWidth * i + spacing * (i - 1);

        auto pFriend = pFriends->Get().size() > i ? &pFriends->Get()[i] : nullptr;

        auto pProfile = std::make_shared<friend_profile>(pFriend,
            std::string("FRIEND_LIST_PROFILE_") + std::to_string(i),
            bound);
        profiles.push_back(pProfile);
        unitManager->Add(GetLayer() + 1, pProfile);
        AddChild(pProfile);
    }

    /* Create scrolling buttons */
    auto pLeftTex = std::make_shared<tex::img>();
    pLeftTex->Load(LEFT_BUTTON_TEXTURE_PATH);
    auto pButtonLeft = std::make_shared<button>("FRIEND_LIST_BUTTON_LEFT",
        leftButtonAnchor, buttonSizePx, buttonSizePx, texture::manager.Add(LEFT_BUTTON_TEXTURE_PATH, pLeftTex),
        winInfo, pControls, [this](){ PrevPage(); });
    unitManager->Add(GetLayer() + 2, pButtonLeft);
    AddChild(pButtonLeft);

    auto pRightTex = std::make_shared<tex::img>();
    pRightTex->Load(RIGHT_BUTTON_TEXTURE_PATH);
    auto pButtonRight = std::make_shared<button>("FRIEND_LIST_BUTTON_RIGHT",
        rightButtonAnchor, buttonSizePx, buttonSizePx, texture::manager.Add(RIGHT_BUTTON_TEXTURE_PATH, pRightTex),
        winInfo, pControls, [this](){ NextPage(); });
    unitManager->Add(GetLayer() + 2, pButtonRight);
    AddChild(pButtonRight);
}


void friend_list::NextPage()
{
    currentPage = ++currentPage % (int)std::ceil((float)pFriends->Get().size() / friendsPerPage);
    UpdateProfiles();
}


void friend_list::PrevPage()
{
    int totalPages = (int)std::ceil((float)pFriends->Get().size() / friendsPerPage);
    currentPage = (--currentPage + totalPages) % totalPages;
    UpdateProfiles();
}


void friend_list::UpdateProfiles()
{
    for (int i = 0; i < friendsPerPage; ++i) {
        auto pFriend = pFriends->Get().size() > (currentPage * friendsPerPage + i) ?
            &pFriends->Get()[currentPage * friendsPerPage + i] : nullptr;
        profiles[i].lock()->Update(pFriend);
    }
}
}
