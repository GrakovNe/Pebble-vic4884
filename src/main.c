#include <pebble.h>
#include <window.h>

static char Time_Buffer []    = "00:00";
static char Date_Buffer []   = "WEDNESDAY 12.12";
static char Battery_Buffer [] = "100%";
static char Weather_Buffer [] = "+9999";
Window *s_window;

static uint32_t BT_Icons[] = {RESOURCE_ID_NO_BT_PICTURE, RESOURCE_ID_BT_PICTURE} ;

const char Weekday_names[7][32] = {
	"SUNDAY",
	"MONDAY",
	"TUESDAY",
	"WEDNESDAY",
	"THURSDAY",
	"FRIDAY",
	"SATURDAY"
};

const uint32_t Weather_icons[] = {RESOURCE_ID_CLOUDS, RESOURCE_ID_RAIN, RESOURCE_ID_SNOW, RESOURCE_ID_STORM, RESOURCE_ID_SUN};

struct {
	bool Vibe_Hourly;
	bool Vibe_BT;
} Settings;

enum {
	WEATHER_TEMPERATURE  = 0,
	WEATHER_CONDITIONS   = 1,
	SETTINGS_HOURLY_VIBE = 2,
	SETTINGS_BT_VIBE     = 3
};

extern TextLayer *Time_Text;
extern TextLayer *Date_Text;
extern TextLayer *Battery_Text;
extern TextLayer *Weather_Text;
extern GBitmap *s_res_bt_picture;
extern BitmapLayer *BT_Layer;
BitmapLayer *Weather_Layer;
GBitmap *Weather_Bitmap;

char Temp [32];

void send_int(uint8_t key, uint8_t cmd)
{
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);
 
    app_message_outbox_send();

}

void Update_Connection(bool Connected){
	if (Settings.Vibe_BT)
		vibes_long_pulse();
	
	gbitmap_destroy(s_res_bt_picture);
	s_res_bt_picture = gbitmap_create_with_resource(BT_Icons[Connected]); 
	bitmap_layer_set_bitmap(BT_Layer, s_res_bt_picture); 
}

void Update_Battery(BatteryChargeState State){
	snprintf(Battery_Buffer, sizeof(Battery_Buffer), "%d%%", State.charge_percent);
	text_layer_set_text(Battery_Text, Battery_Buffer);
}

static void Update_Time(struct tm* CurrentTime, TimeUnits units_changed){
	strftime(Time_Buffer, sizeof(Time_Buffer), "%H:%M", CurrentTime);
	text_layer_set_text(Time_Text, Time_Buffer);
	
	
	snprintf(Date_Buffer, sizeof(Date_Buffer), "%s %02d.%02d", Weekday_names[CurrentTime->tm_wday], CurrentTime->tm_mday, CurrentTime->tm_mon + 1);
	text_layer_set_text(Date_Text, Date_Buffer);
	
	if (CurrentTime -> tm_min == 30)
			send_int(WEATHER_TEMPERATURE, 0);
	
	if ( (CurrentTime -> tm_min == 0) && (Settings.Vibe_Hourly) )
		vibes_double_pulse();
	
}

static void Process_Received_Data(DictionaryIterator *iter, void *context){
	Tuple *t = dict_read_first(iter);
	 while(t != NULL){
		APP_LOG(APP_LOG_LEVEL_INFO, "something received");
		int key = t->key;
        int value = t->value->int32;
		char string_value[32];
		strcpy(string_value, t->value->cstring);	
		switch (key){
			case WEATHER_TEMPERATURE:
				APP_LOG(APP_LOG_LEVEL_INFO, string_value);
				snprintf(Weather_Buffer, sizeof(Weather_Buffer), "%s", string_value);
				text_layer_set_text(Weather_Text, Weather_Buffer);
				break;
			
			case WEATHER_CONDITIONS:
				gbitmap_destroy(Weather_Bitmap); 
				Weather_Bitmap = gbitmap_create_with_resource(Weather_icons[value]); 
				bitmap_layer_set_bitmap(Weather_Layer, Weather_Bitmap); 
				break;
			
			case SETTINGS_HOURLY_VIBE:
				Settings.Vibe_Hourly = value;
				break;
			
			case SETTINGS_BT_VIBE:
				Settings.Vibe_BT = value;
		}
		 
		 
        t = dict_read_next(iter);
    }	
}



void handle_init(void) {
	show_window();
	app_message_register_inbox_received(Process_Received_Data);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	window_stack_push(s_window, true);
	
	
	tick_timer_service_subscribe(MINUTE_UNIT, &Update_Time);
	
	Update_Battery(battery_state_service_peek());
	
	battery_state_service_subscribe(Update_Battery);
	
	
	Update_Connection(bluetooth_connection_service_peek());
	bluetooth_connection_service_subscribe(Update_Connection);
	
}

void handle_deinit(void) {
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
