#include <pebble.h>
#include "utils.h"
#include "structs.h"

int x_wobble(int start_val){
  int wobble_amount = rand() % 3;
  if(wobble_amount == 0){ //Stay still
    return 0;
  } else if (wobble_amount == 1){ //Wobble right (if possible)
    if(start_val != 5){
      return 1;
    } else {
      return 0;
    }
  } else { //Wobble left (if possible)
    if(start_val != 0){
      return -1;
    } else {
      return 0;
    }
  }
}

int y_wobble(int start_val){
  int wobble_amount = rand() % 3;
  if(wobble_amount == 0){ //Stay still
    return 0;
  } else if (wobble_amount == 1){ //Wobble up (if possible)
    if(start_val != 0){
      return -1;
    } else {
      return 0;
    }
  } else { //Wobble down (if possible)
    if(start_val != 5){
      return 1;
    } else {
      return 0;
    }
  }
}

int is_solid(element_type element){
  if(element == TREE){
    return 1;
  }
  return 0;
}

char* attack_to_string(attack atk){
  switch(atk){
    case NONE:
      return "---";
      break;
    case SWORD:
      return "Sword";
      break;
  }
  return "Error";
}

char* item_to_string(item itm){
  switch(itm){
    case NO_ITEM:
      return "---";
      break;
  }
  return "Error";
}