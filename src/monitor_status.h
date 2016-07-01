#pragma once

#include <pebble.h>
#include "config.h"

Layer *g_window_layer;
GRect g_bounds;
GPoint g_center;
GColor g_connection_color;

static void handle_bluetooth(bool connected) {
	if(bluetooth_connection_service_peek()) {
		g_connection_color = GColorWhite;
	} else {
		vibes_double_pulse();
		g_connection_color = GColorMelon;
	}
}

void handle_battery(BatteryChargeState charge_state) {
	//int battery_level = charge_state.charge_percent;
}

void status_service_on() {
	bluetooth_connection_service_subscribe(handle_bluetooth);
	battery_state_service_subscribe(handle_battery);
}

void status_service_off() {
	bluetooth_connection_service_unsubscribe();
	battery_state_service_unsubscribe();
}