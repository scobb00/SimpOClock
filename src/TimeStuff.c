#include <pebble.h>
//#include <pebble_fonts.h>

#include "TimeStuff.h"
#include "Common.h"


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

  // text_layer_set_text(text_oclock_layer, time_text);
  text_layer_set_text(text_time_layer, time_text);
  layer_set_frame((Layer*)text_oclock_layer, OCLOCK_HIDE_RECT);
  layer_set_frame((Layer*)text_time_layer, TIME_RECT);
}


void update_display_date(struct tm *tick_time) {
  // Need to be static because they're used by the system later.
  static char date_text[] = "Xxxxxxxxx 00";		// reserve enough space for full month name
  static char year_text[] = "0000";
	static char day_text[] = "Xxxxxxxxx";	// reserve enough space for full weekday name
	static char week_text[] = "w00";

	// if(options[DISPLAY_WEEK]) 
  if (0)
  {
    /*
		// shortened day_text
		strftime(day_text, sizeof(day_text), "%a", tick_time);
		
		//int week = 87; // PLACEHOLDER VALUE!  Should be calculated - look at code from simplicity_plus_wo_year
		//sprintf(week_text, "w%d", week);
		strftime(week_text, sizeof(week_text), "%w", tick_time);
		text_layer_set_text(text_week_layer, week_text);
    */
	} 
  else 
  {
		// full day_text
		strftime(day_text, sizeof(day_text), "%A", tick_time);
	}
	text_layer_set_text(text_day_layer, day_text);
  layer_set_frame((Layer*)text_beer_layer, SCOTCH_HIDE_RECT);
  layer_set_frame((Layer*)text_day_layer, DAY_RECT);
	
//#if SHOW_WEEK
//	// shortened day_text
//  strftime(day_text, sizeof(day_text), "%a", tick_time);
//	
//	// Too bad sprintf doesn't work with this version of the Pebble SDK...
//	// sprintf(week_text, "w%d", week);
//	strcpy(week_text, "");
//
//	itoa(week, temp_text);
//	strcat(week_text, "P");
//	strcat(week_text, temp_text);
//
//	strftime(week_text, sizeof(week_text), "%w", tick_time);
//  text_layer_set_text(text_week_layer, week_text);
//#else
//	// full day_text
//  strftime(day_text, sizeof(day_text), "%A", tick_time);
//#endif
//	text_layer_set_text(text_day_layer, day_text);
	
	// if(options[DISPLAY_YEAR]) 
  if(0)
  {
/*
		// short month
		strftime(date_text, sizeof(date_text), "%b %e", tick_time);
		
		strftime(year_text, sizeof(year_text), "%Y", tick_time);
		text_layer_set_text(text_year_layer, year_text);
*/
	} 
  else 
  {
		// full month
		strftime(date_text, sizeof(date_text), "%B %e", tick_time);
		// short month
		//strftime(date_text, sizeof(date_text), "%B %e", tick_time);
	}
  text_layer_set_text(text_date_layer, date_text);
  // scotch layer already hiden above
  layer_set_frame((Layer*)text_date_layer, DATE_RECT);

  
//#if SHOW_YEAR
//	// short month
//  strftime(date_text, sizeof(date_text), "%b %e", tick_time);
//	
//  strftime(year_text, sizeof(year_text), "%Y", tick_time);
//  text_layer_set_text(text_year_layer, year_text);
//#else
//	// full month
//	strftime(date_text, sizeof(date_text), "%B %e", tick_time);
//#endif
//  text_layer_set_text(text_date_layer, date_text);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) 
{
  // see if update is ok
  if (alt_count > 0)
  {
    alt_count--;
       
    // if done counting down, go back to minutes
    if (alt_count == 0)
    {
      tick_timer_service_unsubscribe();
      //tick_timer_service_subscribe(SECOND_UNIT, handle_minute_tick);
      tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
    }
  }
  else
  {
	  update_display_time(tick_time);
	  // if(units_changed & DAY_UNIT) {update_display_date(tick_time);}
    update_display_date(tick_time);
  }
}

