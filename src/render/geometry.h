#pragma once

#include <memory>

#include "vertex.h"


class geometry {
public:
    geometry() = default;
    /// @param vertx Takes ownership.
    /// @param indx Takes ownership.
    geometry(int numVert, vertex* vertx, int numInd, int* indx);
    ~geometry();

    int GetNumVert() const { return numVert; }
    int GetNumInd() const { return numInd; }
    int* GetIndArray() const { return indArray; }
    vertex* GetVertArray() const { return vertArray; }

private:
    int numVert = 0, numInd = 0;
    int* indArray = nullptr;
    vertex* vertArray = nullptr;
};
