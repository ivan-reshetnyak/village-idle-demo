#pragma once

#include <memory>
#include <vector>

#include "RapidXml/rapidxml.hpp"

#include "paths.h"
#include "unit_map_building.h"
#include "game/camera.h"
#include "game/unit.h"
#include "render/prim/points.h"
#include "math/common.h"


namespace units {
class map_grid : public unit {
public:
    using ptr = std::weak_ptr<map_grid>;


    map_grid(const camera*);
    map_grid(const rect_winapi bounds, const point& cellSizes,
        const camera*);
    bool Load(rapidxml::xml_node<char>*);
    /// @brief Save grid description.
    /// @param document for allocations.
    /// @param <Grids> node to attach to.
    void Save(rapidxml::xml_document<>*, rapidxml::xml_node<char>*);
    /// @brief Save buildings.
    /// @param index Grid index.
    /// @param document for allocations.
    /// @param <Buildings> node to attach to.
    void Save(int index, rapidxml::xml_document<>*, rapidxml::xml_node<char>*);
    virtual ~map_grid() = default;

    void ToggleGridVisible() { isGridVisible = !isGridVisible; }

    /// @brief Assign new building to the (x; y) cell.
    /// Does NOT call map_building::desc::OnBuild().
    /// Use for buildings loaded from file and such.
    /// @sa map_building::desc::OnBuild()
    /// @return true is building was added.
    bool SetBuilding(int x, int y, map_building::storage_type,
        unit_manager*, controls*, camera*, player_info*);
    /// @brief Assign new building to the (x; y) cell.
    /// Calls map_building::desc::OnBuild().
    /// Use for buildings constructed at runtime and such.
    /// @sa map_building::desc::OnBuild()
    /// @return true is building was added.
    bool BuildBuilding(int x, int y, map_building::storage_type, unit_manager*, controls*, camera*, player_info*);
    /// @brief Assign new building to the cell at given screen coordinates.
    /// Calls map_building::desc::OnBuild().
    /// Use for buildings constructed at runtime and such.
    /// @sa map_building::desc::OnBuild()
    /// @return true is building was added.
    bool BuildBuilding(const coord_winapi& pt, map_building::storage_type, unit_manager*, controls*, camera*, player_info*);

    bool Has(const coord_winapi& pt) const { return bounds.Has(pt); }

private:
    static constexpr const char* SHADER_PATH = PATHS_SHADERS "grid/";
    static constexpr const char* MTL_NAME = "GRID_MTL";
    static constexpr const char* GRID_TEX_PATH = PATHS_IMG "grid_cell.png";
    static constexpr const char* PROMPT_TEX_PATH = PATHS_IMG "close.png";


    bool isGridVisible = false;
    prim::points gridPrim;
    point cellSizes;
    rect_winapi bounds;
    const camera* pCamera;
    std::vector<std::shared_ptr<map_building>> cells;
    texture::ptr closePromptTexture;


    void Render() override;
};
}
