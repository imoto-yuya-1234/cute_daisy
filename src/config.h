#pragma once

#include <pebble.h>

#define KEY_INIT					0
#define KEY_CONNECTION    1
#define KEY_SHOW_TICKS    2
#define KEY_DECORATION    3
#define KEY_LANG					4
#define KEY_ROTATE				5

extern GColor g_connection_color;

void reload_window();

static void in_recv_handler(DictionaryIterator *iter, void *context) {
  
	Tuple *connection_t = dict_find(iter, KEY_CONNECTION);
  if(connection_t && connection_t->value->int8 > 0) {
    persist_write_bool(KEY_CONNECTION, true);
  } 
	else {
    persist_write_bool(KEY_CONNECTION, false);
  }
	
	Tuple *show_ticks_t = dict_find(iter, KEY_SHOW_TICKS);
	if(show_ticks_t && show_ticks_t->value->int8 > 0) {
		persist_write_bool(KEY_SHOW_TICKS, true);
	} 
	else {
		persist_write_bool(KEY_SHOW_TICKS, false);
	}
	
	Tuple *decoration_t = dict_find(iter, KEY_DECORATION);
	persist_write_string(KEY_DECORATION, decoration_t->value->cstring);
	
	//Tuple *lang_t = dict_find(iter, KEY_LANG);
	//persist_write_string(KEY_LANG, lang_t->value->cstring);
	
	#if defined(PBL_PLATFORM_APLITE)
	#else
	Tuple *rotate_t = dict_find(iter, KEY_ROTATE);
	if(rotate_t && rotate_t->value->int8 > 0) {
		persist_write_bool(KEY_ROTATE, true);
	} 
	else {
		persist_write_bool(KEY_ROTATE, false);
	}
	#endif
	
	reload_window();
}

void initialize_value() {
	if(!persist_read_bool(KEY_INIT)) {
		persist_write_bool(KEY_INIT, true);
		persist_write_bool(KEY_CONNECTION, true);
		persist_write_bool(KEY_SHOW_TICKS, false);
		persist_write_string(KEY_DECORATION, "heart");
		//persist_write_string(KEY_LANG, "en");
		#if defined(PBL_PLATFORM_APLITE)
		#else
		persist_write_bool(KEY_ROTATE, true);
		#endif
	}
	g_connection_color = GColorWhite;
}

void config_service() {
	app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}