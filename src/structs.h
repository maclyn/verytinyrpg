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

typedef enum enemy enemy;
enum enemy {
  SPIDER = 0
};

typedef struct enemy_state enemy_state;
struct enemy_state{
  char* name;
  short hp;
  short max_hp;
};