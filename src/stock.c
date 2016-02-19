#include "stock.h"

void translate_stock_response(DictionaryIterator *iterator, void *layer, void *upLayer, void *downLayer) {
  // Store incoming information
  static char stock_buffer[8];
  static char percent_buffer[8];

  // Read tuples for data
  Tuple *change_tuple = dict_find(iterator, KEY_STOCKS_TOTAL_CHANGE);
  Tuple *percent_tuple = dict_find(iterator, KEY_STOCKS_TOTAL_PERCENT);
  Tuple *direction_tuple = dict_find(iterator, KEY_STOCKS_TOTAL_DIRECTION_IS_UP);

  // If all data is available, use it
  if (change_tuple && percent_tuple) {
    bool isUp = (bool) direction_tuple->value->int32;
    char *format = (isUp) ? "+%s" : "%s";
    snprintf(stock_buffer, sizeof(stock_buffer), format, change_tuple->value->cstring);
    snprintf(percent_buffer, sizeof(percent_buffer), format, percent_tuple->value->cstring);

    // TODO: Set persistent storage and then mark as dirty rather than set text.
    text_layer_set_text(layer, stock_buffer);
    layer_set_hidden(upLayer, !isUp);
    layer_set_hidden(downLayer, isUp);
  }
}
