#pragma once
  
//"State" enum so drawing layer knows what to do
typedef enum game_state game_state;
enum game_state {
  SHOWING_INTRO = 0,
  EXPLORING = 1,
  IN_BATTLE = 2
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