#pragma once

#include <memory>
#include <map>
#include <set>


class unit {
    friend class unit_manager;
public:
    using ptr = std::weak_ptr<unit>;
    using storage_type = std::shared_ptr<unit>;

    virtual ~unit() = default;

    /// @brief Set if unit should be rendered.
    /// Recursively sets children to same visibility.
    /// @param newVisibility true by default.
    virtual void SetVisible(bool newVisibility = true);
    bool IsVisible() const { return isVisible; }

protected:
    bool isVisible = true;


    /// @brief For automatic cleanup register all child units here.
    /// @param child
    void AddChild(ptr child) { children.push_back(child); }

    int GetLayer() const { return layer; }

    /// @brief Called after registration in \b unit_manager.
    /// Any child units should be added here.
    /// @sa unit_manager::Add
    virtual void OnAdd(unit_manager*) {};
    /// @brief All non-render-related logic should be processed here.
    virtual void Update() {};
    /// @brief Everything render-related should be here.
    virtual void Render() {};

    /// @param layer Units on \b HIGHER layer are rendered \b LAST
    /// @sa unit_manager::operator<<
    /// @sa unit_manager::Render<<
    unit() = default;

private:
    /// @brief should ONLY be assigned by unit_manager::Add
    /// @sa unit_manager::Add
    /// @sa unit_manager::Render
    int layer = 0;
    /// @brief Will be removed after this unit.
    /// @sa AddChild()
    /// @sa unit_manager::Remove()
    std::vector<ptr> children;
};


class unit_manager {
public:
    /// @param layer Units on \b HIGHER layer are rendered \b LAST
    /// @sa unit_manager::Render
    unit_manager& Add(int layer, unit::storage_type);
    unit_manager& Remove(unit::ptr);

    void Update();
    void Render();

private:
    std::map<int, std::set<unit::storage_type>> units;
};
