#include "message.h"
#include "weather.h"
#include "stock.h"
#include "layers.h"

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  Tuple *message_type_tuple = dict_find(iterator, KEY_TYPE);
  MessageType message_type = message_type_tuple->value->int32;

  // Find out what type of message we recieved
  APP_LOG(APP_LOG_LEVEL_DEBUG, "[DEBUG] Message Type Recieved: %i", message_type);
  switch(message_type) {
    case WEATHER :
      translate_weather_response(iterator, s_weather_layer);
      break;
    case STOCK :
      translate_stock_response(iterator, s_stock_layer, s_up_arrow_layer, s_down_arrow_layer);
      break;
    default :
      APP_LOG(APP_LOG_LEVEL_ERROR, "[ERROR] Missing message type.");
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

/**
 * Called when registering all the App Message callbacks
 */
void register_message_handlers() {

  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}
