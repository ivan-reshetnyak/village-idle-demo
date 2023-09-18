#include "pch.h"

#ifndef GLEW_STATIC
#  define GLEW_STATIC
#endif
#include "glew/glew.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glew32s")

#include "buffer.h"
#include "resource/shader.h"


buffer::buffer(const geometry& geom)
{
    Generate(geom);
}


void buffer::Generate(const geometry& geom)
{
    /* Vertex buffer */
    glGenBuffers(1, &vertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
    glBufferData(GL_ARRAY_BUFFER,
                 geom.GetVertArray()->GetSize() * geom.GetNumVert(),
                 &*geom.GetVertArray(), GL_STATIC_DRAW);

    /* Index buffer */
    glGenBuffers(1, &indBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * geom.GetNumInd(),
                 geom.GetIndArray(), GL_STATIC_DRAW);

    /* Vertex array buffer */
    glGenVertexArrays(1, &vertArray);
    glBindVertexArray(vertArray);

    shader::SetLayout(geom.GetVertArray());

    numInd = geom.GetNumInd();
    Unbind();
}


void buffer::Unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
