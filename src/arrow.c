#include "arrow.h"

/**
 * Callback for processing arrow graphics layer.
 */
static void layer_update_proc(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorMalachite);

  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_path);
}

Layer* getArrowLayer(GRect bounds, GPathInfo path) {
  Layer *arrow_layer;

  s_path = gpath_create(&path);

  // Create Layer that the path will be drawn on
  arrow_layer = layer_create(bounds);
  layer_set_update_proc(arrow_layer, layer_update_proc);

  return arrow_layer;
}
