#include "board.h"
#include "rules.h"
#ifndef LEVEL
    #define LEVEL 4
#endif

#define WEST -1
#define EAST 1
#define NORTH 1
#define SOUTH -1

#define EASY_BOARD_EVALUATOR 0
#define HARD_BOARD_EVALUATOR 1

class TreeNode{
    private:
        Board boardState;
        TreeNode *nextNode;
        TreeNode *nextRoundNode;
        int index[2];    // Record which orb result in this TreeNode
    public:
        TreeNode();
        TreeNode(Board board);
        TreeNode(Board board, int x, int y);
        void setNextNode(TreeNode* TreeNode);
        void setNextRoundNode(TreeNode *TreeNode);
        TreeNode* getNextNode();
        TreeNode* getNextRoundNode();
        TreeNode* getTail();
        Board getBoardState();
        int getX();
        int getY();
};

class gameTree{
    private:
        TreeNode *root;
        Player *player;
        Player *opponent;
    public:
        gameTree(Board board, Player *player);
        TreeNode* chooseOrb(int difficulty);   // Use the four-level look-ahead game tree to determine the next step of orb
        int insertNextRound(TreeNode *Node, Player currentTurn);
        int levelLookAhead();  // Add a new level of look-ahead in gameTree
        int numAtLevel(TreeNode *Node); 
        float BoardEvaluator(TreeNode *Node, int difficulty);
};