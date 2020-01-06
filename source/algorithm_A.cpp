#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
using namespace std;
#define INFINITY 1000000


/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/

//Data Sturcture <stack>
template <class T>
class stack{
    private:
        struct Node{
            T data;
            Node* next_node;
            Node(T data = 0):data(data){}
            Node(T data, Node* next):data(data),next_node(next){}
        };
        Node* topNode;
        int Size;
    public:
        stack(): Size(0),topNode(0){}
        void push(T element);
        int size();
        T top();
        bool empty();
        void pop();
};
template <class T> 
int stack<T>::size(){
    return this->Size;
}
template <class T> 
bool stack<T>::empty(){
    if(this->size() == 0){
        return true;
    }
    return false;
}
template <class T> 
void stack<T>::push(T n){
    if(empty()){
        Node* newnode = new Node(n,0);
        this->topNode = newnode;
        this->Size++;
        return;
    }
    Node* newnode = new Node(n,topNode);//point to the current top address
    topNode = newnode; //Update the top address to newnode
    this->Size++;
}
template <class T> 
void stack<T>::pop() {
    if(empty()) return;
    Node* deletenode = topNode;
    topNode = topNode->next_node;
    delete deletenode;
    deletenode = 0;
    Size--;
}
template <class T> 
T stack<T>::top(){
    return topNode->data;
}
//endof <stack> 

struct Node {
    int row;
    int col;
    Node(int i, int j):row(i),col(j){}
};

Board copy(Board board) {
    int record[ROW][COL];
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            record[i][j] = board.get_orbs_num(i,j);
        }
    }
    char player[ROW][COL];
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            player[i][j] = board.get_cell_color(i,j);   
        }
    }
    Board copy;
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            for(int k = 0; k < record[i][j]; k++) {
                Player* play = new Player(player[i][j]);
                //cout<<"problem in copy"<<endl;
                copy.place_orb(i, j, play);
                delete play;
            }
        }
    }

    return copy;
}

bool is_enemy(Board board,int i, int j, Player player) {
    bool enemy = false;
    // is_enemy:= not player, 'w' excluded
    if(board.get_cell_color(i,j) != player.get_color() && board.get_cell_color(i,j) != 'w')
        enemy = true;
    return enemy;
}

bool is_critical(Board board,int i, int j) {
    return board.get_orbs_num(i,j) == board.get_capacity(i,j)-1;
}

int* find_contiguous(Board board, Player player) {
    bool table[ROW][COL];
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            table[i][j] = false;
            if(is_critical(board,i,j)) {
                table[i][j] = true;
            }   
        }
    }
    int* chain = new int[ROW*COL];
    for(int i = 0; i < ROW*COL; i++) {
        chain[i] = 0;
    }
    int num = 0;
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if(table[i][j]) {
                int l = 0;
                Node node(i,j);
                stack<Node> s;
                while(!s.empty()) {
                    Node pos = s.top();
                    int x = pos.row;
                    int y = pos.col;
                    table[x][y] = false;
                    l++;
                    if(x-1 >= 0 && table[x-1][y]) {
                        Node temp(x-1,y);
                        s.push(temp);
                    }
                    if(x+1 <= 4 && table[x+1][y]) {
                        Node temp(x+1,y);
                        s.push(temp);
                    }
                    if(y-1 >= 0 && table[x][y-1]) {
                        Node temp(x,y-1);
                        s.push(temp);
                    }
                    if(y+1 <= 5 && table[x][y+1]) {
                        Node temp(x,y+1);
                        s.push(temp);
                    }
                }
                chain[num] = l;
                num++;
            }
        }
    }
    return chain;
}
// Need to be accurate :((
int evaluate(Board board, Player player) {
    int score;
    int my_count = 0;
    int enemy_count = 0;
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            // if cell can be placed by the player
            if(board.get_cell_color(i,j) == player.get_color()) {
                my_count += board.get_orbs_num(i,j);
                bool isVulnerable = false;
                if( i-1 >= 0 && is_enemy(board, i-1, j, player) && is_critical(board,i-1, j)) {
                    score += 5 -board.get_capacity(i-1,j);
                    bool isVulnerable = true;
                }
                if(i+1 <= 4 && is_enemy(board, i+1, j, player) && is_critical(board, i+1, j)) {
                    score += 5 -board.get_capacity(i+1,j);
                    bool isVulnerable = true;
                }
                if( j-1 >= 0 && is_enemy(board, i, j-1, player) && is_critical(board, i, j-1)) {
                    score += 5 -board.get_capacity(i,j-1);
                    bool isVulnerable = true;
                }
                if(j+1 <= 5 && is_enemy(board, i, j+1, player) && is_critical(board, i, j+1)) {
                    score += 5 -board.get_capacity(i,j+1);
                    bool isVulnerable = true;
                }
                //
                if(!isVulnerable){
                    if(board.get_capacity(i,j) == 3) {
                        score += 2;
                    }
                    else if(board.get_capacity(i,j) == 2) {
                        score += 3;
                    }
                    if(is_critical(board, i, j)) {
                        score += 2;
                    }
                }
            }
            //enemy
            else {
                enemy_count += board.get_orbs_num(i,j);
            }
        }
    }
    // start fixing here
    /*
    if board is full 
        then start a new scenario

    */
    score += my_count;
    // we win 
    if(enemy_count == 0 && my_count != 0){
        return 100000;
    }
    // enemy has won last round board 
    if(enemy_count != 0 && my_count == 0){
        return -100000;
    }
    int* chain = find_contiguous(board, player);
    for(int i = 0; i < ROW*COL; i++) {
        if(chain[i] > 1) {
            score += 2*chain[i]; 
        }
    }
    //cout<<"This round player is "<<player.get_color()<<endl;
    return score;
}

int minimax(Board board, int depth, Player player, Player opponent, int alpha, int beta, bool isMax) {
    // seperate our and oppponent's color 
    char max_round = player.get_color();
    char min_round = opponent.get_color();
    
    Player* max_player= new Player(max_round);
    Player* min_player= new Player(min_round);
    //board.print_current_board(0, 0, -100);
    //cout<<"depth: "<<depth<<endl;
    int score = evaluate(board, player);
    if(score == 100000) return score;
    if(score == -100000) return score;
    // Base condition: leaf node always set to max, which implies depth must be an odd number
    if(depth == 3) { 
        //cout<<"The score is: "<<score<<endl;   
        return score;
    }

    //isMax : this_round is us while opponent_round is enemy
    if(isMax == true) {
        //cout<<"isMax"<<endl;
        int best = -INFINITY;
        for(int i = 0; i < ROW; i++) {
            for(int j = 0; j < COL; j++) {
                // if cell can be placed by the player
                if(board.get_cell_color(i,j) == max_round || board.get_cell_color(i,j) == 'w') {
                    //cout<<"\nIn [ "<<i<<" , "<<j<<" ]"<<endl;
                    Board board_copy = copy(board);
                    // Problem not sure why
                    board_copy.place_orb(i,j,max_player);
 
                    //best = max(best, minimax(board_copy, depth+1, player, opponent, alpha, beta, false));
                    if(minimax(board_copy, depth+1, player, opponent, alpha, beta, false) > best ) {
                        best = minimax(board_copy, depth+1, player, opponent, alpha, beta, false); 
                    }
                    //alpha = max(alpha, best); 
                    if(best > alpha) {
                        alpha = best;
                    }   
                    if (beta <= alpha) 
                        break;  
                }
            }
        }
        //cout<<"Value returned by isMax: "<<best<<endl;
        return best;
    }
    // isMin
    else {
        //cout<<"isMin"<<endl;
        int best = INFINITY;
        for(int i = 0; i < ROW; i++) {
            for(int j = 0; j < COL; j++) {
                // if cell can be placed by the player
                if(board.get_cell_color(i,j) == min_round || board.get_cell_color(i,j) == 'w') {
                    //cout<<"\nIn [ "<<i<<" , "<<j<<" ]"<<endl;
                    Board board_copy = copy(board);
                    board_copy.place_orb(i,j,min_player);
            
                    //best = min(best, minimax(board_copy, depth+1, player, opponent, alpha, beta, true));
                    if(minimax(board_copy, depth+1, player, opponent, alpha, beta, true) < best ) {
                        best = minimax(board_copy, depth+1, player, opponent, alpha, beta, true);
                    }
                    //beta = min(beta, best);
                    if(best < beta) {
                        beta = best;
                    }  
                    if (beta <= alpha) 
                        break;   
                }
            }
        }
        //cout<<"Value returned by isMin: "<<best<<endl;
        return best;
    }
    // Error occurs
    return -1;
}

Node find_best_move(Board board, Player player) {
    // player_me is the Maximum player
    char me = player.get_color();
    Player* player_me = new Player(me);
    // create enemy for minimax
    char enemy;
    if(me == 'r') { enemy = 'b'; }
    else { enemy = 'r';}
    Player opponent(enemy); 
    // might cause violation
    int bestvalue = -INFINITY;
    Node best_move(-1,-1);
    // we operate minimax on each of the possible placement and return the best one (score wised) among them.
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            if(board.get_cell_color(i,j) == me || board.get_cell_color(i,j) == 'w'){
                // use board_copy to avoid corrupting the current board
                Board board_copy = copy(board);
                // place the possible movement
                //cout<<"place in findMax"<<endl;
                board_copy.place_orb(i,j,player_me);
                // Emergency cut-off
                // if(evaluate(board_copy,player) == 100000) {
                //     cout<<"\nWin!"<<endl;
                //     best_move.row = i;
                //     best_move.col = j;
                //     return best_move;
                // }
            
                // operate minimax
                int alpha = -INFINITY;
                int beta =  INFINITY;
                int depth = 0;
                bool isMax = true;
                int move = minimax(board_copy, depth, player ,opponent, alpha, beta, false);
                if(move > bestvalue) {
                    bestvalue = move;
                    best_move.row = i;
                    best_move.col = j;
                }
            }
        }
    }
    //cout<<"Return the best move: [ "<<best_move.row<<", "<<best_move.col<<" ] and the corresponding best value is: "<<bestvalue<<endl;
    //cout<<endl;
    return best_move;
}


void algorithm_A(Board board, Player player, int index[]){

    // we are the first one; first step get corner
    int count = 0;
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            count += board.get_orbs_num(i,j);
        }
    }
    if(count == 0) {
        index[0] = 0;
        index[1] = 0;
    }
    // other than the first step
    else {
        Node best = find_best_move(board, player);
        index[0] = best.row;
        index[1] = best.col;
    }
}