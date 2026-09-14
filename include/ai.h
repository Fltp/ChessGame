#ifndef AI_H
#define AI_H

#include "board.h"

extern std::vector<bool> hasAI;
extern std::vector<Move> possibleMoves;
extern Square aiBoard[boardSize][boardSize];
extern int aiSearchDepth;

Move getBestMove(void);
int getScoreForPosition(int playerToCheck);
int negamax(int playerToMove, int depth, int alpha, int beta);
std::vector<Move> generatePossibleMoves(int playerToCheck);
void fillWithPossibleMoves(int playerToCheck);
void simulateMove(Move m);
void loadAiBoard(void);


#endif // AI_H