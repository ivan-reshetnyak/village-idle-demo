#pragma once

#include "geometry.h"


class buffer {
public:
  buffer() = default;
  buffer(const geometry&);
  void Generate(const geometry&);

  static void Unbind();

  int GetNumOfIndices() const { return numInd; }
  unsigned GetIndexBuf() const { return indBuf; }
  unsigned GetVertexBuf() const { return vertBuf; }
  unsigned GetVertexArray() const { return vertArray; }

private:
  int numInd = 0;
  unsigned indBuf = 0, vertBuf = 0, vertArray = 0;
};
