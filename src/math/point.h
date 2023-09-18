#pragma once


template<class type>
class point_t {
public:
    type x, y;


    explicit point_t() = default;


    point_t(const point_t& other)
        : x(other.x), y(other.y)
    {
    }


    explicit point_t(const type& x, const type& y)
        : x(x), y(y)
    {
    }


    point_t& operator=(const point_t& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }


    point_t operator+(const point_t& other) const { return point_t(x + other.x, y + other.y); }
    point_t operator-(const point_t& other) const { return point_t(x - other.x, y - other.y); }


    point_t& operator+=(const point_t& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }


    point_t& operator-=(const point_t& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }


    point_t operator*(const type& mul) const { return point_t(x * mul, y * mul); }
    point_t operator/(const type& mul) const { return point_t(x / mul, y / mul); }


    point_t& operator*=(const type& mul)
    {
        x *= mul;
        y *= mul;
        return *this;
    }


    point_t& operator/=(const type& mul)
    {
        x /= mul;
        y /= mul;
        return *this;
    }


    point_t operator*(const point_t& other) const { return point_t(x * other.x, y * other.y); }
    point_t operator/(const point_t& other) const { return point_t(x / other.x, y / other.y); }


    point_t& operator*=(const point_t& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }


    point_t& operator/=(const point_t& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }
};
