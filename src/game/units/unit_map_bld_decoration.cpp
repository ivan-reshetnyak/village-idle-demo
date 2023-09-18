#include "pch.h"

#include "RapidXml/rapidxml.hpp"

#include "unit_map_bld_decoration.h"
#include "util/utils.h"
#include "render/resource/tex/tex_img.h"


namespace units {
namespace map_bld {
decoration::decoration()
    : map_building::desc(TYPE)
{
}


void decoration::OnBuild(player_info* pInfo)
{
    pInfo->AddHappiness(happiness);
    map_building::desc::OnBuild(pInfo);
}


void decoration::OnDemolish(player_info* pInfo)
{
    pInfo->AddHappiness(-happiness);
    map_building::desc::OnDemolish(pInfo);
}


bool decoration::Load(const std::string& filename)
{
    map_building::desc::Load(filename);

    auto docText = utils::LoadText(filename);
    if (!docText) {
        return false;
    }

    auto xmlDoc = std::make_unique<rapidxml::xml_document<>>();  // Allocate on heap
    xmlDoc->parse<0>(&docText[0]);

    auto rootNode = xmlDoc->first_node("Decoration");
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
        texture::manager.Add(filename + "_DECORATION_TEXTURE", pTex);
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

    if (auto node = rootNode->first_node("Happiness")) {
        happiness = std::stof(node->value());
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
