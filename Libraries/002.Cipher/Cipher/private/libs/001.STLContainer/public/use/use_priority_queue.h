#pragma once

#include "001.STLContainer_Setting.h"

#ifdef FLAG_USE_PRIORITY_QUEUE
#include "src/Priority_queue.h"
#else
#include <queue>
#define Priority_queue priority_queue
#endif
