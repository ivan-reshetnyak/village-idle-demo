#include "pch.h"

#include <iostream>
#include <fstream>

#include "RapidXml/rapidxml.hpp"
#include "RapidXml/rapidxml_print.hpp"

#include "unit_map.h"
#include "unit_map_bld_decoration.h"
#include "unit_map_bld_workshop.h"
#include "util/utils.h"
#include "render/resource/tex/tex_img.h"



namespace units {
bool map::data::Load(const std::string& filename,
                     int layer, unit_manager* unitManager,
                     window::info winInfo, controls* pControls,
                     camera* pCamera, player_info* playerInfo,
                     friends* pFriends, timer* pTimer)
{
    auto docText = utils::LoadText(filename);
    if (!docText) {
        return false;
    }

    auto xmlDoc = std::make_unique<rapidxml::xml_document<>>();  // Allocate on heap
    xmlDoc->parse<0>(&docText[0]);

    auto mapNode = xmlDoc->first_node("Map");
    if (!mapNode) {
        return false;
    }

    if (auto node = mapNode->first_node("Background")) {
        auto pTex = std::make_shared<tex::img>();
        if (!pTex->Load(node->value())) {
            return false;
        }
        bgTexName = node->value();
        backgroundTexture = texture::manager.Add(filename + "_Background", pTex);
    } else {
        return false;
    }

    if (auto node = mapNode->first_node("ZoomIn")) {
        auto pTex = std::make_shared<tex::img>();
        if (!pTex->Load(node->value())) {
            return false;
        }
        zoomInTexName = node->value();
        zoomInTexture = texture::manager.Add(filename + "_ZoomIn", pTex);
    } else {
        return false;
    }

    if (auto node = mapNode->first_node("ZoomOut")) {
        auto pTex = std::make_shared<tex::img>();
        if (!pTex->Load(node->value())) {
            return false;
        }
        zoomOutTexName = node->value();
        zoomOutTexture = texture::manager.Add(filename + "_ZoomOut", pTex);
    } else {
        return false;
    }

    if (auto node = mapNode->first_node("Descriptions")) {
        for (auto element = node->first_node(); element; element = element->next_sibling()) {
            auto typeAttr = element->first_attribute("type");
            if (!typeAttr) {
                return false;
            }

            map_building::desc::storage_type newDesc = nullptr;
            if (strncmp(typeAttr->value(), "workshop", 8) == 0) {
                newDesc = std::make_shared<map_bld::workshop>();
            } else if (strncmp(typeAttr->value(), "decoration", 10) == 0) {
                newDesc = std::make_shared<map_bld::decoration>();
            }
            if (!newDesc || !newDesc->Load(element->value())) {
                return false;
            }
            map_building::desc::manager.Add(newDesc->GetName(), newDesc);
        }
    } else {
        return false;
    }

    if (auto node = mapNode->first_node("Grids")) {
        for (auto element = node->first_node(); element; element = element->next_sibling()) {
            auto newGrid = std::make_shared<map_grid>(pCamera);
            if (!newGrid->Load(element)) {
                return false;
            }

            unitManager->Add(layer + 1, newGrid);
            grids.push_back(newGrid);
        }
    } else {
        return false;
    }

    if (!(playerInfo->Load(mapNode) && pFriends->Load(mapNode))) {
        return false;
    }

    if (auto node = mapNode->first_node("Buildings")) {
        for (auto element = node->first_node(); element; element = element->next_sibling()) {
            auto gridNAttr = element->first_attribute("grid");
            auto gridXAttr = element->first_attribute("posX");
            auto gridYAttr = element->first_attribute("posY");
            if (!(gridNAttr && gridXAttr && gridYAttr)) {
                return false;
            }
            auto pBld = std::make_shared<map_building>(
                map_building::desc::manager[element->value()],
                pCamera, playerInfo, pTimer);
            // unitManager->Add(layer + 2, pBld);

            grids[std::stoi(gridNAttr->value())]->SetBuilding(
                std::stoi(gridXAttr->value()), std::stoi(gridYAttr->value()),
                pBld, unitManager, pControls, pCamera, playerInfo);
        }
    }

    return true;
}


void map::data::Save(const std::string& filename, player_info* playerInfo, friends* pFriends)
{
    std::ofstream file(filename);
    auto xmlDoc = std::make_unique<rapidxml::xml_document<>>();  // Allocate on heap

    auto mapNode = xmlDoc->allocate_node(rapidxml::node_element, "Map");
    xmlDoc->append_node(mapNode);

    /* Textures */
    auto node = xmlDoc->allocate_node(rapidxml::node_element, "Background", bgTexName.c_str());
    mapNode->append_node(node);
    node = xmlDoc->allocate_node(rapidxml::node_element, "ZoomIn", zoomInTexName.c_str());
    mapNode->append_node(node);
    node = xmlDoc->allocate_node(rapidxml::node_element, "ZoomOut", zoomOutTexName.c_str());
    mapNode->append_node(node);

    /* Descs */
    node = xmlDoc->allocate_node(rapidxml::node_element, "Descriptions");
    for (auto& it : map_building::desc::manager.Get()) {
        auto element = xmlDoc->allocate_node(rapidxml::node_element, "value", it.second->GetPath().c_str());
        auto typeAttr = xmlDoc->allocate_attribute("type", it.second->GetType().c_str());
        element->append_attribute(typeAttr);
        node->append_node(element);
    }
    mapNode->append_node(node);

    /* Grids */
    node = xmlDoc->allocate_node(rapidxml::node_element, "Grids");
    for (auto& pGrid : grids) {
        pGrid->Save(xmlDoc.get(), node);
    }
    mapNode->append_node(node);

    /* Buildings */
    node = xmlDoc->allocate_node(rapidxml::node_element, "Buildings");
    std::string txt;
    for (int i = 0; i < grids.size(); ++i) {
        grids[i]->Save(i, xmlDoc.get(), node);
    }
    mapNode->append_node(node);

    playerInfo->Save(xmlDoc.get(), mapNode);
    pFriends->Save(xmlDoc.get(), mapNode);

    file << *xmlDoc;
}
}
