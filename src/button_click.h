#pragma once
#include "structs.h"
  
//Screen draw function
static void update_drawing_layer(struct Layer *layer, GContext *ctx);

//Game movement functions
static void generate_level();
static void move_user(int x, int y);
static void check_menu_back(void *nodata);
static void enter_battle();
static void leave_battle();
static void attack_with(attack a);
static void use_item(item i);
static void enemy_turn();

//Pebble lifecycle management functions
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void up_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_handler(ClickRecognizerRef recognizer, void *context);
static void click_config_provider(void *context);
static void window_load(Window *window);
static void window_unload(Window *window);
static void accelerometer_check(void *nodata);
static void register_check_timer();
static void register_accel_timer();
static void show_intro(char* text);
static void hide_intro();
static void init(void);
static void deinit(void);
int main(void);