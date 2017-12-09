#pragma once

#include <pebble.h>
#include "config.h"
#include "frame.h"
#include "monitor_status.h"

Layer *g_window_layer;
GRect g_bounds;
GPoint g_center;
GColor g_connection_color;

static Layer *s_hands_layer, *s_second_layer;
static GPath *s_minute_arrow, *s_hour_arrow, *s_second_decorate_shape;
static GPoint s_time_center;

static void hands_update_proc(Layer *layer, GContext *ctx) {
	// hour/minute/second hand
	time_t now = time(NULL);
  struct tm *t = localtime(&now);
	int16_t hour = t->tm_hour;
	int16_t min = t->tm_min;
	
	#if defined(PBL_PLATFORM_APLITE)
	#else
	if (battery_state_service_peek().is_plugged && persist_read_bool(KEY_ROTATE)) {
		hour += 3;
		min += 15;
	}
	#endif
	// minute/hour hand
  graphics_context_set_fill_color(ctx, g_connection_color);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_stroke_width(ctx, 1);

  gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * min / 60);
  gpath_draw_filled(ctx, s_minute_arrow);
  gpath_draw_outline(ctx, s_minute_arrow);

  gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((hour % 12) * 6) + (min / 10))) / (12 * 6));
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);
		
	// dot in the middle
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_fill_circle(ctx, s_time_center, 4);
}

static void second_update_proc(Layer *layer, GContext *ctx) {
	time_t now = time(NULL);
  struct tm *t = localtime(&now);
	int16_t sec = t->tm_sec;
	
	#if defined(PBL_PLATFORM_APLITE)
	#else
	if (battery_state_service_peek().is_plugged && persist_read_bool(KEY_ROTATE)) {
		sec += 15;
	}
	#endif
	
	// second hand
  const int16_t second_hand_length = PBL_IF_ROUND_ELSE(g_bounds.size.w / 2 - 10, g_bounds.size.w / 2 - 10);
  int32_t second_angle = TRIG_MAX_ANGLE * sec / 60;
  GPoint second_hand_start = {
    .x = (int16_t)(sin_lookup(second_angle) * (int32_t)(second_hand_length - 3) / TRIG_MAX_RATIO) + s_time_center.x,
    .y = (int16_t)(-cos_lookup(second_angle) * (int32_t)(second_hand_length - 3) / TRIG_MAX_RATIO) + s_time_center.y,
  };
	GPoint second_hand_end = {
    .x =  - (int16_t)(sin_lookup(180 + second_angle) * 15 / TRIG_MAX_RATIO) + s_time_center.x,
    .y =  - (int16_t)(-cos_lookup(180 + second_angle) * 15 / TRIG_MAX_RATIO) + s_time_center.y,
  };
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorFashionMagenta, GColorWhite));
	graphics_context_set_stroke_width(ctx, 1);
  graphics_draw_line(ctx, second_hand_start, second_hand_end);
	
	// second hand decorate
	gpath_move_to(s_second_decorate_shape, second_hand_end);
	graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorFashionMagenta, GColorWhite));
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorFashionMagenta, GColorWhite));
	gpath_rotate_to(s_second_decorate_shape, second_angle);
	gpath_draw_filled(ctx, s_second_decorate_shape);
	gpath_draw_outline(ctx, s_second_decorate_shape);
	
  // dot in the middle
  graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorFashionMagenta, GColorWhite));
  graphics_fill_circle(ctx, s_time_center, 2);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(g_window_layer);
}

void display_time() {
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(g_window_layer, s_hands_layer);
	
	layer_set_update_proc(s_second_layer, second_update_proc);
	layer_add_child(g_window_layer, s_second_layer);
}

void init_time() {
	s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
	
	char buffer[6];
	persist_read_string(KEY_DECORATION, buffer, sizeof(buffer));
	if(strcmp(buffer, "heart") == 0) {
		s_second_decorate_shape = gpath_create(&HEART_POINTS);
	}
	else if(strcmp(buffer, "star") == 0) {
		s_second_decorate_shape = gpath_create(&STAR_POINTS);
	}

	s_time_center = PBL_IF_ROUND_ELSE(GPoint(g_center.x, g_center.y), GPoint(g_center.x, g_center.y));
  gpath_move_to(s_minute_arrow, s_time_center);
  gpath_move_to(s_hour_arrow, s_time_center);
	
  s_hands_layer = layer_create(g_bounds);
	s_second_layer = layer_create(g_bounds);
}

void deinit_time() {
	gpath_destroy(s_minute_arrow);
  gpath_destroy(s_hour_arrow);
	gpath_destroy(s_second_decorate_shape);
  layer_destroy(s_hands_layer);
	layer_destroy(s_second_layer);
}

void time_service_on() {
  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

void time_service_off() {
  tick_timer_service_unsubscribe();
}