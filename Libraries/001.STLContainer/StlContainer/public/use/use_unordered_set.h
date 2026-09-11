#pragma once

#include "001.STLContainer_Setting.h"

#ifdef FLAG_USE_UNORDERED_SET
#include "src/Unordered_set.h"
#else
#include <unordered_set>
#define Unordered_set unordered_set
#endif