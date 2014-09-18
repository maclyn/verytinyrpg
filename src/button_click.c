#include <pebble.h>
#include "button_click.h"
#include "utils.h"
#include "structs.h"

static Window *window;
static Layer *main_layer;
static AppTimer *accel_timer;

//Game state variables
int score = 0;
static char score_buffer[] = "Score: 000000";
static char enemy_hp_buffer[] = "En:xxx/xxx";
static char player_hp_buffer[] = "Pl:xxx/xxx";

static int scene_elements[6][6];
static elpos user_pos = {3, 3};

static GBitmap *player_bmp;
static GBitmap *tree_bmp;
static GBitmap *grass_bmp;
static GBitmap *enemy_bmp;
static GFont main_font;

//Arrays to hold the content of the levels
game_state state = INTRO;
enemy_state battle_enemy = { "Unknown", 100, 100, 2, 5 };
player_state player = { 100, 100, SWORD, NONE, NONE, NO_ITEM, NO_ITEM, NO_ITEM };
menu_state menu_level = MAIN;

char* welcome_text;

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
        } else if (type == GRASS){
          graphics_draw_bitmap_in_rect(ctx, grass_bmp, GRect(i * 24, ((j * 24) + 24), 24, 24));
        }
      }
    }
    
    //Draw the player on top
    graphics_draw_bitmap_in_rect(ctx, player_bmp, GRect(user_pos.x * 24, user_pos.y * 24 + 24, 24, 24));
  } else if (state == INTRO) {
    graphics_draw_text(ctx, welcome_text, main_font, GRect(2, 70, 140, 18), GTextAlignmentCenter, GTextOverflowModeTrailingEllipsis, NULL);
  } else if (state == BATTLE) {
    //Draw state in top 24 pixels
    graphics_draw_rect(ctx, GRect(-1, 0, 145, 24));
    graphics_draw_text(ctx, "Battle Mode", main_font, GRect(2, 0, 140, 18), GTextAlignmentCenter, GTextOverflowModeTrailingEllipsis, NULL);
    
    //Draw enemy bmp + health (of player + enemy) + name
    graphics_draw_bitmap_in_rect(ctx, enemy_bmp, GRect(2, 32, 64, 48));
    snprintf(enemy_hp_buffer, sizeof(enemy_hp_buffer), "En:%d/%d", battle_enemy.hp, battle_enemy.max_hp);
    graphics_draw_text(ctx, enemy_hp_buffer, main_font, GRect(68, 36, 76, 18),
                       GTextAlignmentLeft, GTextOverflowModeTrailingEllipsis, NULL);
    snprintf(player_hp_buffer, sizeof(enemy_hp_buffer), "Pl:%d/%d", player.hp, player.max_hp);
    graphics_draw_text(ctx, player_hp_buffer, main_font, GRect(68, 58, 76, 18),
                       GTextAlignmentLeft, GTextOverflowModeTrailingEllipsis, NULL);
    graphics_draw_text(ctx, battle_enemy.name, main_font, GRect(2, 85, 140, 18),
                       GTextAlignmentLeft, GTextOverflowModeTrailingEllipsis, NULL);
    
    //Draw menu options based on menu state
    char* line1 = "";
    char* line2 = "";
    char* line3 = "";
    switch(menu_level){
      case MAIN:
        line1 = "Attack";
        line2 = "Item";
        line3 = "Run";
        break;
      case ATTACKS:
        line1 = attack_to_string(player.attack_1);
        line2 = attack_to_string(player.attack_2);
        line3 = attack_to_string(player.attack_3);
        break;
      case ITEMS:
        line1 = item_to_string(player.item_1);
        line2 = item_to_string(player.item_2);
        line3 = item_to_string(player.item_3);
        break;
    }
    graphics_draw_text(ctx, line1, main_font, GRect(2, 108, 140, 18),
                       GTextAlignmentLeft, GTextOverflowModeTrailingEllipsis, NULL);
    graphics_draw_text(ctx, line2, main_font, GRect(2, 128, 140, 18),
                       GTextAlignmentLeft, GTextOverflowModeTrailingEllipsis, NULL);
    graphics_draw_text(ctx, line3, main_font, GRect(2, 148, 140, 18),
                       GTextAlignmentLeft, GTextOverflowModeTrailingEllipsis, NULL);
  }
}

static void attack_with(attack a){
  int min_to_subtract = 5;
  int max_to_subtract = 10;
  switch(a){
    case SWORD: 
      min_to_subtract = 15;
      max_to_subtract = 25;
      break;
    case NONE:
      break;
  }
  
  int difference = max_to_subtract - min_to_subtract;
  int result = min_to_subtract + ((int)((float)(rand() % 5) * (float)difference));
  battle_enemy.hp -= result;
  enemy_turn();
}

static void use_item(item i){
  
}

static void enemy_turn(){

}

//Indicates select click
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(state == BATTLE){
    switch(menu_level){
      case MAIN:
        menu_level = ITEMS;
        break;
      case ATTACKS:
        attack_with(player.attack_2);
        break;
      case ITEMS:
        use_item(player.item_3);
        break;
    }
  }
  
  layer_mark_dirty(main_layer);
}

//Indicates up click
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(state == BATTLE){
    switch(menu_level){
      case MAIN:
        menu_level = ATTACKS;
        break;
      case ATTACKS:
        attack_with(player.attack_1);
        break;
      case ITEMS:
        use_item(player.item_1);
        break;
    }
  }
  
  layer_mark_dirty(main_layer);
}

//Indicates down click
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(state == BATTLE){
    int should_leave;
    switch(menu_level){
      case MAIN:
        should_leave = rand() % 4;
        if(should_leave){
          leave_battle();
        } else {
          player.hp -= 10;
        }
        layer_mark_dirty(main_layer);
        break;
      case ATTACKS:
        attack_with(player.attack_3);
        break;
      case ITEMS:
        use_item(player.item_3);
        break;
    }
  }
  
  layer_mark_dirty(main_layer);
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
  grass_bmp = gbitmap_create_with_resource(RESOURCE_ID_GRASS_TILE);
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
  gbitmap_destroy(grass_bmp);
  layer_destroy(main_layer);
}


static void generate_level(){
  //(1) Reset scene
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 6; j++){
      scene_elements[i][j] = -1;
    }
  }
  
  //(2) Calculate at least one clear path to another edge
  //    from user position
  int x_pos = user_pos.x;
  int y_pos = user_pos.y;
  scene_elements[x_pos][y_pos] = 0; //Make user pos empty
  if(x_pos == 5){ //Get to x_pos = 0
    while(x_pos != 0){
      int y_wob = y_wobble(y_pos);
      if(y_wob == 0){
        --x_pos;  
      }
      y_pos += y_wob;
      scene_elements[x_pos][y_pos] = 0;
    }
  } else if (x_pos == 0){ //Get to x_pos = 5
    while(x_pos != 5){
      int y_wob = y_wobble(y_pos);
      if(y_wob == 0){
        ++x_pos;  
      }
      y_pos += y_wob;
      scene_elements[x_pos][y_pos] = 0;
    }
  } else if (y_pos == 5){ //Get to y_pos = 0
    while(y_pos != 0){
      int x_wob = x_wobble(x_pos);
      if(x_wob == 0){
        --y_pos;  
      }
      x_pos += x_wob;
      scene_elements[x_pos][y_pos] = 0;
    }
  } else if (y_pos == 0){ //Get to x_pos = 5
    while(y_pos != 5){
      int x_wob = x_wobble(x_pos);
      if(x_wob == 0){
        ++y_pos;  
      }
      x_pos += x_wob;
      scene_elements[x_pos][y_pos] = 0;
    }
  }
  
  //For the rest of the field, randomly generate elements
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 6; j++){
      if(scene_elements[i][j] == -1) scene_elements[i][j] = rand() % NUM_ELEMENTS;
    }
  } 
}

static void move_user(int x, int y){
  int xPos = x;
  int yPos = y;
  
  if(state == EXPLORING){
    if(abs(xPos) > abs(yPos)){ //Take x action
      if(xPos > 400){ //Right
        if(user_pos.x < 5){
          if(!is_solid(scene_elements[user_pos.x+1][user_pos.y])){
            ++user_pos.x;
          }
        } else { //Level to the right
          user_pos.x = 0;
          generate_level();
        }
      } else if (xPos < -400){ //Left
        if(user_pos.x > 0){
          if(!is_solid(scene_elements[user_pos.x-1][user_pos.y])){
            --user_pos.x;
          }
        } else {
          user_pos.x = 5;
          generate_level();
        }
      }    
    } else { //Take y action
      if(yPos < -400){ //Down
        if(user_pos.y < 5){
          if(!is_solid(scene_elements[user_pos.x][user_pos.y+1])){
            ++user_pos.y;
          }
        } else {
          user_pos.y = 0;
          generate_level();
        }         
      } else if (yPos > 400){ //Up
        if(user_pos.y > 0){
          if(!is_solid(scene_elements[++user_pos.x][user_pos.y-1])){
            --user_pos.y;
          }
        } else {
          user_pos.y = 5;
          generate_level();
        }
      }
    }
    
    //Run collision checks
    int type = scene_elements[user_pos.x][user_pos.y];
    switch(type){
     case GRASS: //Potential battle
      enter_battle();
      break;
    }
  }
  
  layer_mark_dirty(main_layer);
}

static void enter_battle(){
  if(rand() % 2 == 1){ //Randomly enter the battles
    //Set state
    state = BATTLE;
    
    //Set enemy
    int enemy = SPIDER; //only one for now
    int resource_to_load; 
    switch(enemy){
      case SPIDER:
        battle_enemy.name = "Spider";
        battle_enemy.hp = 100;
        battle_enemy.max_hp = 100;
        battle_enemy.min_attack = 2;
        battle_enemy.max_attack = 10;
        resource_to_load = RESOURCE_ID_SPIDER_SPRITE;
        break;
    }
    
    enemy_bmp = gbitmap_create_with_resource(resource_to_load);
    menu_level = MAIN;
  }
}

static void leave_battle(){
  state = EXPLORING;
  layer_mark_dirty(main_layer);
  gbitmap_destroy(enemy_bmp);
}

//Manage moving the user
static void accelerometer_check(void *nodata){
  AccelData data;
  accel_service_peek(&data);
  
  //Get data values if in valid state
  if(!data.did_vibrate){
    move_user(data.x, data.y);
  }
  
  register_accel_timer();
}

//Update position every second
static void register_accel_timer(){
  app_timer_register(750, accelerometer_check, NULL);
}

//Check menu back
static void register_check_timer(){
  app_timer_register(200, check_menu_back, NULL);
}

//Check menu state
static void check_menu_back(void *nodata){
  if(state == BATTLE){
    AccelData data;
    accel_service_peek(&data);
    if(data.x < -400){ //Back
      if(menu_level == ATTACKS || menu_level == ITEMS){
        menu_level = MAIN;
      }
    }
  }
  
  layer_mark_dirty(main_layer);
  register_check_timer();
}

static void show_intro(char* text){
  welcome_text = text;
  layer_mark_dirty(main_layer);
  app_timer_register(2000, hide_intro, NULL);
}

static void hide_intro(void *nodata){
  state = EXPLORING;
  generate_level();
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
  show_intro("FOREST");
  
  //Schedule acceleromter updates
  accel_data_service_subscribe(0, NULL);
  register_accel_timer();
  register_check_timer();
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