#include "pch.h"

#include "trimesh.h"


namespace prim {
void trimesh::Render()
{
    glBindVertexArray(mBuffer.GetVertexArray());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer.GetIndexBuf());
    glDrawElements(GL_TRIANGLES, mBuffer.GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
}
}
