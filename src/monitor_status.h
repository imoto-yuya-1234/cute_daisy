#pragma once

#include <pebble.h>
#include <effect_layer/effect_layer.h>
#include "config.h"

Layer *g_window_layer;
GRect g_bounds;
GPoint g_center;
GColor g_connection_color;

static EffectLayer *s_effect_layer;

static void handle_bluetooth(bool connected) {
	if(bluetooth_connection_service_peek()) {
		g_connection_color = GColorWhite;
	} else {
		vibes_double_pulse();
		#if defined(PBL_BW)
		g_connection_color = GColorIslamicGreen;
		#else
		g_connection_color = GColorMelon;
		#endif
	}
}


void handle_battery(BatteryChargeState charge_state) {
	#if defined(PBL_PLATFORM_APLITE)
	#else
	if (battery_state_service_peek().is_plugged && persist_read_bool(KEY_ROTATE)) {
		effect_layer_remove_effect(s_effect_layer);
		effect_layer_add_effect(s_effect_layer,effect_rotate_90_degrees,(void*)true);
	} else {
		effect_layer_remove_effect(s_effect_layer);	
	}
	#endif
}

void init_status() {
	#if defined(PBL_PLATFORM_APLITE)
	#else
	s_effect_layer = effect_layer_create(GRect(0,(g_bounds.size.h-g_bounds.size.w)/2-1,g_bounds.size.w,g_bounds.size.w));
	#endif
}

void deinit_status() {
	#if defined(PBL_PLATFORM_APLITE)
	#else
	effect_layer_destroy(s_effect_layer);
	#endif
}

void valid_rot() {
	#if defined(PBL_PLATFORM_APLITE)
	#else
	handle_battery(battery_state_service_peek());
	layer_add_child(g_window_layer, effect_layer_get_layer(s_effect_layer));
	#endif
}

void status_service_on() {
	bluetooth_connection_service_subscribe(handle_bluetooth);
	battery_state_service_subscribe(handle_battery);
}

void status_service_off() {
	bluetooth_connection_service_unsubscribe();
	battery_state_service_unsubscribe();
}