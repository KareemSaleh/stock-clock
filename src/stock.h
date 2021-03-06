#ifndef STOCK_H
#define STOCK_H

#include <pebble.h>

#define KEY_STOCKS_TOTAL_CHANGE 3
#define KEY_STOCKS_TOTAL_PERCENT 4
#define KEY_STOCKS_TOTAL_DIRECTION_IS_UP 5

void translate_stock_response(DictionaryIterator *iterator, void *layer, void *upWindow, void *downWindow);

#endif
