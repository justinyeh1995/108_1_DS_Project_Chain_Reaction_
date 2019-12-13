#ifndef INCLUDE_RULES_H_
#define INCLUDE_RULES_H_

#include <iostream>
#include "player.h"
#include "board.h"

/******************************************************
 * This header is about the data structure of the Rules
 * which includes all the illegal operations detection 
 * functions
******************************************************/

#define LEGAL true
#define ILLEGAL false

// Check all the rules when player places the orb
bool placement_illegal(Player player, Cell cell);

// Check wether the idnex is over the boundary of board
bool index_range_illegal(int i, int j);

// Determine if there is a violation, if yes return true
bool rules_violation(Player player);

#endif