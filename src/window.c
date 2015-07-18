#include <pebble.h>
#include "window.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
extern Window *s_window;
static GFont s_res_time_font_54;
static GFont s_res_battery_font_12;
static GFont s_res_date_font_12;
static GFont s_res_weather_font_42;
TextLayer *Time_Text;
TextLayer *Battery_Text;
TextLayer *Date_Text;
TextLayer *Weather_Text;
BitmapLayer *BT_Layer;
GBitmap *s_res_bt_picture;

extern BitmapLayer *Weather_Layer;
extern GBitmap *Weather_Bitmap;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, 1);
  #endif
  
  s_res_time_font_54 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TIME_FONT_54));
  s_res_battery_font_12 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_BATTERY_FONT_12));
  s_res_date_font_12 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DATE_FONT_16));
  s_res_weather_font_42 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_WEATHER_FONT_42));
  s_res_bt_picture = gbitmap_create_with_resource(RESOURCE_ID_BT_PICTURE);
  // Time_Text
  Time_Text = text_layer_create(GRect(1, 53, 143, 54));
  text_layer_set_background_color(Time_Text, GColorClear);
  text_layer_set_text(Time_Text, "00:00");
  text_layer_set_text_alignment(Time_Text, GTextAlignmentCenter);
  text_layer_set_font(Time_Text, s_res_time_font_54);
  layer_add_child(window_get_root_layer(s_window), (Layer *)Time_Text);
  
  // Battery_Text
  Battery_Text = text_layer_create(GRect(109, 2, 32, 20));
  text_layer_set_text(Battery_Text, "100%");
  text_layer_set_text_alignment(Battery_Text, GTextAlignmentRight);
  text_layer_set_font(Battery_Text, s_res_battery_font_12);
  layer_add_child(window_get_root_layer(s_window), (Layer *)Battery_Text);
  
  // Date_Text
  Date_Text = text_layer_create(GRect(0, 115, 143, 20));
  text_layer_set_background_color(Date_Text, GColorClear);
  text_layer_set_text(Date_Text, "");
  text_layer_set_text_alignment(Date_Text, GTextAlignmentCenter);
  text_layer_set_font(Date_Text, s_res_date_font_12);
  layer_add_child(window_get_root_layer(s_window), (Layer *)Date_Text);
  
  // Weather_Text
  Weather_Text = text_layer_create(GRect(57, 17, 69, 42));
  text_layer_set_text(Weather_Text, "+25");
  text_layer_set_text_alignment(Weather_Text, GAlignRight);
  text_layer_set_font(Weather_Text, s_res_weather_font_42);
  layer_add_child(window_get_root_layer(s_window), (Layer *)Weather_Text);
  
  // BT_Layer
  BT_Layer = bitmap_layer_create(GRect(2, 2, 16, 16));
  bitmap_layer_set_bitmap(BT_Layer, s_res_bt_picture);
  layer_add_child(window_get_root_layer(s_window), (Layer *)BT_Layer);
	
  // Weather icon
  Weather_Layer = bitmap_layer_create(GRect(10, 26, 62, 34));
  Weather_Bitmap = gbitmap_create_with_resource(RESOURCE_ID_CLOUDS);
  bitmap_layer_set_bitmap(Weather_Layer, Weather_Bitmap);
  layer_add_child(window_get_root_layer(s_window), (Layer *)Weather_Layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(Time_Text);
  text_layer_destroy(Battery_Text);
  text_layer_destroy(Date_Text);
  text_layer_destroy(Weather_Text);
  bitmap_layer_destroy(BT_Layer);
  fonts_unload_custom_font(s_res_time_font_54);
  fonts_unload_custom_font(s_res_battery_font_12);
  fonts_unload_custom_font(s_res_date_font_12);
  fonts_unload_custom_font(s_res_weather_font_42);
  gbitmap_destroy(s_res_bt_picture);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_window(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
 
}

void hide_window(void) {
  window_stack_remove(s_window, true);
}
