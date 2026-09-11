#pragma once


#include "001.STLContainer_Setting.h"

#ifdef FLAG_USE_SET
#include "src/Set.h"
#else
#define Set set
#endif