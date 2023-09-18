#include "pch.h"

#include "points.h"


namespace prim {
void points::Render()
{
    glBindVertexArray(mBuffer.GetVertexArray());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer.GetIndexBuf());
    glDrawElements(GL_POINTS, mBuffer.GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
}
}
