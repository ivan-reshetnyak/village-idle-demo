#pragma once

#include "util/resource_manager.h"


class texture {
private:
    friend class manager_t;
    friend resource_manager_named<texture>;
    class manager_t : public resource_manager_named<texture> {
    public:
        /// @brief Crash if not found.
        ptr Get(const std::string& name) override;
        ptr Add(const std::string& name, storage_type pTex);
    };

public:
    using ptr = std::weak_ptr<texture>;


    static manager_t manager;
    static const int INVALID_ID = 0;
    static const int INVALID_SAMPLER = -1;


    virtual ~texture();
    void Enable();
    void Disable();

    void SetSampler(int newSampler) { sampler = newSampler; }
    int GetSampler() const { return sampler; }

protected:
    unsigned glId = INVALID_ID;


    texture(int samplerNum = INVALID_SAMPLER);

private:
    int sampler;
};
