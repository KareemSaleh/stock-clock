#ifndef ARROW_H
#define ARROW_H

#include <pebble.h>

#if defined(PBL_RECT)
// 144 x 168
static GPathInfo UP_PATH_INFO = {
  .num_points = 5,
  .points = (GPoint[]) { {10, 60}, {70, 20}, {130, 60}, {130, 140}, {10, 140} }
};
static GPathInfo DOWN_PATH_INFO = {
  .num_points = 5,
  .points = (GPoint[]) { {10, 20}, {130, 20}, {130, 100}, {70, 140}, {10, 100} }
};

#elif defined(PBL_ROUND)
// 180 x 180
static GPathInfo UP_PATH_INFO = {
  .num_points = 5,
  .points = (GPoint[]) { {20, 80}, {90, 40}, {160, 80}, {160, 160}, {20, 160} }
};
static GPathInfo DOWN_PATH_INFO = {
  .num_points = 5,
  .points = (GPoint[]) { {20, 40}, {160, 40}, {160, 120}, {90, 160}, {20, 120} }
};

#endif


// GPath describes the arrow shape
static GPath *s_path;

static void layer_update_proc(Layer *layer, GContext *ctx);
Layer* getArrowLayer(GRect bounds, GPathInfo path);

#endif
