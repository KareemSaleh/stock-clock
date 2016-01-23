#include "arrow.h"

/**
 * Callback for processing arrow graphics layer.
 */
static void layer_update_proc(Layer *layer, GContext *ctx) {
  // Retrieve this layer's data
  ArrowLayerData *layer_data = (ArrowLayerData*)layer_get_data(layer);

  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, layer_data->color);

  // Draw the filled shape in above color
  gpath_draw_filled(ctx, gpath_create(&layer_data->path_info));
}

Layer* getArrowLayer(GRect bounds, GPathInfo path_info, GColor color) {
  Layer *arrow_layer;

  // Create Layer that the path will be drawn on
  arrow_layer = layer_create_with_data(bounds, sizeof(ArrowLayerData));
  layer_set_update_proc(arrow_layer, layer_update_proc);

  // Store initial values
  ArrowLayerData *layer_data = (ArrowLayerData*)layer_get_data(arrow_layer);
  layer_data->color = color;
  layer_data->path_info = path_info;

  return arrow_layer;
}
