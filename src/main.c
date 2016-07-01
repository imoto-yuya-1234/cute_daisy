#include "pebble.h"
#include "config.h"
#include "display_time.h"
#include "display_day.h"
#include "ornament.h"
#include "monitor_status.h"

extern Layer *g_window_layer;
extern GRect g_bounds;
extern GPoint g_center;

static Window *s_window;
static BitmapLayer *s_daisy_layer;
static GBitmap *s_daisy_bitmap;

static void window_load(Window *window) {
	g_window_layer = window_get_root_layer(window);
  g_bounds = layer_get_bounds(g_window_layer);
  g_center = grect_center_point(&g_bounds);
	
	initialize_value();
	
	s_daisy_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DAISY);
	s_daisy_layer = bitmap_layer_create(g_bounds);
	bitmap_layer_set_bitmap(s_daisy_layer, s_daisy_bitmap);
	layer_add_child(g_window_layer, bitmap_layer_get_layer(s_daisy_layer));
	
	init_ornament();
	display_ticks();
	
	//init_day();
	//display_day();
	
	init_time();
	display_time();
}

static void window_unload(Window *window) {
	bitmap_layer_destroy(s_daisy_layer);
  gbitmap_destroy(s_daisy_bitmap);
	//deinit_day();
	deinit_time();
	deinit_ornament();
}

static void load_window() {
	s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}

void reload_window() {
	window_stack_remove(s_window, true);
	load_window();
	
  time_service_off();
	time_service_on();
}

static void init() {
	load_window();
	
  config_service();
	time_service_on();
	status_service_on();
}

static void deinit() {
	time_service_off();
	status_service_off();
  window_destroy(s_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
