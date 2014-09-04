#include <pebble.h>
#include <pebble_fonts.h>

#include "TimeStuff.h"
#include "Common.h"

#define NUMBER_OF_OPTIONS 3
enum e_options {
	TEXT_SELECTED = 0,
	CLOCK_DISP_TYPE = 1,
	DISPLAY_YEAR = 2
};
int options[NUMBER_OF_OPTIONS];

static Window *window;
//static TextLayer *text_layer;
TextLayer *text_beer_layer;
TextLayer *text_oclock_layer;
TextLayer *text_time_layer;
TextLayer *text_day_layer;
TextLayer *text_date_layer;

Layer *line_layer;

// strings
static char s_oclock[] =   {"O'Clock"};
static char s_scotch[] =   {"Scotch"};
static char s_beer[] =     {"Beer"};
static char s_wine[] =     {"Wine"};
static char s_happy[] =    {"Happy"};
static char s_hour[] =     {"Hour"};


#define MAX_STRING (3)
#define NUM_STRINGS (MAX_STRING + 1)
static char* text_list[NUM_STRINGS][2] =
{
  {s_wine,      s_oclock},   //  0
  {s_scotch,    s_oclock},   //  1
  {s_beer,      s_oclock},   //  2
  {s_happy,     s_hour},     //  3
};
  
static GFont beer_font;
static GFont big_time_font;
static GFont small_font;
int alt_count = 0;

// mode stuff
int mode_clock = 1;
int text_select = 1;

#define FONT_BEER_TIME RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_36
#define FONT_BIG_TIME  RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49
#define FONT_SMALL     RESOURCE_ID_FONT_ROBOTO_CONDENSED_21

void swap_to_oclock()
{
  // put new text in
  text_layer_set_text(text_beer_layer, text_list[text_select][0]);
  text_layer_set_text(text_oclock_layer, text_list[text_select][1]);
  
  layer_set_frame((Layer*)text_oclock_layer, OCLOCK_RECT);
  layer_set_frame((Layer*)text_time_layer, TIME_HIDE_RECT);

  layer_set_frame((Layer*)text_beer_layer, SCOTCH_RECT);
  layer_set_frame((Layer*)text_day_layer, DAY_HIDE_RECT);
  layer_set_frame((Layer*)text_date_layer, DATE_HIDE_RECT);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  if (mode_clock)  // was clock - now get and display text
  {
    mode_clock = 0;  // toggle to text
    options[TEXT_SELECTED] = persist_read_int(TEXT_SELECTED);
    text_select = options[TEXT_SELECTED]; // get stored mode
    swap_to_oclock();
  }
  else  // was displaying text - save choice then display clock
  {
    mode_clock = 1;  // toggle back to clock 
    
    options[TEXT_SELECTED] = text_select; // set stored mode
    persist_write_int(TEXT_SELECTED, options[TEXT_SELECTED]);
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    handle_minute_tick(current_time, SECOND_UNIT);
  }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  if (!mode_clock)
  {
    if (text_select)
    {
      text_select--;
    }
    else
    {
      text_select = MAX_STRING;
    }
    swap_to_oclock();
  }
}
 
static void down_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  if (!mode_clock)
  {
    if (text_select == MAX_STRING)
    {
      text_select = 0;
    }
    else
    {
      text_select++;
    }
    swap_to_oclock();
  }
}

void line_layer_update_callback(Layer *layer, GContext* ctx) 
{
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static void click_config_provider(void *context) 
{
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) 
{
  Layer *window_layer = window_get_root_layer(window);
  //GRect bounds = layer_get_bounds(window_layer);

  // Get our fonts
  beer_font = fonts_load_custom_font(resource_get_handle(FONT_BEER_TIME));
  big_time_font = fonts_load_custom_font(resource_get_handle(FONT_BIG_TIME));
  small_font = fonts_load_custom_font(resource_get_handle(FONT_SMALL));
  // big_font = fonts_load_custom_font(resource_get_handle(FONT_BIG_TIME));
  // seconds_font = fonts_load_custom_font(resource_get_handle(FONT_SECONDS));
  // laps_font = fonts_load_custom_font(resource_get_handle(FONT_LAPS));

  // text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_beer_layer = text_layer_create(SCOTCH_RECT);
  text_layer_set_text(text_beer_layer, s_scotch);
  //text_layer_set_text_alignment(text_beer_layer, GTextAlignmentCenter);
  text_layer_set_text_color(text_beer_layer, GColorWhite);
  text_layer_set_background_color(text_beer_layer, GColorClear);
  // text_layer_set_font(text_beer_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_36)));
  text_layer_set_font(text_beer_layer, beer_font);
  layer_add_child(window_layer, text_layer_get_layer(text_beer_layer));
  
  // text_beer_layer
  /*
  text_beer_layer = text_layer_create(GRect(8, 52, 144-8, 168-52));
  text_layer_set_text(text_beer_layer, "Beer");
  text_layer_set_text_alignment(text_beer_layer, GTextAlignmentCenter);
  text_layer_set_text_color(text_beer_layer, GColorWhite);
  text_layer_set_background_color(text_beer_layer, GColorClear);
  text_layer_set_font(text_beer_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_36)));
  */
  // layer_add_child(window_layer, text_layer_get_layer(text_beer_layer));
  
  // OClock layer
  //text_oclock_layer = text_layer_create(GRect(8, 97, 144-8, 168-97));
  text_oclock_layer = text_layer_create(OCLOCK_RECT);
  text_layer_set_text(text_oclock_layer, s_oclock);
  text_layer_set_text_color(text_oclock_layer, GColorWhite);
  text_layer_set_background_color(text_oclock_layer, GColorClear);
  // text_layer_set_font(text_oclock_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_36)));
  text_layer_set_font(text_oclock_layer, beer_font);
  layer_add_child(window_layer, text_layer_get_layer(text_oclock_layer));

  // big time layer
  text_time_layer = text_layer_create(TIME_HIDE_RECT);
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, big_time_font);
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

  // day text layer
	text_day_layer = text_layer_create(DAY_HIDE_RECT);
  text_layer_set_text_color(text_day_layer, GColorWhite);
  text_layer_set_background_color(text_day_layer, GColorClear);
  text_layer_set_font(text_day_layer, small_font);
  layer_add_child(window_layer, text_layer_get_layer(text_day_layer));

  // date text layer
  text_date_layer = text_layer_create(DATE_HIDE_RECT);
  text_layer_set_text_color(text_date_layer, GColorWhite);
  text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_font(text_date_layer, small_font);
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

  // Line layer
  GRect line_frame = LINE_RECT;  //GRect(8, 97, 139, 2);
  line_layer = layer_create(line_frame);
  layer_set_update_proc(line_layer, line_layer_update_callback);
  layer_add_child(window_layer, line_layer);


}

static void window_unload(Window *window) 
{
  //text_layer_destroy(text_layer);
  text_layer_destroy(text_beer_layer);
  layer_destroy(line_layer);
  text_layer_destroy(text_oclock_layer);
  text_layer_destroy(text_time_layer);
  text_layer_destroy(text_day_layer);
  text_layer_destroy(text_date_layer);
  
  fonts_unload_custom_font(small_font);
  fonts_unload_custom_font(big_time_font);
	fonts_unload_custom_font(beer_font);
}

static void handle_init(void) {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_fullscreen(window, true);
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) 
  {
	.load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true /* Animated */);
	//tick_timer_service_subscribe(SECOND_UNIT, handle_minute_tick);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  // tick_timer_service_unsubscribe();

}

void load_options_from_persistent_storage(void) {
	// default values
	options[TEXT_SELECTED] = 1;
	options[CLOCK_DISP_TYPE] = 0;
	options[DISPLAY_YEAR] = 2;
	
	// load values (or set them, if they don't already exist)
	if(persist_exists(TEXT_SELECTED))		{options[TEXT_SELECTED]			= persist_read_int(TEXT_SELECTED);}		else {persist_write_int(TEXT_SELECTED,   options[TEXT_SELECTED]);}
	if(persist_exists(CLOCK_DISP_TYPE))	{options[CLOCK_DISP_TYPE] 	= persist_read_int(CLOCK_DISP_TYPE);}	else {persist_write_int(CLOCK_DISP_TYPE,	options[CLOCK_DISP_TYPE]);}
	if(persist_exists(DISPLAY_YEAR))	  {options[DISPLAY_YEAR]	    = persist_read_int(DISPLAY_YEAR);}	  else {persist_write_int(DISPLAY_YEAR,	  options[DISPLAY_YEAR]);}
}

static void handle_deinit(void) 
{
  window_destroy(window);
}

int main(void) 
{
  handle_init();
  load_options_from_persistent_storage();
  app_event_loop();
  handle_deinit();
}


/*
    // hold on this for 1 seconds - then switch back
    alt_count = 1;
    tick_timer_service_unsubscribe();
    tick_timer_service_subscribe(SECOND_UNIT, handle_minute_tick);
    tick_timer_service_subscribe()
*/

