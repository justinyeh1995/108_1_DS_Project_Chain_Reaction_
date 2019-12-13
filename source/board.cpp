#include <iostream>
#include "../include/board.h"
#include "../include/rules.h"

using namespace std;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define CLEAR "CLS"
    
#elif __APPLE__
    #define CLEAR "clear"
    #endif

Cell::Cell(){
    orbs_num = 0, capacity = 4, color='w', explode = false;
}

int Cell::get_capacity(){
    return this->capacity;
}

int Cell::get_orbs_num(){
    return this->orbs_num;
}

char Cell::get_color(){
    return this->color;
}

bool Cell::get_explode(){
    return this->explode;
}

void Cell::set_orbs_num(int orbs){
    this->orbs_num = orbs;
}

void Cell::set_capacity(int cap){
    this->capacity = cap;
}

void Cell::set_color(char col){
    this->color = col;
}

void Cell::set_explode(bool tof){
    this->explode = tof;
}

Board::Board(){

    ////// Initialize the borad with correct capacity //////
    // The corners of the board
    cells[0][0].set_capacity(2), cells[0][5].set_capacity(2),
    cells[4][0].set_capacity(2), cells[4][5].set_capacity(2);

    // The edges of the board
    cells[0][1].set_capacity(3), cells[0][2].set_capacity(3), cells[0][3].set_capacity(3), cells[0][4].set_capacity(3),
    cells[1][0].set_capacity(3), cells[2][0].set_capacity(3), cells[3][0].set_capacity(3),
    cells[1][5].set_capacity(3), cells[2][5].set_capacity(3), cells[3][5].set_capacity(3),
    cells[4][1].set_capacity(3), cells[4][2].set_capacity(3), cells[4][3].set_capacity(3), cells[4][4].set_capacity(3);

}

void Board::print_current_board(int i, int j, int round){

    int orb_num;
    char symbol;

    ////// Print out the current state of the board //////
    //system(CLEAR);
    cout << "Round: " << round << endl;
    cout << "Place orb on (" << i << ", " << j << ")" << endl;
    cout << "=========================================" << endl;
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){

            symbol = cells[i][j].get_color();
            switch(symbol){
                case 'r':
                    symbol = 'O';
                    break;
                case 'b':
                    symbol = 'X';
                    break;
                default:
                    break;
            }

            orb_num = cells[i][j].get_orbs_num();
            switch(orb_num){
                case 0:
                    cout << "|    | ";
                    break;
                case 1:
                    cout << "|" << symbol << "   | ";
                    break;
                case 2: 
                    cout << "|" << symbol << symbol << "  | ";
                    break;
                case 3:
                    cout << "|" << symbol << symbol << symbol << " | ";
                    break;
                default:
                    cout << "|" << symbol << symbol << symbol << symbol << "| ";
            }
        }
        cout << endl;
    }
    cout << "=========================================" << endl << endl;
}

bool Board::place_orb(int i, int j, Player * player){
    
    if(index_range_illegal(i, j) || !placement_illegal(*player, cells[i][j])){
        int temp = cells[i][j].get_orbs_num();
        temp += 1;
        cells[i][j].set_orbs_num(temp);
        cells[i][j].set_color(player->get_color());
    }
    else{
        player->set_illegal();
        return false;
    }

    if(cell_is_full(&cells[i][j])){
        cell_explode(i, j);
        cell_reaction_marker();
        cell_chain_reaction(*player);
    }

    return true;
}

bool Board::cell_is_full(Cell* cell){
    if(cell->get_orbs_num() >= cell->get_capacity()){
        cell->set_explode(true);
        return true;
    }
    else return false;
}

void Board::add_orb(int i, int j, char color){
    int orb_num = cells[i][j].get_orbs_num();
    orb_num ++;
    cells[i][j].set_orbs_num(orb_num);
    cells[i][j].set_color(color);
}

void Board::cell_reset(int i, int j){
    cells[i][j].set_orbs_num(0);
    cells[i][j].set_explode(false);
    cells[i][j].set_color('w');
}

void Board::cell_explode(int i, int j){

    int orb_num;
    char color = cells[i][j].get_color();

    cell_reset(i, j);

    if( i + 1 < ROW){
        add_orb(i+1, j, color);
    }

    if( j + 1 < COL){
        add_orb(i, j+1, color);
    }

    if( i - 1 >= 0){
        add_orb(i-1, j, color);
    }

    if( j - 1 >= 0){
        add_orb(i, j-1, color);
    }
}

void Board::cell_reaction_marker(){

    // Mark the next cell whose number of orbs is equal to the capacity
    for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                cell_is_full(&cells[i][j]);
            }
        }     
}

void Board::cell_chain_reaction(Player player){
    
    bool chain_reac = true;

    while(chain_reac){

        chain_reac = false;

        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                if(cells[i][j].get_explode()){
                    cell_explode(i ,j);
                    chain_reac = true;
                }
            }
        }

        if(win_the_game(player)){
            return;
        }

        cell_reaction_marker();
    }
}

bool Board::win_the_game(Player player){

    char player_color = player.get_color();
    bool win = true;

    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(cells[i][j].get_color() == player_color || cells[i][j].get_color() == 'w') continue;
            else{
                win = false;
                break;
            }
        }
        if(!win) break;
    }
    return win;
}

int Board::get_orbs_num(int i, int j){
    return cells[i][j].get_orbs_num();
}

int Board::get_capacity(int i, int j){
    return cells[i][j].get_capacity();
}

char Board::get_cell_color(int i, int j){
    return cells[i][j].get_color();
}