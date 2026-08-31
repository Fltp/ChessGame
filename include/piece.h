#ifndef PIECE_H
#define PIECE_H

#include <iostream>

typedef enum {
    NONE,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
} Piece;

std::ostream& operator<< (std::ostream& out, const Piece& p);

#endif // PIECE_H