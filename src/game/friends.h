#pragma once

#include <string>
#include <vector>

#include "RapidXml/rapidxml.hpp"

#include "render/resource/texture.h"


class friends {
public:
    struct profile {
        int level;
        std::string name, texPath;
        texture::ptr texture;


        profile(int level, const std::string& name, const std::string& texPath, texture::ptr tex)
            : level(level), name(name), texture(tex), texPath(texPath)
        {
        }
    };


    /// @return true if successful.
    bool Load(rapidxml::xml_node<char>* rootNode);
    /// @brief Save to xml node.
    /// @param document for allocations.
    /// @param <Map> node to attach to.
    void Save(rapidxml::xml_document<>*, rapidxml::xml_node<char>*);

    const std::vector<profile>& Get() const { return data; }
    void Add(int level, const std::string& name, const std::string& texPath);

private:
    std::vector<profile> data;
};
