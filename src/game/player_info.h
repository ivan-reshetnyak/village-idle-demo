#pragma once

#include <string>

#include "RapidXml/rapidxml.hpp"


class player_info {
public:
    /// @brief Load from xml node.
    /// @param rootNode
    /// @return true if successful.
    bool Load(rapidxml::xml_node<char>* rootNode);
    /// @brief Save to xml node.
    /// @param document for allocations.
    /// @param <Map> node to attach to.
    void Save(rapidxml::xml_document<>*, rapidxml::xml_node<char>*);

    /// @brief Set experience to new value and check for level-ups.
    /// @param xp
    /// @return true if at least one level-up has occured.
    bool SetXP(float xp);
    /// @brief Add experience and check for level-up.
    /// @param xp
    /// @returns true if level-up has occured from this.
    bool AddXP(float xp);
    void SetLevel(int newLevel);
    int GetLevel() const { return currentLevel; }
    /// @return Current xp, rounded down.
    int GetXP() const { return (int)currentXP; }
    /// @return xp required for next level-up, rounded down.
    int GetXPRequired() const { return (int)GetExperienceRequired(currentLevel); }

    void SetHappiness(float newHappiness);
    void AddHappiness(float delta);
    float GetHappiness() const { return currentHappiness; }

    void SetGold(float newGold);
    void AddGold(float delta);
    /// @return Current gold, rounded down.
    int GetGold() const { return (int)currentGold; }

    bool WasUpdated() const { return wasUpdated; }
    void ResetUpdated() { wasUpdated = false; }

    void SetName(const std::string&);
    const std::string& GetName() const { return name; }

private:
    bool wasUpdated = false;
    int currentLevel = 1;
    float currentXP = 0;  /// Stored as float, used as int
    float currentGold = 0;  /// Stored as float, used as int
    float currentHappiness = 0;
    std::string name;


    static float GetExperienceRequired(int lvl) { return 10.f * lvl; }
    /// @brief Check if player needs to level-up and level-up if so.
    /// Can level-up multiple times.
    /// @returns true if at least one level-up has occured.
    bool CheckLvlUp();
};
