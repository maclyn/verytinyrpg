#include <pebble.h>
#include "button_click.h"
#include "utils.c"

//"State" enum so drawing layer knows what to do
enum game_state {
  SHOWING_INTRO = 0,
  EXPLORING = 1,
  IN_BATTLE = 2
};

//Element enum defining the different elements that can be drawn
enum element_type {
  BLANK = 0,
  TREE = 1
};

typedef struct elpos elpos;
struct elpos{
  char x;
  char y;
};

static Window *window;
static Layer *main_layer;
static AppTimer *accel_timer;

//Game state variables
int score = 0;
static char score_buffer[] = "Score: 000000";
static char scene_elements[6][6];
static elpos user_pos = {3, 3};

static GBitmap *player_bmp;
static GBitmap *tree_bmp;
static GFont main_font;

//Arrays to hold the content of the levels
enum game_state state = EXPLORING;

//Update layer
static void update_drawing_layer(struct Layer *layer, GContext *ctx){
  //Init ctx
  graphics_context_set_compositing_mode(ctx, GCompOpAssign); 
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  
  if(state == EXPLORING){
    //Draw score in top 24 pixels
    graphics_draw_rect(ctx, GRect(-1, 0, 145, 24));
    snprintf(score_buffer, sizeof(score_buffer), "Score: %d", score);
    graphics_draw_text(ctx, score_buffer, main_font, GRect(2, 0, 140, 18), GTextAlignmentRight, GTextOverflowModeTrailingEllipsis, NULL);
  
    //Draw the scenery
    for(int i = 0; i < 6; i++){
      for(int j = 0; j < 6; j++){
        int type = scene_elements[i][j];
        if(type == BLANK){
          
        } else if (type == TREE){
          graphics_draw_bitmap_in_rect(ctx, tree_bmp, GRect(i * 24, ((j * 24) + 24), 24, 24));
        }
      }
    }
    
    //Draw the player on top
    graphics_draw_bitmap_in_rect(ctx, player_bmp, GRect(user_pos.x * 24, user_pos.y * 24 + 24, 24, 24));
  } else if (state == SHOWING_INTRO) {
  } else if (state == IN_BATTLE) {
  }
}

//Indicates select click
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
}

//Indicates up click
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
}

//Indicates down click
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  tree_bmp = gbitmap_create_with_resource(RESOURCE_ID_TREE_TILE);
  player_bmp = gbitmap_create_with_resource(RESOURCE_ID_PLAYER_SPRITE);
  main_font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  
  //Create bitmap layer
  main_layer = layer_create(bounds);
  layer_set_update_proc(main_layer, update_drawing_layer);
  layer_add_child(window_get_root_layer(window), main_layer);
}

static void window_unload(Window *window) {
  gbitmap_destroy(tree_bmp);
  gbitmap_destroy(player_bmp);
  layer_destroy(main_layer);
}


static void generate_level(){
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 6; j++){
      scene_elements[i][j] = rand() % 2;
    }
  } 
}

//Manage moving the user
static void accelerometer_check(void *nodata){
  AccelData data;
  accel_service_peek(&data);
  
  //Get data values if in valid state
  if(!data.did_vibrate && state == EXPLORING){
    int xPos = data.x;
    int yPos = data.y;
    if(abs(xPos) > abs(yPos)){ //Take x action
      if(xPos > 400){ //Right
        if(user_pos.x < 5){
          ++user_pos.x;
        } else { //Level to the right
          user_pos.x = 0;
          generate_level();
        }
      } else if (xPos < -400){ //Left
        if(user_pos.x > 0){
          --user_pos.x;
        } else {
          user_pos.x = 5;
          generate_level();
        }
      }    
    } else { //Take y action
      if(yPos < -400){ //Down
        if(user_pos.y < 5){
          ++user_pos.y;
        } else {
          user_pos.y = 0;
          generate_level();
        }         
      } else if (yPos > 400){ //Up
        if(user_pos.y > 0){
          --user_pos.y;
        } else {
          user_pos.y = 5;
          generate_level();
        }
      }
    }
  }
  
  layer_mark_dirty(main_layer);
  register_accel_timer();
}

//Update position every second
static void register_accel_timer(){
  app_timer_register(1000, accelerometer_check, NULL);
}

//-------------------------------
// Window initialization routines
//-------------------------------

static void init(void) {
  window = window_create();
  window_set_fullscreen(window, true);
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	  .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, false);
  
  //Generate level
  //Seed random generator
  srand(time(NULL));
  generate_level();
  
  //Schedule acceleromter updates
  accel_data_service_subscribe(0, NULL);
  register_accel_timer();
}

static void deinit(void) {
  window_destroy(window);
  accel_data_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}