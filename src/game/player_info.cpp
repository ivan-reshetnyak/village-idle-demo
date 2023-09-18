#include "pch.h"

#include "player_info.h"


bool player_info::Load(rapidxml::xml_node<char>* rootNode)
{
    auto node = rootNode->first_node("Player");
    if (!node) {
        return false;
    }

    if (auto nameNode = node->first_node("Name")) {
        SetName(nameNode->value());
    } else {
        return false;
    }

    if (auto goldNode = node->first_node("Gold")) {
        SetGold(std::stof(goldNode->value()));
    } else {
        return false;
    }

    if (auto levelNode = node->first_node("Level")) {
        SetLevel(std::stoi(levelNode->value()));
    } else {
        return false;
    }

    if (auto xpNode = node->first_node("Experience")) {
        SetXP(std::stof(xpNode->value()));
    } else {
        return false;
    }

    if (auto happyNode = node->first_node("Happiness")) {
        SetHappiness(std::stof(happyNode->value()));
    } else {
        return false;
    }

    return true;
}


void player_info::Save(rapidxml::xml_document<>* doc, rapidxml::xml_node<char>* rootNode)
{
    auto playerNode = doc->allocate_node(rapidxml::node_element, "Player");

    auto node = doc->allocate_node(rapidxml::node_element, "Name", name.c_str());
    playerNode->append_node(node);

    std::string txt = std::to_string(currentGold);
    node = doc->allocate_node(rapidxml::node_element, "Gold",
        doc->allocate_string(txt.c_str(), txt.size() + 1));
    playerNode->append_node(node);

    txt = std::to_string(currentLevel);
    node = doc->allocate_node(rapidxml::node_element, "Level",
        doc->allocate_string(txt.c_str(), txt.size() + 1));
    playerNode->append_node(node);

    txt = std::to_string(currentXP);
    node = doc->allocate_node(rapidxml::node_element, "Experience",
        doc->allocate_string(txt.c_str(), txt.size() + 1));
    playerNode->append_node(node);

    txt = std::to_string(currentHappiness);
    node = doc->allocate_node(rapidxml::node_element, "Happiness",
        doc->allocate_string(txt.c_str(), txt.size() + 1));
    playerNode->append_node(node);

    rootNode->append_node(playerNode);
}


bool player_info::SetXP(float xp)
{
    wasUpdated = wasUpdated || (currentXP != xp);
    currentXP = xp;
    return CheckLvlUp();
}


bool player_info::AddXP(float xp)
{
    wasUpdated = wasUpdated || (xp != 0);
    currentXP += xp;
    return CheckLvlUp();
}


void player_info::SetLevel(int newLevel)
{
    wasUpdated = wasUpdated || (currentLevel != newLevel);
    currentLevel = newLevel;
}


bool player_info::CheckLvlUp()
{
    float xpReq = GetExperienceRequired(currentLevel);

    if (currentXP < xpReq) {
        return false;
    }

    while (currentXP >= xpReq) {
        currentXP -= xpReq;
        xpReq = GetExperienceRequired(++currentLevel);
    }

    wasUpdated = true;
    return true;
}


void player_info::SetHappiness(float newHappiness)
{
    wasUpdated = wasUpdated || (currentHappiness != newHappiness);
    currentHappiness = newHappiness;
}


void player_info::AddHappiness(float delta)
{
    wasUpdated = wasUpdated || (delta != 0);
    currentHappiness += delta;
}


void player_info::SetGold(float newGold)
{
    wasUpdated = wasUpdated || (newGold != currentGold);
    currentGold = newGold;
}


void player_info::AddGold(float delta)
{
    wasUpdated = wasUpdated || (delta != 0);
    currentGold += delta;
}


void player_info::SetName(const std::string& newName)
{
    wasUpdated = wasUpdated || (newName != name);
    name = newName;
}
