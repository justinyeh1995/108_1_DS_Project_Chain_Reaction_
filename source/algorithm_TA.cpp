#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

void algorithm_B(Board board, Player player, int index[]){
    srand(time(NULL)*time(NULL));
    int row, col;
    int color = player.get_color();
    
    while(1){
        row = rand() % 5;
        col = rand() % 6;
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }

    index[0] = row;
    index[1] = col;
}