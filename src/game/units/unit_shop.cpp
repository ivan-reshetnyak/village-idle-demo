#include "pch.h"

#include <vector>
#include <string>

#include "paths.h"
#include "unit_billboard.h"
#include "unit_text.h"
#include "unit_button.h"
#include "unit_map_building.h"
#include "unit_map_bld_decoration.h"
#include "unit_map_bld_workshop.h"
#include "unit_shop.h"
#include "render/resource/tex/tex_img.h"
#include "math/common.h"


namespace units {
shop::listener::listener(unit_manager* unitManager, map::ptr wMap,
    camera* pCamera, player_info* playerInfo, timer* pTimer)
    : unitManager(unitManager), wMap(wMap)
    , pCamera(pCamera), playerInfo(playerInfo)
    , pTimer(pTimer)
{
}


bool shop::listener::ProcessMouseEvent(controls* pControls, controls::input_event type)
{
    auto pBB = previewBB.lock();
    if (!pBB) {
        return false;
    }

    switch (type) {
    case controls::input_event::MOUSE_MOVE:
        pBB->Move(pControls->GetMouse().GetDeltaOpenGL());
        return false;
    case controls::input_event::MOUSE_R_DOWN:
        unitManager->Remove(previewBB);
        return true;
    case controls::input_event::MOUSE_L_DOWN:
        if (auto pMap = wMap.lock()) {
            for (auto& pGrid : pMap->mData.grids) {
                if (pGrid->Has(pControls->GetMouse().GetPosRelative())) {
                    auto pBld = std::make_shared<map_building>(previewDesc, pCamera, playerInfo, pTimer);
                    pGrid->BuildBuilding(pControls->GetMouse().GetPosRelative(), pBld, unitManager,
                        pControls, pCamera, playerInfo);
                }
            }
        }
        unitManager->Remove(previewBB);
        return true;
    }
    return false;
}


class shop_item : public unit {
public:
    using ptr = std::weak_ptr<shop_item>;


    shop_item(const std::string& name, const rect_opengl& bounds,
              controls* pControls, shop::listener::ptr wListener,
              unit* pShop)
        : bounds(bounds), name(name)
        , pControls(pControls), wListener(wListener)
        , pShop(pShop)
    {
    }


    void OnAdd(unit_manager* unitManager) override
    {
        rect_opengl imgBorders = rect_opengl((coord_opengl)bounds.TopLeft(),
                (coord_opengl)(bounds.TopLeft() + bounds.GetSize() * coord(1.f, 0.8f)));
        auto pImage = std::make_shared<billboard_clickable>(name + "_SHOWCASE",
            imgBorders);
        unitManager->Add(GetLayer(), pImage);
        AddChild(pImage);
        pImage->SetCallbacks(pControls, [this, unitManager, imgBorders](){
                auto pPreview = std::make_shared<billboard>(name + "_PREVIEW", imgBorders,
                    wDesc.lock()->GetTexture(),
                    color(1.f, 0, 0, 0.4f));
                unitManager->Add(GetLayer() + 3, pPreview);
                pShop->SetVisible(false);

                if (auto pListener = wListener.lock()) {
                    pListener->Update(pPreview, wDesc);
                }
            });
        wImage = pImage;

        auto pGoldTex = std::make_shared<tex::img>();
        pGoldTex->Load(COIN_TEX_PATH);
        auto pGold = std::make_shared<billboard>(name + "_COIN",
            rect_opengl((coord_opengl)(bounds.TopLeft() + bounds.GetSize() * coord(0.f, 0.8f)),
                (coord_opengl)(bounds.TopLeft() + bounds.GetSize() * coord(0.2f, 1.f))),
            texture::manager.Add(COIN_TEX_PATH, pGoldTex));
        unitManager->Add(GetLayer(), pGold);
        AddChild(pGold);

        auto pCost = std::make_shared<text>(
            rect_opengl((coord_opengl)(bounds.TopLeft() + bounds.GetSize() * coord(0.2f, 0.8f)),
                (coord_opengl)(bounds.TopLeft() + bounds.GetSize() * coord(0.25f, 1.f))),
            text::FONT_SPLEEN, "");
        unitManager->Add(GetLayer(), pCost);
        AddChild(pCost);
        wCost = pCost;
    }


    void Update(map_building::desc::ptr newDesc)
    {
        auto pDesc = newDesc.lock();
        if (!pDesc) {
            SetVisible(false);
            return;
        } else {
            wImage.lock()->Update(pDesc->GetTexture());
            wCost.lock()->Update(std::to_string(pDesc->GetCost()));
            wDesc = newDesc;
        }
    }

private:
    static constexpr const char* COIN_TEX_PATH = PATHS_IMG "coin.png";


    std::string name;
    rect_opengl bounds;
    billboard_clickable::ptr wImage;
    text::ptr wCost;
    controls* pControls;
    shop::listener::ptr wListener;
    map_building::desc::ptr wDesc;
    unit* pShop;
};


class shop_tab : public unit {
public:
    using ptr = std::weak_ptr<shop_tab>;


    shop_tab(const std::string& name,
        window::info winInfo, controls* pControls,
        shop::listener::ptr wListener,
        const rect_opengl& bounds, float spacing,
        const point& pageSizes,
        const std::vector<map_building::desc::ptr> bldDescs,
        unit* pShop)
        : name(name), bounds(bounds), spacing(spacing)
        , bldDescs(bldDescs), pageSizes(pageSizes)
        , winInfo(winInfo), pControls(pControls)
        , wListener(wListener), pShop(pShop)
    {
    }


    void OnAdd(unit_manager* unitManager) override
    {
        coord cellSizes((bounds.right - bounds.left) / pageSizes.x, (bounds.bottom - bounds.top) / pageSizes.y);

        for (int y = 0; y < pageSizes.y; ++y) {
            for (int x = 0; x < pageSizes.x; ++x) {
                coord_opengl cellTopLeft = (coord_opengl)(bounds.TopLeft()
                    + cellSizes * spacing
                    + coord(cellSizes.x * x, cellSizes.y * y));

                auto pItem = std::make_shared<shop_item>(name + "_SHOP_TAB_" + std::to_string(x) + "_" + std::to_string(y),
                    rect_opengl(cellTopLeft, cellTopLeft + (coord_opengl)(cellSizes * (1.f - spacing))),
                    pControls, wListener, pShop);
                unitManager->Add(GetLayer(), pItem);
                AddChild(pItem);
                items.push_back(pItem);
            }
        }

        auto pNextButtonTexture = std::make_shared<tex::img>();
        pNextButtonTexture->Load(NEXT_BUTTON_TEX);
        auto pNextButton = std::make_shared<button>(name + "_NEXT_BUTTON",
            coord_opengl(bounds.TopLeft() + bounds.GetSize() * coord(1.f, 0.5f)), 32, 32,
            texture::manager.Add(NEXT_BUTTON_TEX, pNextButtonTexture),
            winInfo, pControls, [this](){ NextPage(); });
        unitManager->Add(GetLayer() + 1, pNextButton);
        AddChild(pNextButton);

        auto pPrevButtonTexture = std::make_shared<tex::img>();
        pPrevButtonTexture->Load(PREV_BUTTON_TEX);
        auto pPrevButton = std::make_shared<button>(name + "_PREV_BUTTON",
            coord_opengl(bounds.TopLeft() + bounds.GetSize() * coord(0.f, 0.5f)), 32, 32,
            texture::manager.Add(PREV_BUTTON_TEX, pPrevButtonTexture),
            winInfo, pControls, [this](){ PrevPage(); });
        unitManager->Add(GetLayer() + 1, pPrevButton);
        AddChild(pPrevButton);
    }


    void SetVisible(bool newVisibility = true) override
    {
        if (!newVisibility) {
            unit::SetVisible(false);
        } else {
            unit::SetVisible(true);
            Refresh();
        }
    }


    void NextPage()
    {
        currentPage = ++currentPage % (int)std::ceil((float)bldDescs.size() / (pageSizes.x * pageSizes.y));
        Refresh();
    }


    void PrevPage()
    {
        int totalPages = (int)std::ceil((float)bldDescs.size() / (pageSizes.x * pageSizes.y));
        currentPage = (--currentPage + totalPages) % totalPages;
        Refresh();
    }


private:
    static constexpr const char* PREV_BUTTON_TEX = PATHS_IMG "previous.png";
    static constexpr const char* NEXT_BUTTON_TEX = PATHS_IMG "next.png";


    float spacing;
    int currentPage = 0;
    window::info winInfo;
    controls* pControls;
    point pageSizes;
    rect_opengl bounds;
    std::string name;
    std::vector<map_building::desc::ptr> bldDescs;
    std::vector<shop_item::ptr> items;
    shop::listener::ptr wListener;
    unit* pShop;


    void Refresh()
    {
        if (items.size() <= 0) {
            return;
        }

        int firstDescN = currentPage * pageSizes.x * pageSizes.y;

        for (int y = 0; y < pageSizes.y; ++y) {
            for (int x = 0; x < pageSizes.x; ++x) {
                int itemN = y * pageSizes.x + x;
                if (auto pItem = items[itemN].lock()) {
                    if (firstDescN + itemN >= bldDescs.size()) {
                        pItem->SetVisible(false);
                    } else {
                        pItem->SetVisible(true);
                        pItem->Update(bldDescs[firstDescN + itemN]);
                    }
                }
            }
        }
    }
};


class shop_interface : public unit {
public:
    shop_interface(const rect_opengl& bounds, window::info winInfo,
        controls* pControls, shop::listener::ptr wListener, timer* pTimer)
        : bounds(bounds), winInfo(winInfo)
        , pControls(pControls), wListener(wListener)
        , pTimer(pTimer)
    {
    }


    ~shop_interface() = default;


    void SetVisible(bool newVisibility = true) override
    {
        shouldShow = newVisibility;
        stateChangeTime = pTimer->globalTime;
    }

    void Update() override
    {
        if (shouldShow == IsVisible()) {
            return;
        }

        if (pTimer->globalTime - stateChangeTime >= FADE_TIME) {
            if (shouldShow) {
                wBackground.lock()->Update(color(0.9843f, 0.5333f, 0.2588f, 1));
                unit::SetVisible(true);
                wDecorTab.lock()->SetVisible(false);  // Show only workshops by default
            } else {
                unit::SetVisible(false);
            }
        } else {
            float alpha = std::min(std::max(0.f, (pTimer->globalTime - stateChangeTime) / FADE_TIME), 1.f);
            if (!shouldShow) {
                alpha = 1.f - alpha;
            }
            wBackground.lock()->SetVisible(true);
            wBackground.lock()->Update(color(0.9843f, 0.5333f, 0.2588f, alpha));
        }
    }


    void OnAdd(unit_manager* unitManager) override
    {
        auto pBackground = std::make_shared<billboard_clickable>(
            "SHOP_BACKGROUND_MATERIAL",
            bounds, color(0.9843f, 0.5333f, 0.2588f));
        unitManager->Add(GetLayer(), pBackground);
        AddChild(pBackground);
        pBackground->SetCallbacks(pControls, [](){});  // Consume clicks and do nothing
        wBackground = pBackground;

        auto pCloseTex = std::make_shared<tex::img>();
        pCloseTex->Load(CLOSE_BUTTON_TEX);
        auto pCloseButton = std::make_shared<button>("SHOP_CLOSE_BUTTON",
            coord_opengl(bounds.TopRight() - bounds.GetSize() * coord(0.02f, 0)), 32, 32,
            texture::manager.Add(CLOSE_BUTTON_TEX, pCloseTex),
            winInfo, pControls,
            [this](){ SetVisible(false); });
        unitManager->Add(GetLayer() + 2, pCloseButton);
        AddChild(pCloseButton);

        std::vector<map_building::desc::ptr> workshopDescs, decorDescs;
        for (auto& desc : map_building::desc::manager.Get()) {
            if (desc.second->GetType() == map_bld::workshop::TYPE) {
                workshopDescs.push_back(desc.second);
            } else if (desc.second->GetType() == map_bld::decoration::TYPE) {
                decorDescs.push_back(desc.second);
            }
        }

        auto pWorkshopTab = std::make_shared<shop_tab>("Workshops",
            winInfo, pControls, wListener,
            rect_opengl(bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * 0.1f),
                        bounds.BottomRight() - (coord_opengl)(bounds.GetSize() * 0.1f)),
            0.05f, point(2, 2), workshopDescs, this);
        unitManager->Add(GetLayer() + 1, pWorkshopTab);
        AddChild(pWorkshopTab);
        wWorkshopTab = pWorkshopTab;

        auto pDecorTab = std::make_shared<shop_tab>("Decorations",
            winInfo, pControls, wListener,
            rect_opengl(bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * 0.1f),
                        bounds.BottomRight() - (coord_opengl)(bounds.GetSize() * 0.1f)),
            0.05f, point(2, 2), decorDescs, this);
        unitManager->Add(GetLayer() + 1, pDecorTab);
        AddChild(pDecorTab);
        wDecorTab = pDecorTab;

        auto pWorkshopTabButton = std::make_shared<billboard_clickable>("pWorkshopTabButton",
            rect_opengl(bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * coord(0, 0)),
                        bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * coord(0.5f, 0.1f))),
            color(0.2f, 0.2f, 0.2f, 0.2f));
        unitManager->Add(GetLayer() + 1, pWorkshopTabButton);
        pWorkshopTabButton->SetCallbacks(pControls, [this](){
                if (auto pWorkshopTab = wWorkshopTab.lock()) {
                    pWorkshopTab->SetVisible(true);
                }
                if (auto pDecorTab = wDecorTab.lock()) {
                    pDecorTab->SetVisible(false);
                }
            });
        AddChild(pWorkshopTabButton);

        auto pWorkshopTabButtonText = std::make_shared<text>(
            rect_opengl(bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * coord(0, 0)),
                        bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * coord(0.04f, 0.1f))),
            text::FONT_SPLEEN, "Workshops");
        unitManager->Add(GetLayer() + 2, pWorkshopTabButtonText);
        AddChild(pWorkshopTabButtonText);

        auto pDecorTabButton = std::make_shared<billboard_clickable>("pDecorTabButton",
            rect_opengl(bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * coord(0.5f, 0)),
                        bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * coord(1.f, 0.1f))),
            color(0.2f, 0.2f, 0.2f, 0.2f));
        unitManager->Add(GetLayer() + 1, pDecorTabButton);
        pDecorTabButton->SetCallbacks(pControls, [this](){
                if (auto pWorkshopTab = wWorkshopTab.lock()) {
                    pWorkshopTab->SetVisible(false);
                }
                if (auto pDecorTab = wDecorTab.lock()) {
                    pDecorTab->SetVisible(true);
                }
            });
        AddChild(pDecorTabButton);

        auto pDecorTabButtonText = std::make_shared<text>(
            rect_opengl(bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * coord(0.5f, 0)),
                        bounds.TopLeft() + (coord_opengl)(bounds.GetSize() * coord(0.54f, 0.1f))),
            text::FONT_SPLEEN, "Decorations");
        unitManager->Add(GetLayer() + 2, pDecorTabButtonText);
        AddChild(pDecorTabButtonText);
    }

private:
    static constexpr const char* CLOSE_BUTTON_TEX = PATHS_IMG "close.png";
    static constexpr float FADE_TIME = 0.25f;


    rect_opengl bounds;
    window::info winInfo;
    controls* pControls;
    shop::listener::ptr wListener;
    shop_tab::ptr wWorkshopTab, wDecorTab;
    float stateChangeTime = -FADE_TIME;
    bool shouldShow = false;
    billboard::ptr wBackground;
    timer* pTimer;
};


shop::shop(const rect_opengl& bounds, controls* pControls,
           window::info winInfo, timer* pTimer,
           camera* pCamera, map::ptr wMap,
           player_info* playerInfo)
    : bounds(bounds), pControls(pControls)
    , winInfo(winInfo), pTimer(pTimer)
    , wMap(wMap), pCamera(pCamera)
    , playerInfo(playerInfo)
{
}


shop::~shop()
{
    pControls->Remove(wListener);
}


void shop::OnAdd(unit_manager* unitManager)
{
    auto pListener = std::make_shared<listener>(unitManager, wMap, pCamera, playerInfo, pTimer);
    pControls->Add(GetLayer() + 5, pListener);
    wListener = pListener;

    auto pEntranceTex = std::make_shared<tex::img>();
    pEntranceTex->Load(ENTRANCE_TEX_PATH);
    auto pEntrance = std::make_shared<billboard_clickable>("SHOP_ENTRANCE_MATERIAL",
        rect_opengl(0.6f, -0.6f, 1.f, -1.f),
        texture::manager.Add(ENTRANCE_TEX_PATH, pEntranceTex));
    unitManager->Add(GetLayer(), pEntrance);
    AddChild(pEntrance);

    auto pInterface = std::make_shared<shop_interface>(bounds, winInfo, pControls, wListener, pTimer);
    unitManager->Add(GetLayer() + 1, pInterface);
    AddChild(pInterface);
    pInterface->SetVisible(false);

    unit::ptr wInterface = pInterface;
    pEntrance->SetCallbacks(pControls,
        [wInterface](){
            if (auto pInterface = wInterface.lock()) {
                pInterface->SetVisible(true);
            }
        });
}
}
