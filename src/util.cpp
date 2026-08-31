#include "util.h"

void swap(Piece& a, Piece& b)
{
    Piece temp = a;
    a = b;
    b = temp;
}

int getTrueNumDist(int a, int b, bool isWhite)
{
    if (isWhite)
        return b - a;
    return a - b;
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

int moduleOf(int a)
{
    if (a < 0)
        return -a;
    return a;
}

int signal(int a)
{
    if (a < 0)
        return -1;
    return 1;
}

int getOpponent(int player)
{
    return player ^ 1;
}

bool playerIsWhite(int player)
{
    return player == WHITE;
}

std::string getPlayerName(int player)
{
    if (playerIsWhite(player))
        return "White";
    return "Black";
}

bool isDesambiguationChar(char c)
{
    return c == '-' || c == 'x';
}