#pragma once

#include "render/primitive.h"


namespace prim {
class points : public primitive {
protected:
    virtual void Render() override;
};
}
