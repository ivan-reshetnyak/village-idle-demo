#pragma once


#include "unit_map_building.h"


namespace units {
namespace map_bld {
class workshop : public map_building::desc {
public:
    static constexpr const char* TYPE = "workshop";


    workshop();
    virtual ~workshop() = default;
    bool Load(const std::string &filename) override;

    virtual void OnBuild(player_info*) override;
    virtual void Update(player_info*, int ticksPassed) override;

private:
    float gpm;  /// Gold per minute
};
}}
