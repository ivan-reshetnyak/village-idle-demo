#pragma once

#include "point.h"


template<class type>
class rect_t {
public:
    type left, top, right, bottom;


    rect_t() = default;
    ~rect_t() = default;


    rect_t(const type& left, const type& top, const type& right, const type& bottom)
        : left(left), top(top), right(right), bottom(bottom)
    {
    }


    rect_t(const point_t<type>& topleft, const point_t<type>& botright)
        : left(topleft.x), top(topleft.y), right(botright.x), bottom(botright.y)
    {
    }


    point_t<type> TopLeft() const
    {
        return point_t<type>(left, top);
    }


    point_t<type> TopRight() const
    {
        return point_t<type>(right, top);
    }


    point_t<type> BottomRight() const
    {
        return point_t<type>(right, bottom);
    }


    point_t<type> BottomLeft() const
    {
        return point_t<type>(left, bottom);
    }


    /// @brief Get width (top left to bottom right).
    point_t<type> GetSize() const
    {
        return point_t<type>(right - left, bottom - top);
    }


    bool Has(const point_t<type>& pt) const
    {
        return ((left <= pt.x && pt.x <= right) || (right <= pt.x && pt.x <= left))
            && ((top <= pt.y && pt.y <= bottom) || (bottom <= pt.y && pt.y <= top));
    }
};
