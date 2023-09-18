#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <windowsx.h>
#undef min
#undef max

#ifndef GLEW_STATIC
#  define GLEW_STATIC
#endif
#include "glew/glew.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
