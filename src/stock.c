#include "stock.h"
void translate_stock_response(DictionaryIterator *iterator, void *layer, void *upWindow, void *downWindow) {
  // Store incoming information
  static char stock_buffer[8];
  static char percent_buffer[8];
  char *stock_change;

  // Read tuples for data
  Tuple *change_tuple = dict_find(iterator, KEY_STOCKS_TOTAL_CHANGE);
  Tuple *percent_tuple = dict_find(iterator, KEY_STOCKS_TOTAL_PERCENT);

  // If all data is available, use it
  if (change_tuple && percent_tuple) {
    stock_change = change_tuple->value->cstring;
    snprintf(stock_buffer, sizeof(stock_buffer), "%s", stock_change);
    snprintf(percent_buffer, sizeof(percent_buffer), "%s", percent_tuple->value->cstring);

    // TODO: Set persistent storage and then mark as dirty rather than set text.
    if(layer != NULL) {
      text_layer_set_text(layer, stock_buffer);
    }
  }
}
