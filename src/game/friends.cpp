#include "pch.h"

#include "friends.h"
#include "render/resource/tex/tex_img.h"


bool friends::Load(rapidxml::xml_node<char>* rootNode)
{
    auto friendsNode = rootNode->first_node("Friends");

    for (auto element = friendsNode->first_node(); element; element = element->next_sibling()) {
        auto levelAttr = element->first_attribute("level");
        auto nameAttr = element->first_attribute("name");
        if (!(levelAttr && nameAttr)) {
            return false;
        }
        Add(std::stoi(levelAttr->value()), nameAttr->value(), element->value());
    }

    return true;
}


void friends::Save(rapidxml::xml_document<>* doc, rapidxml::xml_node<char>* mapNode)
{
    auto friendsNode = doc->allocate_node(rapidxml::node_element, "Friends");

    std::string txt;
    for (auto& it : data) {
        auto node = doc->allocate_node(rapidxml::node_element, "value", it.texPath.c_str());

        auto attr = doc->allocate_attribute("name", it.name.c_str());
        node->append_attribute(attr);

        txt = std::to_string(it.level);
        attr = doc->allocate_attribute("level", doc->allocate_string(txt.c_str(), txt.size() + 1));
        node->append_attribute(attr);

        friendsNode->append_node(node);
    }

    mapNode->append_node(friendsNode);
}


void friends::Add(int level, const std::string& name, const std::string& texPath)
{
    if (texture::manager.Exists(texPath)) {
        data.emplace_back(level, name, texPath, texture::manager.Get(texPath));
        return;
    }
    auto pTex = std::make_shared<tex::img>();
    pTex->Load(texPath);
    data.emplace_back(level, name, texPath, texture::manager.Add(texPath, pTex));
}
