#pragma once


#include <vector>

#include "math/common.h"


#define OFFSET(type, field) \
  ((void *)(&((type *)0)->field))


struct tex_coord {
    float U, V;
};


class vertex {
public:
    struct location {
        bool isNormalised;
        int index, size, type, stride;
        const void* pointer;
    };

    virtual std::vector<location> GetLayout() const { return std::vector<location>(); }
    virtual int GetSize() const { return sizeof(vertex); }

    vertex() = default;
    virtual ~vertex() = default;
};


namespace vertices {
class simple : public vertex {
public:
  tex_coord tex;
  vec pos, norm;
  color col;

  simple() = default;
  virtual ~simple() = default;
  simple(const vec& pos, const vec& norm,
         const tex_coord& tex, const color& col);
  simple(const simple&);

  virtual std::vector<location> GetLayout() const override;
  virtual int GetSize() const override;
};
} // End of 'vertices' namespace
