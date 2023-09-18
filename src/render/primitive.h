#pragma once

#include <memory>

#include "buffer.h"
#include "geometry.h"
#include "resource/material.h"


class primitive {
public:
    primitive() = default;
    virtual ~primitive() = default;
    /// @brief Generate buffers for this new geometry.
    /// @param geom Shares ownership.
    void Set(std::shared_ptr<geometry> geom);
    void Set(material::ptr wMat) { wMaterial = wMat; }
    material::ptr GetMaterial() { return wMaterial; }

    void Draw();

protected:
    buffer mBuffer;
    std::shared_ptr<geometry> pGeometry;
    material::ptr wMaterial;

    virtual void Render() = 0;

private:
    // primitive & operator=(const primitive &&P);
    primitive & operator=(const primitive &P) = delete;
    primitive(const primitive &P) = delete;
};