#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "util/resource_manager.h"
#include "texture.h"
#include "shader.h"
#include "math/common.h"


class material {
private:
    friend resource_manager_named<material>;
    using manager_t = resource_manager_named<material>;

public:
    using ptr = manager_t::ptr;


    static manager_t manager;

    ~material() = default;
    void Set(shader::ptr newShader) { mShader = newShader; }
    void Add(const std::string& uniformName, texture::ptr pTex) { textures[uniformName] = pTex; }

    void SetUniform(const std::string& name, float val) { unifFloat[name] = val; }
    void SetUniform(const std::string& name, const coord& val) { unifFloat2[name] = val; }
    void SetUniform(const std::string& name, const color& val) { unifFloat4[name] = val; }
    void SetUniform(const std::string& name, const point& val) { unifInt2[name] = val; }

    void Enable();
    void Disable();

private:
    shader::ptr mShader;
    std::unordered_map<std::string, texture::ptr> textures;
    std::unordered_map<std::string, float> unifFloat;
    std::unordered_map<std::string, coord> unifFloat2;
    std::unordered_map<std::string, color> unifFloat4;
    std::unordered_map<std::string, point> unifInt2;


    material() = default;
};
