#pragma once

#include "001.STLContainer_Setting.h"

#ifdef FLAG_USE_UNORDERED_MAP
#include "src/Unordered_map.h"
#else
#include <unordered_map>
#define Unordered_map unordered_map
#endif