#include "pch.h"

#include "unit.h"


void unit::SetVisible(bool newVisibility)
{
    isVisible = newVisibility;
    for (auto& wChild : children) {
        if (auto pChild = wChild.lock()) {
            pChild->SetVisible(newVisibility);
        }
    }
}


unit_manager& unit_manager::Add(int layer, unit::storage_type newUnit)
{
    newUnit->layer = layer;

    auto layerIt = units.find(layer);
    if (layerIt == units.end()) {
        layerIt = units.insert({layer, std::set<unit::storage_type>()}).first;
    }

    layerIt->second.insert(newUnit);
    newUnit->OnAdd(this);

    return *this;
}


void unit_manager::Update()
{
    for (auto& layerIt : units) {
        for (auto& unitIt : layerIt.second) {
            unitIt->Update();
        }
    }
}


void unit_manager::Render()
{
    for (auto& layerIt : units) {
        for (auto& unitIt : layerIt.second) {
            if (unitIt->IsVisible()) {
                unitIt->Render();
            }
        }
    }
}


unit_manager& unit_manager::Remove(unit::ptr wUnit)
{
    auto pUnit = wUnit.lock();
    if (!pUnit) {
        // Unit was never added into manager or was already deleted.
        return *this;
    }
    auto layerIt = units.find(pUnit->layer);
    if (layerIt == units.end()) {
        // Something wrong with layering.
        return *this;
    }

    auto unitIt = layerIt->second.find(pUnit);
    if (unitIt == layerIt->second.end()) {
        // Unit was never added into manager or was already deleted.
        return *this;
    }
    for (auto& child : unitIt->get()->children) {
        Remove(child);
    }
    layerIt->second.erase(unitIt);
    return *this;
}
