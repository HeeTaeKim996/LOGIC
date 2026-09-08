#pragma once

#include "001.STLContainer_AllSet.h"

#ifdef FLAG_USE_CUSTOM_LIBRARY
#include "public/Priority_queue.h"
#define priority_queue Priority_queue
#else
#include <queue>
#define priority_queue priority_queue
#endif
