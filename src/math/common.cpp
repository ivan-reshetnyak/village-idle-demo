#include "pch.h"

#include "common.h"


coord_opengl::coord_opengl(const coord_winapi& other)
    : coord(other.x * 2.f - 1.f, -other.y * 2.f + 1.f)
{
}


coord_opengl::coord_opengl(const coord& other)
    : coord(other)
{
}


coord_opengl::operator coord_winapi() const
{
    return coord_winapi((x + 1.f) * 0.5f, 1.f - ((y + 1.f) * 0.5f));
}


coord_winapi::coord_winapi(const coord& other)
    : coord(other)
{
}


coord_winapi::coord_winapi(const coord_opengl& other)
    : coord((other.x + 1.f) * 0.5f, 1.f - ((other.y + 1.f) * 0.5f))
{
}


coord_winapi::operator coord_opengl() const
{
    return coord_opengl(x * 2.f - 1.f, -y * 2.f + 1.f);
}


rect_opengl::rect_opengl(const rect_winapi& other)
    : rect((coord_opengl)(coord_winapi)other.TopLeft(),
           (coord_opengl)(coord_winapi)other.BottomRight())
{
}
