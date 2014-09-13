#pragma once
  
//Header file for button_click (i.e. main)
static void update_drawing_layer(struct Layer *layer, GContext *ctx);
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void up_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_handler(ClickRecognizerRef recognizer, void *context);
static void click_config_provider(void *context);
static void window_load(Window *window);
static void window_unload(Window *window);
static void generate_level();
static void accelerometer_check(void *nodata);
static void register_accel_timer();
static void init(void);
static void deinit(void);
int main(void);