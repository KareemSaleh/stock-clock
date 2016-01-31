#ifndef ARROW_H
#define ARROW_H

#include <pebble.h>

typedef struct {
  GColor color;
  GPathInfo path_info;
} ArrowLayerData;

Layer* getArrowLayer(GRect bounds, GPathInfo path_info, GColor color);

#endif
