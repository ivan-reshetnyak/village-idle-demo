#pragma once

#include "render/primitive.h"


namespace prim {
class trimesh : public primitive {
protected:
    virtual void Render() override;
};
}
