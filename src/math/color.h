#pragma once


/// @brief 4-component col class.
/// @tparam type Fractional type.
template<class type>
class color_t {
public:
    using dword = unsigned long;
    type r, g, b, a;


    explicit color_t()
        : r(0), g(0), b(0), a(1)
    {
    }


    explicit color_t(const type& r, const type& g, const type& b, const type& a = 1)
        : r(r), g(g), b(b), a(a)
    {
    }


    explicit color_t(const type& value, const type& a = 1)
        : r(value), g(value), b(value), a(a)
    {
    }


    explicit color_t(const dword& val)
        : r(((val >> 16) & 0xFF) / 255.0)
        , g(((val >> 8) & 0xFF) / 255.0)
        , b((val & 0xFF) / 255.0)
        , a(((val >> 24) & 0xFF) / 255.0)
    {
    }


    color_t operator+(const color_t & col) const
    {
        return color_t(r + col.r, g + col.g, b + col.b, a + col.a);
    }


    color_t& operator+=(const color_t& col)
    {
        r += col.r;
        g += col.g;
        b += col.b;
        a += col.a;
        return *this;
    }


    color_t operator-(const color_t& col) const
    {
        return color_t(r - col.r, g - col.g, b - col.b, a - col.a);
    }


    color_t & operator-=(const color_t& col)
    {
        r -= col.r;
        g -= col.g;
        b -= col.b;
        a -= col.a;
        return *this;
    }


    color_t operator*(const type& mul) const
    {
        return color_t(r * mul, g * mul, b * mul, a * mul);
    }


    color_t& operator*=(const type& mul)
    {
        r *= mul;
        g *= mul;
        b *= mul;
        a *= mul;
        return *this;
    }


    color_t operator*(const color_t& col) const
    {
        return color_t(r * col.r, g * col.g, b * col.b, a * col.a);
    }


    color_t& operator*=(const color_t& col)
    {
        r *= col.r;
        g *= col.g;
        b *= col.b;
        a *= col.a;
        return *this;
    }


    color_t operator/(const type &val ) const
    {
        return color_t(r / val, g / val, b / val, a / val);
    }


    color_t& operator/=(const type& val)
    {
        r /= val;
        g /= val;
        b /= val;
        a /= val;
        return *this;
    }


    /// @brief Linear interpolation between 2 colors
    /// @param first
    /// @param second
    /// @param t
    /// @return first * (1 - t) + second * t
    static color_t mix(const color_t& first, const color_t& second, const type t)
    {
        return first * ((type)1 - t) + second * t;
    }
};
