#ifndef AI_H
#define AI_H

#include "board.h"

extern std::vector<bool> hasAI;
extern std::vector<Move> possibleMoves;
extern Square aiBoard[boardSize][boardSize];

void alphaBeta(void);
Move getBestMove(void);
int getScoreForPosition(int playerToCheck);
int getScoreForMove(Move m);
void fillWithPossibleMoves(int playerToCheck);
void simulateMove(Move m);
void loadAiBoard(void);


#endif // AI_H