#ifndef WEATHER_H
#define WEATHER_H

#include <pebble.h>

#define KEY_TEMPERATURE 1
#define KEY_CONDITIONS 2

void translate_weather_response(DictionaryIterator *iterator, void *window);

#endif
