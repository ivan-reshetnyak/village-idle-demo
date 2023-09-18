#include "pch.h"

#include "unit_map_grid.h"
#include "unit_billboard.h"
#include "render/resource/tex/tex_img.h"


namespace units {
map_grid::map_grid(const camera* pCamera)
    : pCamera(pCamera)
{
    bool needInitMtl = !material::manager.Exists(MTL_NAME);
    auto wMtl = material::manager.Get(MTL_NAME);
    auto pMtl = wMtl.lock();
    if (needInitMtl) {
        pMtl->Set(shader::manager.Get(SHADER_PATH));
        auto pTex = std::make_shared<tex::img>();
        pTex->Load(GRID_TEX_PATH);
        pMtl->Add("Texture", texture::manager.Add(GRID_TEX_PATH, pTex));
    }
    gridPrim.Set(wMtl);

    gridPrim.Set(std::make_shared<geometry>(1, new vertex[1]{vertex()}, 1, new int[1]{0}));

    bool needInitClosePromptTex = !texture::manager.Exists(PROMPT_TEX_PATH);
    if (needInitClosePromptTex) {
        auto pTex = std::make_shared<tex::img>();
        pTex->Load(PROMPT_TEX_PATH);
        closePromptTexture = texture::manager.Add(PROMPT_TEX_PATH, pTex);
    } else {
        closePromptTexture = texture::manager.Get(PROMPT_TEX_PATH);
    }
}


map_grid::map_grid(const rect_winapi bounds, const point& cellSizes, const camera* pCamera)
    : map_grid(pCamera)
{
    this->bounds = bounds;
    this->cellSizes = cellSizes;
}


bool map_grid::Load(rapidxml::xml_node<char>* gridNode)
{
    auto attrLeft = gridNode->first_attribute("left");
    auto attrRight = gridNode->first_attribute("right");
    auto attrTop = gridNode->first_attribute("top");
    auto attrBottom = gridNode->first_attribute("bottom");

    auto attrCellX = gridNode->first_attribute("cellsX");
    auto attrCellY = gridNode->first_attribute("cellsY");

    if (!(attrLeft && attrRight && attrTop && attrBottom
            && attrCellX && attrCellY)) {
        return false;
    }

    bounds = rect_winapi(std::stof(attrLeft->value()), std::stof(attrTop->value()),
                         std::stof(attrRight->value()), std::stof(attrBottom->value()));
    cellSizes = point(std::stoi(attrCellX->value()), std::stoi(attrCellY->value()));
    cells.resize(cellSizes.x * cellSizes.y);

    return true;
}


void map_grid::Save(rapidxml::xml_document<>* doc, rapidxml::xml_node<char>* gridNode)
{
    auto node = doc->allocate_node(rapidxml::node_element, "value");

    std::string txt = std::to_string(bounds.left);
    auto attr = doc->allocate_attribute("left", doc->allocate_string(txt.c_str(), txt.size() + 1));
    node->append_attribute(attr);

    txt = std::to_string(bounds.right);
    attr = doc->allocate_attribute("right", doc->allocate_string(txt.c_str(), txt.size() + 1));
    node->append_attribute(attr);

    txt = std::to_string(bounds.top);
    attr = doc->allocate_attribute("top", doc->allocate_string(txt.c_str(), txt.size() + 1));
    node->append_attribute(attr);

    txt = std::to_string(bounds.bottom);
    attr = doc->allocate_attribute("bottom", doc->allocate_string(txt.c_str(), txt.size() + 1));
    node->append_attribute(attr);

    txt = std::to_string(cellSizes.x);
    attr = doc->allocate_attribute("cellsX", doc->allocate_string(txt.c_str(), txt.size() + 1));
    node->append_attribute(attr);

    txt = std::to_string(cellSizes.y);
    attr = doc->allocate_attribute("cellsY", doc->allocate_string(txt.c_str(), txt.size() + 1));
    node->append_attribute(attr);

    gridNode->append_node(node);
}


void map_grid::Save(int index, rapidxml::xml_document<>* doc, rapidxml::xml_node<char>* bldNode)
{
    std::string txt;
    for (int y = 0; y < cellSizes.y; ++y) {
        for (int x = 0; x < cellSizes.x; ++x) {
            if (auto pBld = cells[y * cellSizes.x + x]) {
                auto node = doc->allocate_node(rapidxml::node_element, "value", pBld->GetDesc().lock()->GetName().c_str());

                txt = std::to_string(index);
                auto attr = doc->allocate_attribute("grid", doc->allocate_string(txt.c_str(), txt.size() + 1));
                node->append_attribute(attr);

                txt = std::to_string(x);
                attr = doc->allocate_attribute("posX", doc->allocate_string(txt.c_str(), txt.size() + 1));
                node->append_attribute(attr);

                txt = std::to_string(y);
                attr = doc->allocate_attribute("posY", doc->allocate_string(txt.c_str(), txt.size() + 1));
                node->append_attribute(attr);

                bldNode->append_node(node);
            }
        }
    }
}


void map_grid::Render()
{
    auto pMtl = gridPrim.GetMaterial().lock();
    if (!isGridVisible || !pMtl) {
        return;
    }
    pMtl->SetUniform("Position", rect_opengl(bounds).TopLeft() * pCamera->GetZoom() + pCamera->GetOffset());
    pMtl->SetUniform("Sizes", rect_opengl(bounds).GetSize() * pCamera->GetZoom());
    pMtl->SetUniform("NumCells", point(cellSizes.x, cellSizes.y));
    gridPrim.Draw();
}


bool map_grid::SetBuilding(int x, int y, map_building::storage_type pBld,
                           unit_manager* unitManager, controls* pControls,
                           camera* pCamera, player_info* playerInfo)
{
    if (!pBld || y * cellSizes.x + x >= cells.size()
        || y < 0 || x < 0 || y >= cellSizes.y || x >= cellSizes.x
        || cells[y * cellSizes.x + x]) {
        return false;
    }
    cells[y * cellSizes.x + x] = pBld;
    unitManager->Add(GetLayer() + 1, pBld);


    map_building::ptr wBld = pBld;
    coord topLeft = (coord_opengl)(coord_winapi)bounds.TopLeft();
    coord cellSize = (coord_opengl)rect_opengl(bounds).GetSize();
    cellSize.x /= cellSizes.x;
    cellSize.y /= cellSizes.y;
    pBld->SetTopLeft((coord_opengl)(topLeft + coord(cellSize.x * x, cellSize.y * y)));
    pBld->SetSizes((coord_opengl)cellSize);

    pBld->SetCallback(pControls, [this, x, y, unitManager, pControls, pCamera, wBld, playerInfo](){
            if (auto pBld = wBld.lock()) {
                auto pConfirmPrompt = std::make_shared<billboard_clickable>(
                    "GRID_BUILDING_DELETE_PROMPT_" + std::to_string(x) + "_" + std::to_string(y),
                    pBld->GetBoundsDefault(), closePromptTexture);
                billboard_clickable::ptr wConfirmPrompt = pConfirmPrompt;
                unitManager->Add(GetLayer() + 2, pConfirmPrompt);
                pConfirmPrompt->SetCallbacks(pControls, 
                    [this, x, y, unitManager, wBld, wConfirmPrompt, playerInfo](){
                        if (auto pBld = wBld.lock()) {
                            pBld->GetDesc().lock()->OnDemolish(playerInfo);
                            unitManager->Remove(wBld);
                            cells[y * cellSizes.x + x] = nullptr;
                        }
                        unitManager->Remove(wConfirmPrompt);
                    },
                    [unitManager, wConfirmPrompt](){
                        unitManager->Remove(wConfirmPrompt);
                    });
                pConfirmPrompt->Update(pCamera);
            }
        });
    AddChild(wBld);

    return true;
}


bool map_grid::BuildBuilding(int x, int y, map_building::storage_type pBld,
                             unit_manager* unitManager, controls* pControls,
                             camera* pCamera, player_info* playerInfo)
{
    if (!SetBuilding(x, y, pBld, unitManager, pControls, pCamera, playerInfo)) {
        return false;
    }
    pBld->GetDesc().lock()->OnBuild(playerInfo);
    return true;
}


bool map_grid::BuildBuilding(const coord_winapi& pt, map_building::storage_type pBld,
                             unit_manager* unitManager, controls* pControls,
                             camera* pCamera, player_info* playerInfo)
{
    int x = (int)((pt.x - bounds.left) / ((bounds.right - bounds.left) / cellSizes.x));
    int y = (int)((pt.y - bounds.top) / ((bounds.bottom - bounds.top) / cellSizes.y));
    return BuildBuilding(x, y, pBld, unitManager, pControls, pCamera, playerInfo);
}
}
