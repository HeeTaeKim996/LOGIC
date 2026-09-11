#pragma once

#include "001.STLContainer_Setting.h"

#ifdef FLAG_USE_MAP
#include "src/Map.h"
#else
#define Map map
#endif