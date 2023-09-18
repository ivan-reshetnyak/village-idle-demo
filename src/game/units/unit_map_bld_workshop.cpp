#include "pch.h"

#include "RapidXml/rapidxml.hpp"

#include "unit_map_bld_workshop.h"
#include "util/utils.h"
#include "render/resource/tex/tex_img.h"


namespace units {
namespace map_bld {
workshop::workshop()
    : map_building::desc(TYPE)
{
}


void workshop::OnBuild(player_info* pInfo)
{
    map_building::desc::OnBuild(pInfo);
}


void workshop::Update(player_info* pInfo, int ticksPassed)
{
    pInfo->AddGold(gpm / 60.f * ticksPassed);
}


bool workshop::Load(const std::string& filename)
{
    map_building::desc::Load(filename);

    auto docText = utils::LoadText(filename);
    if (!docText) {
        return false;
    }

    auto xmlDoc = std::make_unique<rapidxml::xml_document<>>();  // Allocate on heap
    xmlDoc->parse<0>(&docText[0]);

    auto rootNode = xmlDoc->first_node("Workshop");
    if (!rootNode) {
        return false;
    }

    if (auto node = rootNode->first_node("Name")) {
        name = node->value();
    } else {
        return false;
    }

    if (auto node = rootNode->first_node("Background")) {
        auto pTex = std::make_shared<tex::img>();
        texture::manager.Add(filename + "_WORKSHOP_TEXTURE", pTex);
        if (!pTex->Load(node->value())) {
            return false;
        }
        mTexture = pTex;
    } else {
        return false;
    }

    if (auto node = rootNode->first_node("Description")) {
        description = node->value();
    } else {
        return false;
    }

    if (auto node = rootNode->first_node("Income")) {
        gpm = std::stof(node->value());
    } else {
        return false;
    }

    if (auto node = rootNode->first_node("Cost")) {
        cost = std::stoi(node->value());
    } else {
        return false;
    }

    if (auto node = rootNode->first_node("MinLevel")) {
        minLevel = std::stoi(node->value());
    } else {
        return false;
    }

    if (auto node = rootNode->first_node("XP")) {
        experience = std::stof(node->value());
    } else {
        return false;
    }

    return true;
}
}}
