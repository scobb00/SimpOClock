#include <pebble.h>
#include <pebble_fonts.h>

static Window *window;
//static TextLayer *text_layer;
TextLayer *text_beer_layer;
TextLayer *text_oclock_layer;
Layer *line_layer;

static GFont beer_font;

#define FONT_BEER_TIME RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_36


static void select_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  text_layer_set_text(text_beer_layer, "Scotch");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  text_layer_set_text(text_beer_layer, "Wine");
}
 
static void down_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  text_layer_set_text(text_beer_layer, "Beer");
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
  GRect bounds = layer_get_bounds(window_layer);

  // Get our fonts
  beer_font = fonts_load_custom_font(resource_get_handle(FONT_BEER_TIME));
  // big_font = fonts_load_custom_font(resource_get_handle(FONT_BIG_TIME));
  // seconds_font = fonts_load_custom_font(resource_get_handle(FONT_SECONDS));
  // laps_font = fonts_load_custom_font(resource_get_handle(FONT_LAPS));

  // text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_beer_layer = text_layer_create(GRect(8, 52, 144-8, 168-52));
  text_layer_set_text(text_beer_layer, "Scotch");
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
  text_oclock_layer = text_layer_create(GRect(8, 97, 144-8, 168-97));
  text_layer_set_text(text_oclock_layer, "O'Clock");
  text_layer_set_text_color(text_oclock_layer, GColorWhite);
  text_layer_set_background_color(text_oclock_layer, GColorClear);
  // text_layer_set_font(text_oclock_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_36)));
  text_layer_set_font(text_oclock_layer, beer_font);
  layer_add_child(window_layer, text_layer_get_layer(text_oclock_layer));

  // Line layer
  GRect line_frame = GRect(8, 97, 139, 2);
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
//	text_layer_destroy(text_beer_layer);
//	fonts_unload_custom_font(big_font);
//	fonts_unload_custom_font(seconds_font);
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
}

static void handle_deinit(void) 
{
  window_destroy(window);
}

int main(void) 
{
  handle_init();
  app_event_loop();
  handle_deinit();
}
