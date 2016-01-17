#ifndef ARROW_H
#define ARROW_H

#include <pebble.h>

static GPathInfo UP_PATH_INFO = {
  .num_points = 5,
  .points = (GPoint[]) { {10, 60}, {70, 20}, {130, 60}, {130, 140}, {10, 140} }
};
static GPathInfo DOWN_PATH_INFO = {
  .num_points = 5,
  .points = (GPoint[]) { {10, 20}, {130, 20}, {130, 100}, {70, 140}, {10, 100} }
};

// GPath describes the arrow shape
static GPath *s_path;

static void layer_update_proc(Layer *layer, GContext *ctx);
Layer* getArrowLayer(GRect bounds, GPathInfo path);

#endif
