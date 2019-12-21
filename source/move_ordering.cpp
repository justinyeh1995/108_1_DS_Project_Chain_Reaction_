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

struct Nodess {
    int row;
    int col;
    int value;
    Nodess(int i, int j):row(i),col(j),value(0){}
};

Board copyss(Board board) {
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
                copy.place_orb(i, j, play);
                delete play;
            }
        }
    }
    //copy.print_current_board(100,100,0);
    return copy;
}

bool is_enemyss(Board board,int i, int j, Player player) {
    bool enemy = false;
    // is_enemy:= not player, 'w' excluded
    if(board.get_cell_color(i,j) != player.get_color() && board.get_cell_color(i,j) != 'w')
        enemy = true;
    return enemy;
}

bool is_criticalss(Board board,int i, int j) {
    return board.get_orbs_num(i,j) == board.get_capacity(i,j)-1;
}

int* find_contiguousss(Board board, Player player) {
    bool table[ROW][COL];
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            table[i][j] = false;
            if(is_criticalss(board,i,j)) {
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
                Nodess node(i,j);
                stack<Nodess> s;
                while(!s.empty()) {
                    Nodess pos = s.top();
                    int x = pos.row;
                    int y = pos.col;
                    table[x][y] = false;
                    l++;
                    if(x-1 >= 0 && table[x-1][y]) {
                        Nodess temp(x-1,y);
                        s.push(temp);
                    }
                    if(x+1 <= 4 && table[x+1][y]) {
                        Nodess temp(x+1,y);
                        s.push(temp);
                    }
                    if(y-1 >= 0 && table[x][y-1]) {
                        Nodess temp(x,y-1);
                        s.push(temp);
                    }
                    if(y+1 <= 5 && table[x][y+1]) {
                        Nodess temp(x,y+1);
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
int evaluatess(Board board, Player player) {
    int score;
    int my_count = 0;
    int enemy_count = 0;
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++) {
            // if cell can be placed by the player
            if(board.get_cell_color(i,j) == player.get_color()) {
                my_count += board.get_orbs_num(i,j);
                bool isVulnerable = false;
                if( i-1 >= 0 && is_enemyss(board, i-1, j, player) && is_criticalss(board,i-1, j)) {
                    score += 5 -board.get_capacity(i-1,j);
                    bool isVulnerable = true;
                }
                if(i+1 <= 4 && is_enemyss(board, i+1, j, player) && is_criticalss(board, i+1, j)) {
                    score += 5 -board.get_capacity(i+1,j);
                    bool isVulnerable = true;
                }
                if( j-1 >= 0 && is_enemyss(board, i, j-1, player) && is_criticalss(board, i, j-1)) {
                    score += 5 -board.get_capacity(i,j-1);
                    bool isVulnerable = true;
                }
                if(j+1 <= 5 && is_enemyss(board, i, j+1, player) && is_criticalss(board, i, j+1)) {
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
                    if(is_criticalss(board, i, j)) {
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
    score += my_count;
    // we win 
    if(enemy_count == 0 && my_count != 0){
        return 100000;
    }
    // enemy has won last round board 
    if(enemy_count != 0 && my_count == 0){
        return -100000;
    }
    int* chain = find_contiguousss(board, player);
    for(int i = 0; i < ROW*COL; i++) {
        if(chain[i] > 1) {
            score += 2*chain[i]; 
        }
    }
    //cout<<"This round player is "<<player.get_color()<<endl;
    return score;
}


int partition_descend(Nodess** res, int left, int right) { 
    int i = left - 1; 
    int j;
    for(j = left; j < right; j++) { 
        if(res[j]->value >= res[right]->value) { 
            i++; 
            swap(res[i], res[j]); 
        } 
    } 
    swap(res[i+1], res[right]); 
    return i+1; 
} 

void quicksort_descend(Nodess** res, int left, int right) { 
    if(left < right) { 
        int q = partition_descend(res, left, right); 
        quicksort_descend(res, left, q-1); 
        quicksort_descend(res, q+1, right); 
    } 
}

int partition_ascend(Nodess** res, int left, int right) { 
    int i = left - 1; 
    int j;
    for(j = left; j < right; j++) { 
        if(res[j]->value <= res[right]->value) { 
            i++; 
            swap(res[i], res[j]); 
        } 
    } 
    swap(res[i+1], res[right]); 
    return i+1; 
} 

void quicksort_ascend(Nodess** res, int left, int right) { 
    if(left < right) { 
        int q = partition_ascend(res, left, right); 
        quicksort_ascend(res, left, q-1); 
        quicksort_ascend(res, q+1, right); 
    } 
}

// run Iterative Deepening search, sort by value last iteration.
Nodess** all_possible_scoress_next_depth(Board board, Player player, Player opponent, Player* max_player, Player* min_player, bool isMax) {
    char color;
    Nodess** res = new Nodess*[ROW*COL];
    
    if(isMax == true) {
        color = player.get_color();
        // cout<<"color: "<<color<<endl;
        for(int i = 0; i < ROW; i++) {
            for(int j = 0; j < COL; j++) {
                res[i*COL+j]= new Nodess(i,j);
                //res[i*COL+j]->value = -INFINITY;
                // if cell can be placed by the player
                if(board.get_cell_color(i,j) == color || board.get_cell_color(i,j) == 'w') {
                    //cout<<"Max player place [ "<<i<<", "<<j<<" ] "<<board.get_cell_color(i,j)<<endl;
                    Board board_copy = copyss(board);
                    board_copy.place_orb(i,j,max_player);
                    res[i*COL+j]->value = evaluatess(board_copy, player); 
                }
            }
        }
        quicksort_descend(res,0,ROW*COL-1);
        // for(int i = 0; i < ROW*COL; i++) {
        //     if(res[i]->value  == 0){
        //         cout<<"[ "<<res[i]->row<<", "<<res[i]->col<<" ] ";
        //     }
        //     cout<<res[i]->value <<" ";
        // }
        
        // cout<<endl;
    }
    else {
        color = opponent.get_color();
        for(int i = 0; i < ROW; i++) {
            for(int j = 0; j < COL; j++) {
                res[i*COL+j]= new Nodess(i,j);
                // if cell can be placed by the player
                if(board.get_cell_color(i,j) == color || board.get_cell_color(i,j) == 'w') {
                    //cout<<"Min player place [ "<<i<<", "<<j<<" ] "<<board.get_cell_color(i,j)<<endl;
                    Board board_copy = copyss(board);
                    board_copy.place_orb(i,j,min_player);
                    res[i*COL+j]->value = evaluatess(board_copy, player); 
                }
            }
        }
        quicksort_ascend(res,0,ROW*COL-1);
    }
    // res is the result of depth + 1
    return res;
}


int minimaxss(Board board, int depth, Player player,Player opponent, int alpha, int beta, bool isMax) {
    // seperate our and oppponent's color 
    char max_round = player.get_color();
    char min_round = opponent.get_color();
    
    Player* max_player= new Player(max_round);
    Player* min_player= new Player(min_round);
    if(evaluatess(board,player)==100000) {
        return 100000;
    }
    if(evaluatess(board,player)==-100000) {
        return -100000;
    }
    // problem 1
    //cout<<"depth: "<<depth<<endl;
    // Base condition: leaf node always set to max, which implies depth must be an odd number
   // cout<<"value of isMax:"<< isMax <<endl;
    //board.print_current_board(-1, -1, 0);
    Nodess** score_list = all_possible_scoress_next_depth(board, player, opponent, max_player, min_player, isMax);
     //cout<<"pass score_list"<<endl;
    //Nodess* node;
    // if(score_list[0]->value == 100000) {
    //     return 100000;
    // }
    // if(score_list[0]->value == -100000) {
    //     return -100000;
    // }
    //Problem 
    if(depth == 4) {          
        Nodess* best = score_list[0];
        int best_score = best->value;
        //cout<<" color: "<<board.get_cell_color(best->row,best->col)<<" At ["<<best->row<<", "<<best->col<<" ]"<<endl;   
        return best_score;
    }
    if(isMax == true) {
        int best_max = -INFINITY;
        // Nodess* node;
        for(int i = 0; i < ROW*COL; i++) {
            if(board.get_cell_color(score_list[i]->row,score_list[i]->col) != min_round ){
                Board board_copy = copyss(board);
                board_copy.place_orb(score_list[i]->row,score_list[i]->col,max_player);
                //cout<<board_copy.get_cell_color(score_list[i]->row,score_list[i]->col)<<endl;
                    
                best_max = max(best_max, minimaxss(board_copy, depth+1, player, opponent, alpha, beta, false));
                alpha = max(alpha, best_max);   
                if (beta <= alpha) 
                break; 
            }
        }
        return best_max;     
    }
       
    else {
        int best_min = INFINITY;
        // Nodess* node;
        
        for(int i = 0; i < ROW*COL; i++) {
            if(board.get_cell_color(score_list[i]->row,score_list[i]->col) != max_round ) {
                Board board_copy = copyss(board);
                board_copy.place_orb(score_list[i]->row,score_list[i]->col,min_player);
                //cout<<board_copy.get_cell_color(score_list[i]->row,score_list[i]->col)<<endl;
                //Player next_round(opponent_round); 
                best_min = min(best_min, minimaxss(board_copy, depth+1,  player, opponent, alpha, beta, true));
                //cout<<"best_min: "<<best_min<<endl;
                beta = min(beta, best_min);
                if (beta <= alpha) 
                    break; 
            }
        }
        //cout<<"node->color: "<<board.get_cell_color(node->row,node->col)<<endl;
        return best_min;
    }
}

Nodess* find_best_movess(Board board, Player player) {
    // player_me is the Maximum player
    char me = player.get_color();
    char enemy;
    if(me == 'r') { enemy = 'b'; }
    else { enemy = 'r';}
    Player opponent(enemy); 

    Player* max_player= new Player(me);
    Player* min_player= new Player(enemy);
    // might cause violation
    int bestvalue = -INFINITY;
    Nodess* best_move =  new Nodess(-1,-1);
    // we operate minimax on each of the possible placement and return the best one (score wised) among them.
    Nodess** score_list = all_possible_scoress_next_depth(board, player, opponent, max_player, min_player, true);
    cout<<endl;
    for(int i = 0; i < ROW*COL; i++) {
        if(board.get_cell_color(score_list[i]->row,score_list[i]->col) != enemy ){
            
            Board board_copy = copyss(board);
            board_copy.place_orb(score_list[i]->row,score_list[i]->col,max_player);
           
            
            // operate minimax
            int alpha = -INFINITY;
            int beta =  INFINITY;
            int init_depth = 0;

            int move = minimaxss(board_copy, init_depth, player, opponent, alpha, beta, false);
            //cout<<"move: "<<move<<endl;
            if(move > bestvalue) {
                bestvalue = move;
                cout<<"best_move_value: "<<bestvalue<<endl;
                // bestvalue = move->value;
                best_move->row = score_list[i]->row;
                best_move->col = score_list[i]->col;
                //cout<<"the best move: [ "<<best_move->row<<", "<<best_move->col<<" ] and the corresponding best value is: "<<bestvalue<<" cell color: "<<board.get_cell_color(best_move->row,best_move->col)<<endl;
            }
        }
    }
    //cout<<"This is "<<player.get_color()<<" round"<<endl;
    cout<<"Return the best move: [ "<<best_move->row<<", "<<best_move->col<<" ] and the corresponding best value is: "<<bestvalue<<" cell color: "<<board.get_cell_color(best_move->row,best_move->col)<<endl;
    cout<<endl;
    return best_move;
}

void algorithm_C(Board board, Player player, int index[]){

    // we are the first one; first step get corner
    int count = 0;
    // for(int i = 0; i < ROW; i++) {
    //     for(int j = 0; j < COL; j++) {
    //         count += board.get_orbs_num(i,j);
    //     }
    // }
    // if(count == 0) {
    //     index[0] = 0;
    //     index[1] = 0;
    // }
    // other than the first step
    // else {
        Nodess* best = find_best_movess(board, player);
        index[0] = best->row;
        index[1] = best->col;
    //}
}