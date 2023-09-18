#pragma once


#include "unit_map_building.h"


namespace units {
namespace map_bld {
class decoration : public map_building::desc {
public:
    static constexpr const char* TYPE = "decoration";


    decoration();
    virtual ~decoration() = default;
    bool Load(const std::string &filename);

    virtual void OnBuild(player_info* pInfo) override;
    virtual void OnDemolish(player_info* pInfo) override;

private:
    float happiness;  /// Gained while present
};
}}
