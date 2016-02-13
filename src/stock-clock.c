#include <pebble.h>
#include "stock-clock.h"

static Window *s_main_window;
static GFont s_time_font, s_stock_font, s_weather_font;
Layer *s_up_arrow_layer = NULL, *s_down_arrow_layer = NULL;
TextLayer *s_time_layer = NULL, *s_stock_layer = NULL, *s_weather_layer = NULL;

//@todo Date layer and bitmap layer for date style.
//@todo create pointer to be sent as context to all message events?

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
 * Loads the main window
 */
static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  int stock_layer_w = bounds.size.w/2;
  int stock_layer_x = stock_layer_w/2;

  // Log our bounds For now
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[DEBUG] Bounds: x:%d y:%d w:%d h:%d",
    bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h);

  // Create the TextLayers with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  s_stock_layer = text_layer_create(
      GRect(stock_layer_x, PBL_IF_ROUND_ELSE(98, 92), stock_layer_w, 25));
  s_weather_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(125, 120), bounds.size.w, 25));

  // Set main window colour
  window_set_background_color(s_main_window, GColorBlack);

  // Time layer
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Stock layer
  text_layer_set_background_color(s_stock_layer, GColorClear);
  text_layer_set_text_color(s_stock_layer, GColorWhite);
  text_layer_set_text(s_stock_layer, "+0.0");
  text_layer_set_text_alignment(s_stock_layer, GTextAlignmentCenter);

  // Weather layer
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text(s_weather_layer, "--°C");

  // Create fonts for above layers
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ALARM_CLOCK_36));
  s_stock_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SUBWAY_TICKER_24));
  s_weather_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_20));

  // Apply to TextLayers
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_stock_layer, s_stock_font);
  text_layer_set_font(s_weather_layer, s_weather_font);

  // Setup stock direction layers
  s_up_arrow_layer = getArrowLayer(bounds, PATH_INFO_UP, GColorMayGreen);
  s_down_arrow_layer = getArrowLayer(bounds, PATH_INFO_DOWN, GColorJazzberryJam);

  // Initially hide the arrow layers
  layer_set_hidden(s_up_arrow_layer, true);
  layer_set_hidden(s_down_arrow_layer, true);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, s_up_arrow_layer);
  layer_add_child(window_layer, s_down_arrow_layer);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_stock_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));

  APP_LOG(APP_LOG_LEVEL_DEBUG, "DONE SETUP LAYER :%p", s_weather_layer);
}

/**
 * Unloads the main window
 */
static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_stock_layer);
  text_layer_destroy(s_weather_layer);

  // Destroy ticker graphic layer and path
  layer_destroy(s_up_arrow_layer);
  layer_destroy(s_down_arrow_layer);

  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_stock_font);
  fonts_unload_custom_font(s_weather_font);
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

  // Register callbacks for communicating with phone
  // TODO: We need a giant list of all layers passed here. Can't get children AFAIK.
  register_message_handlers(s_main_window);

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
