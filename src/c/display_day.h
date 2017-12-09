#pragma once

#include <pebble.h>
#include "config.h"

Layer *g_window_layer;
GRect g_bounds;
GPoint g_center; 

static Layer *s_date_layer;
static TextLayer *s_day_label, *s_date_label;
static char s_day_buffer[6], s_date_buffer[16];

static void small2large(char *day_buffer) {
	if(strcmp(day_buffer, "Sun") == 0) {
		strcpy(day_buffer, "SUN");
	}
	else if(strcmp(day_buffer, "Mon") == 0) {
		strcpy(day_buffer, "MON");
	}
	else if(strcmp(day_buffer, "Tue") == 0) {
		strcpy(day_buffer, "TUE");
	}
	else if(strcmp(day_buffer, "Wed") == 0) {
		strcpy(day_buffer, "WED");
	}
	else if(strcmp(day_buffer, "Thu") == 0) {
		strcpy(day_buffer, "THU");
	}
	else if(strcmp(day_buffer, "Fri") == 0) {
		strcpy(day_buffer, "FRI");
	}
	else if(strcmp(day_buffer, "Sat") == 0) {
		strcpy(day_buffer, "SAT");
	}
}

static void en2de(char *day_buffer) {
	if(strcmp(day_buffer, "Sun") == 0) {
		strcpy(day_buffer, "SO");
	}
	else if(strcmp(day_buffer, "Mon") == 0) {
		strcpy(day_buffer, "MO");
	}
	else if(strcmp(day_buffer, "Tue") == 0) {
		strcpy(day_buffer, "DI");
	}
	else if(strcmp(day_buffer, "Wed") == 0) {
		strcpy(day_buffer, "MI");
	}
	else if(strcmp(day_buffer, "Thu") == 0) {
		strcpy(day_buffer, "DO");
	}
	else if(strcmp(day_buffer, "Fri") == 0) {
		strcpy(day_buffer, "FR");
	}
	else if(strcmp(day_buffer, "Sat") == 0) {
		strcpy(day_buffer, "SA");
	}
}

static void en2fr(char *day_buffer) {
	if(strcmp(day_buffer, "Sun") == 0) {
		strcpy(day_buffer, "DIM");
	}
	else if(strcmp(day_buffer, "Mon") == 0) {
		strcpy(day_buffer, "LUN");
	}
	else if(strcmp(day_buffer, "Tue") == 0) {
		strcpy(day_buffer, "MAR");
	}
	else if(strcmp(day_buffer, "Wed") == 0) {
		strcpy(day_buffer, "MER");
	}
	else if(strcmp(day_buffer, "Thu") == 0) {
		strcpy(day_buffer, "JEU");
	}
	else if(strcmp(day_buffer, "Fri") == 0) {
		strcpy(day_buffer, "VEN");
	}
	else if(strcmp(day_buffer, "Sat") == 0) {
		strcpy(day_buffer, "SAM");
	}
}

static void date_update_proc(Layer *layer, GContext *ctx) {
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	
	strftime(s_day_buffer, sizeof(s_day_buffer), "%a", t);
		
	char buffer[3];
	persist_read_string(KEY_LANG, buffer, sizeof(buffer));
	if(strcmp(buffer, "en") == 0) {
		small2large(s_day_buffer);
	}
	else if(strcmp(buffer, "fr") == 0) {
		en2fr(s_day_buffer);
	}
	else if(strcmp(buffer, "de") == 0) {
		en2de(s_day_buffer);
	}
	
	text_layer_set_text(s_day_label, s_day_buffer);
	
  strftime(s_date_buffer, sizeof(s_date_buffer), "%Y/%m/%d", t);
	text_layer_set_text(s_date_label, s_date_buffer);
	
	graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(GColorChromeYellow, GColorLightGray));
	graphics_context_set_stroke_color(ctx , PBL_IF_COLOR_ELSE(GColorChromeYellow, GColorWhite));
	graphics_fill_rect(ctx, GRect(2, 147, 140, 20), 3, GCornersAll);
}

void display_day() {
	#if defined(PBL_RECT)
	layer_set_update_proc(s_date_layer, date_update_proc);
	layer_add_child(g_window_layer, s_date_layer);

	text_layer_set_text(s_day_label, s_day_buffer);
	text_layer_set_background_color(s_day_label, GColorClear);
	text_layer_set_text_color(s_day_label, GColorWhite);
	text_layer_set_font(s_day_label, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	layer_add_child(s_date_layer, text_layer_get_layer(s_day_label));

	text_layer_set_text(s_date_label, s_date_buffer);
	text_layer_set_background_color(s_date_label, GColorClear);
	text_layer_set_text_color(s_date_label, GColorWhite);
	text_layer_set_font(s_date_label, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	layer_add_child(s_date_layer, text_layer_get_layer(s_date_label));
	#endif
}

void init_day() {
	s_date_layer = layer_create(g_bounds);
	
	s_day_label = text_layer_create(GRect(107, 140, 32, 30));
	s_date_label = text_layer_create(GRect(5, 140, 140, 30));
	
	s_day_buffer[0] = '\0';
	s_date_buffer[0] = '\0';
}

void deinit_day() {	
	layer_destroy(s_date_layer);
	text_layer_destroy(s_day_label);
	text_layer_destroy(s_date_label);
}