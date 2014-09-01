#pragma once

extern TextLayer *text_beer_layer;
extern TextLayer *text_oclock_layer;

void update_display_time(struct tm *tick_time); 
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed);

