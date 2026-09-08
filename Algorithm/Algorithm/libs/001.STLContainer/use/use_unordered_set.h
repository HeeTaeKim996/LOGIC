#pragma once

#include "001.STLContainer_AllSet.h"

#ifdef FLAG_USE_CUSTOM_LIBRARY
#include "public/Unordered_set.h"
#define unordered_set Unordered_set
#else
#include <unordered_set>
#define unordered_set unordered_set
#endif