#include "../include/rules.h"
#include <iostream>

using namespace std;

bool placement_illegal(Player player, Cell cell){

    bool illegal = false;
    // Check whether the cell has be placed or not
    if(cell.get_color() == 'w') return illegal;

    //Check color
    if(cell.get_color() != player.get_color()) illegal = true;

    return illegal;

}

bool index_range_illegal(int i, int j){

    bool illegal = false;
    if(i > 4 || j > 5) illegal = true;
    return illegal;

}

bool rules_violation(Player player){

    if(player.is_illegal()){
        
        if(player.get_color() == 'b'){
            cout << "Blue_Player violated the game rules." << endl;
            cout << "Red_Player is winner." << endl;
        }

        if(player.get_color() == 'r'){
            cout << "Red_Player violated the game rules." << endl;
            cout << "Blue_Player is winner." << endl;
        }

        return true;
    }

    else
    {
        return false;
    }
    
}
