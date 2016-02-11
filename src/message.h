#ifndef MESSAGE_H
#define MESSAGE_H

#include <pebble.h>

#define KEY_TYPE 0

typedef enum {
  WEATHER,
  STOCK
} MessageType;

void registerMessage(Window *context);

#endif
