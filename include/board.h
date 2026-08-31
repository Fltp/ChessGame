#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>
#include "piece.h"
#include "move.h"
#include "square.h"
#include "util.h"
#include "consts.h"

#define ERROR_MACRO(string)     { std::cout << "Error: " << string << "\n\n"; return; }

enum ChessSettings {
    CHESS_REGULAR,
    CHESS_960,
    CHESS_SECRET,
};

extern Color currPlayer;
extern bool isCastling;
extern bool isCapture;
extern Piece promotionPiece;
extern Square board[boardSize][boardSize];
extern Square aiBoard[boardSize][boardSize];
extern std::vector<int> kingPos;
extern std::vector<Move> lastMove;
extern std::vector<MoveCase> lastMoveCase;
extern bool rookMoved[numPlayers][2];
extern std::vector<int> rookStartingPos;
extern bool runningAICalcs;

Square *getBoardPosition(int number, int letter);
Color getPlayerColor(const int player);
void initializeBoard(const int setting);
void printBoard(void);
bool checkKingDanger(int numberFrom, int letterFrom, int numberTo, int letterTo, int playerToCheck);
bool hasMovesLeft(const int playerToCheck);
int getCastlingRookLetter(int numbersFrom, int lettersFrom, int kingLettersTo);
bool isCastlingMovementInCheck(Move m, int playerToCheck);
bool isKingInCheck(int playerToCheck);
bool isSquareInCheck(int number, int letter, int playerAction);
bool isMovementObstructedCastling(int numbersFrom, int lettersFrom, int numbersTo, int lettersTo, int letterException);
bool isMovementObstructed(int numbersFrom, int lettersFrom, int numbersTo, int lettersTo);
bool canKnightMove(int distNums, int distLets);
bool canMove(int numbersFrom, int lettersFrom, int numbersTo, int lettersTo, bool doingMove);
bool tryMove(int numbersFrom, int lettersFrom, Move move, MoveErrorCode& err);
Piece interpretPiece(std::string input);
void interpretMove(Move m);
Move interpretInput(std::string input);

#endif // BOARD_H