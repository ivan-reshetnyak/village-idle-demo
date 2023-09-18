#pragma once

#include <string>
#include <memory>

#include "util/resource_manager.h"
#include "render/vertex.h"
#include "math/common.h"


class shader {
private:
    friend class manager_t;
    friend resource_manager_named<shader>;
    class manager_t : public resource_manager_named<shader> {
    public:
        ptr Get(const std::string& name) override;
    };

public:
    using ptr = manager_t::ptr;


    static manager_t manager;


    static void SetLayout(vertex*);

    void Enable();
    void Disable();

    int GetProgram() const { return glProgram; }

    void SetUniform(const std::string &name, float val) const;
    void SetUniform(const std::string &name, int val) const;
    void SetUniform(const std::string &name, const point& val) const;
    void SetUniform(const std::string &name, const coord& val) const;
    // void SetUniform(const std::string &name, matrix &val) const;
    void SetUniform(const std::string &name, const vec& val) const;
    void SetUniform(const std::string &name, const color& val) const;

private:
    static const int ERROR_LOG_LENGTH = 1024;
    static const int NUM_SHADER_TYPES = 5;
    static constexpr const char* LOG_PATH = "logs/shader.log";

    int glProgram = 0;


    shader() = default;

    /// @brief Load all shaders in directory.
    /// @param path Valid path to directory containing shader files, with a trailing slash.
    void Load(const std::string& path);
    static void SaveLog(const std::string& text);
};
