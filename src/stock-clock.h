#ifndef STOCK_CLOCK_H
#define STOCK_CLOCK_H

#include "arrow.h"
#include "message.h"
#include "weather.h"
#include "layers.h"

#if defined(PBL_RECT)
// 144 x 168
static GPathInfo PATH_INFO_UP = {
  .num_points = 5,
  .points = (GPoint[]) { {10, 60}, {70, 20}, {130, 60}, {130, 140}, {10, 140} }
};
static GPathInfo PATH_INFO_DOWN = {
  .num_points = 5,
  .points = (GPoint[]) { {10, 40}, {130, 40}, {130, 120}, {70, 160}, {10, 120} }
};

#elif defined(PBL_ROUND)
// 180 x 180
static GPathInfo PATH_INFO_UP = {
  .num_points = 5,
  .points = (GPoint[]) { {20, 60}, {90, 20}, {160, 60}, {160, 140}, {20, 140} }
};
static GPathInfo PATH_INFO_DOWN = {
  .num_points = 5,
  .points = (GPoint[]) { {20, 40}, {160, 40}, {160, 120}, {90, 160}, {20, 120} }
};

#endif

#endif
