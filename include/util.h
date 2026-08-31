#ifndef UTIL_H
#define UTIL_H

#include "square.h"

void swap(Piece& a, Piece& b);
int getTrueNumDist(int a, int b, bool isWhite);
int min(int a, int b);
int max(int a, int b);
int moduleOf(int a);
int signal(int a);
int getOpponent(int player);
bool playerIsWhite(int player);
std::string getPlayerName(int player);
bool isDesambiguationChar(char c);

#endif // UTIL_H