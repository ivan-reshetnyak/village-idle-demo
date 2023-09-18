#include "pch.h"

#include "geometry.h"


geometry::geometry(int numVert, vertex* vertx,
                   int numInd, int* indx)
    : numVert(numVert), numInd(numInd)
    , vertArray(vertx)
    , indArray(indx)
{
}


geometry::~geometry()
{
    delete[] vertArray;
    delete[] indArray;
}
