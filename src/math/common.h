#pragma once

#include "color.h"
#include "rect.h"
#include "vec.h"
#include "point.h"


using color = color_t<float>;
using vec = vec_t<float>;
using point = point_t<int>;
using coord = point_t<float>;
using rect = rect_t<float>;


class coord_winapi;
/// @brief [-1;1][-1;1], X left to right, Y bottom to top
class coord_opengl : public coord {
public:
    using coord::coord;


    coord_opengl() = default;
    ~coord_opengl() = default;
    explicit coord_opengl(const coord_winapi& other);
    explicit coord_opengl(const coord& other);

    operator coord_winapi() const;


    explicit operator coord() const
    {
        return *this;
    }
};


/// @brief [0;1][0;1], X left to right, Y top to bottom
class coord_winapi : public coord {
public:
    using coord::coord;


    coord_winapi() = default;
    ~coord_winapi() = default;
    coord_winapi(const coord_opengl& other);
    explicit coord_winapi(const coord& other);

    operator coord_opengl() const;


    explicit operator coord() const
    {
        return *this;
    }
};


class rect_winapi;
class rect_opengl : public rect {
public:
    using rect::rect;


    explicit rect_opengl(const rect_winapi& other);
};


class rect_winapi : public rect {
public:
    using rect::rect;
};
