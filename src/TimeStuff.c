#include <pebble.h>
//#include <pebble_fonts.h>

#include "TimeStuff.h"
//#include "common.h"


void update_display_time(struct tm *tick_time) 
{
  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  char *time_format;

  if (clock_is_24h_style()) 
  {
    time_format = "%R";
  } else 
  {
    time_format = "%I:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) 
  {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(text_oclock_layer, time_text);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) 
{
	update_display_time(tick_time);
	// if(units_changed & DAY_UNIT) {update_display_date(tick_time);}
}

