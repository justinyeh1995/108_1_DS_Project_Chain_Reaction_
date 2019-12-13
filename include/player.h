#ifndef INCLUDE_PLAYER_H_
#define INCLUDE_PLAYER_H_

/***************************************************
 * This header is about the data structure player
***************************************************/

#define RED 'r'
#define BLUE 'b'

class Player{
    private:
        char color;     // The color belonging to player
        bool illegal;   // If yes, terminate the program and determine the playerwho violates the rules to be a loser

    public:
        Player(int col);

        // The basic functons of Player
        char get_color();
        bool is_illegal();
        void set_illegal();
};

#endif