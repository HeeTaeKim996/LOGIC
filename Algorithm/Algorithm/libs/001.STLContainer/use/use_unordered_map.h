#pragma once

#include "001.STLContainer_AllSet.h"

#ifdef FLAG_USE_CUSTOM_LIBRARY
#include "public/Unordered_map.h"
#define unordered_map Unordered_map
#else
#include <unordered_map>
#define unordered_map unordered_map
#endif