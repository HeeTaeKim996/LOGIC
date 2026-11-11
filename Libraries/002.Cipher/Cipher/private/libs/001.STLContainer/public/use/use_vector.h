#pragma once

#include "001.STLContainer_Setting.h"

#ifdef FLAG_USE_VECTOR
#include "src/Vector.h"
#else
#include <vector>
#define Vector vector
#endif
