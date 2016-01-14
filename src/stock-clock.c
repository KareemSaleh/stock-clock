#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer, *s_stock_layer;
static GFont s_time_font, s_stock_font;

static Layer *s_path_layer;

// GPath describes the shape
static GPath *s_path;
static GPathInfo PATH_INFO = {
  .num_points = 5,
  .points = (GPoint[]) { {10, 60}, {70, 20}, {130, 60}, {130, 140}, {10, 140} }
};

/**
 * Updates the text layer with current time.
 */
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

/**
 * Handles time "tick" events.
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

/**
 * @TODO clean up this function
 * @param layer [description]
 * @param ctx   [description]
 */
static void layer_update_proc(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorFromRGB(0, 255, 0));

  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_path);
}

/**
 * Loads the main window
 */
static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  int stock_layer_w = bounds.size.w/2;
  int stock_layer_x = stock_layer_w/2;

  // Create the TextLayers with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  s_stock_layer = text_layer_create(
      GRect(stock_layer_x, PBL_IF_ROUND_ELSE(118, 112), stock_layer_w, 25));

  // Set main window colour
  window_set_background_color(s_main_window, GColorOrange);

  // time layer
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // stock layer
  text_layer_set_background_color(s_stock_layer, GColorClear);
  text_layer_set_text_color(s_stock_layer, GColorBlack);
  text_layer_set_text(s_stock_layer, "+0.0");
  text_layer_set_text_alignment(s_stock_layer, GTextAlignmentCenter);

  // Create fonts for above layers
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ALARM_CLOCK_48));
  s_stock_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SUBWAY_TICKER_24));

  // Apply to TextLayers
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_stock_layer, s_stock_font);

  // Setup stock direction layers
  // Create GPath object
  s_path = gpath_create(&PATH_INFO);

  // Create Layer that the path will be drawn on
  s_path_layer = layer_create(bounds);
  layer_set_update_proc(s_path_layer, layer_update_proc);


  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, s_path_layer);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_stock_layer));
}

/**
 * Unloads the main window
 */
static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_stock_layer);

  // Destroy ticker graphic layer and path
  layer_destroy(s_path_layer);
  gpath_destroy(s_path);

  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_stock_font);
}

/**
 * Initialize the watchface
 */
static void init() {
  // Create the Window
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Push to the stack, animated
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();
}

/**
 * Deinitialize the watchface
 */
static void deinit() {
  // Destroy the TextLayer
  text_layer_destroy(s_time_layer);
  // Destroy the Window
  window_destroy(s_main_window);
}

/**
 * Main event loop
 */
int main(void) {
  // Initialize the app
  init();

  // Wait for app events
  app_event_loop();

  // Deinitialize the app
  deinit();

  // App finished without error
  return 0;
}
