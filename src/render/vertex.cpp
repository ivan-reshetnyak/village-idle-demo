#include "pch.h"

#include "vertex.h"


namespace vertices {
simple::simple(const vec& pos, const vec& norm,
               const tex_coord& tex, const color& col )
    : pos(pos), norm(norm), tex(tex), col(col)
{
}


simple::simple(const simple& other)
{
  pos = other.pos;
  norm = other.norm;
  tex = other.tex;
  col = other.col;
}


std::vector<simple::location> simple::GetLayout() const
{
    std::vector<simple::location> layout;
    layout.push_back(location({false, 0, 3, GL_FLOAT, sizeof(simple), OFFSET(simple, pos)}));
    layout.push_back(location({false, 1, 3, GL_FLOAT, sizeof(simple), OFFSET(simple, norm)}));
    layout.push_back(location({false, 2, 4, GL_FLOAT, sizeof(simple), OFFSET(simple, col)}));
    layout.push_back(location({false, 3, 2, GL_FLOAT, sizeof(simple), OFFSET(simple, tex)}));
    return layout;
}


int simple::GetSize() const
{
    return sizeof(simple);
}


} // End of 'vertices' namespace
