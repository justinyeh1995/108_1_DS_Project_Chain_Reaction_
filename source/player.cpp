#include "../include/player.h"
#include <iostream>

using namespace std;

Player::Player(int col){
    color = col;
    illegal = false;
}

void Player::set_illegal(){
    this->illegal = true;
}

char Player::get_color(){
    return this->color;
}

bool Player::is_illegal(){
    return this->illegal;
}