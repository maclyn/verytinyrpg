#pragma once
#define NUM_ENEMIES 1
#define NUM_ELEMENTS 3
  
//"State" enum so drawing layer knows what to do
typedef enum game_state game_state;
enum game_state {
  INTRO = 0,
  EXPLORING = 1,
  BATTLE = 2
};

//Where in the battle menu we are
//Every other menu is one/two levels and 
//status can be represented by a boolean value
typedef enum menu_state menu_state;
enum menu_state{
  MAIN = 0,
  ATTACKS = 1,
  ITEMS = 2
};

//Define the options for enemies
typedef enum enemy enemy;
enum enemy {
  SPIDER = 0,
  TREE_MONSTER = 1
};

//Define the options for attacks
typedef enum attack attack;
enum attack {
  NONE = 0,
  SWORD = 1
};

typedef enum item item;
enum item {
  NO_ITEM = 0,
};

//Element enum defining the different elements that can be drawn
typedef enum element_type element_type;
enum element_type {
  BLANK = 0,
  TREE = 1,
  GRASS = 2
};

typedef struct elpos elpos;
struct elpos{
  short x;
  short y;
};

typedef struct enemy_state enemy_state;
struct enemy_state{
  char* name;
  short hp;
  short max_hp;
  short min_attack;
  short max_attack;
};

typedef struct player_state player_state;
struct player_state{
  short hp;
  short max_hp;
  short exp;
  short money;
  attack attack_1;
  attack attack_2;
  attack attack_3;
  item item_1;
  item item_2;
  item item_3;
};