#pragma once

#include <cmath>


template<class type>
class vec_t {
public:
    type x, y, z;


    explicit vec_t() = default;


    explicit vec_t(const type& val)
        : x(val), y(val), z(val)
    {
    }


    vec_t(const type& x, const type& y, const type& z)
        : x(x), y(y), z(z)
    {
    }


    type Length() const
    {
        type len = x * x + y * y + z * z;

        if (len > 0) {
            if (len != 1) {
                len = sqrt(len);
                return len;
            }
            return len;
        }
        return len;
    }


    /// @brief Vector dot product
    type operator&(const vec_t& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }


    /// @brief Vector cross product
    vec_t operator%(const vec_t& v) const
    {
        return vec_t(y * v.z - v.y * z, v.x * z - x * v.z, x * v.y - v.x * y);
    }


    bool operator==(const vec_t& v) const
    {
        return (x == v.x) && (y == v.y) && (z == v.z);
    }


    bool operator!=(const vec_t& v) const
    {
        return (x != v.x) || (y != v.y) || (z != v.z);
    }


    vec_t operator+(const vec_t& v) const
    {
        return vec_t(x + v.x, y + v.y, z + v.z);
    }


    vec_t operator-(const vec_t& v) const
    {
        return vec_t(x - v.x, y - v.y, z - v.z);
    }


    /// @brief Component-wise multiplication
    vec_t operator*(const vec_t& v) const
    {
        return vec_t(x * v.x, y * v.y, z * v.z);
    }


    vec_t operator/(const vec_t& v) const
    {
        return vec_t(x / v.x, y / v.y, z / v.z);
    }


    /// @brief Component-wise multiplication
    vec_t operator*(const type& a) const
    {
        return vec_t(a * x, a * y, a * z);
    }


    /// @brief Component-wise division. No zero check.
    vec_t operator/(const type& a) const
    {
        return vec_t(x / a, y / a, z / a);
    }


    vec_t operator-() const
    {
        return vec_t(-x, -y, -z);
    }


    vec_t& Negate()
    {
        x = -x;
        y = -y;
        z = -z;

        return *this;
    }


    vec_t& operator+=(const vec_t& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }


    vec_t& operator-=(const vec_t & v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }


    /// @brief Component-wise multiplication
    vec_t& operator*=(const type& a)
    {
        x *= a;
        y *= a;
        z *= a;

        return *this;
    }


    /// @brief Component-wise multiplication
    vec_t& operator*=(const vec_t& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;

        return *this;
    }


    /// @brief Cross product
    vec_t& operator%=(const vec_t& v)
    {
        type a, b;

        a = y * v.z - v.y * z;
        b = v.x * z - x * v.z;
        z = x * v.y - v.x * y;
        x = a;
        y = b;

        return *this;
    }


    /// @brief Component-wise division. No zero check.
    vec_t& operator/=(const type& a)
    {
        x /= a;
        y /= a;
        z /= a;

        return *this;
    }


    /// @brief Access component by index. NO SAFETY CHECKS!
    type& operator[](int i)
    {
        return *(&x + i);
    }


    vec_t& Normalize()
    {
        type len = x * x + y * y + z * z;

        if (len > 0) {
            if (len != 1) {
                len = sqrt(len);
                x /= len;
                y /= len;
                z /= len;
                return *this;
            }
            return *this;
        }
        return *this;
    }


    vec_t Normalized() const
    {
        type len = x * x + y * y + z * z;

        if (len > 0) {
            if (len != 1) {
                len = sqrt(len);
                return vec_t(x / len, y / len, z / len);
            }
            return *this;
        }
        return *this;
    }
};
