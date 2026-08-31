#ifndef SQUARE_H
#define SQUARE_H

#include "piece.h"

enum Color {
    WHITE,
    BLACK,
    NO_COLOR,
};

class Square {
    Piece piece;
    Color color;
    public:
    Square() { piece = NONE; color = NO_COLOR; }
    void setPiece(Piece p, Color c) { piece = p; color = c; }
    void setEmpty() { setPiece(NONE, NO_COLOR); }
    Piece getPiece() { return piece; }
    Color getColor() { return color; }
    bool pieceIsWhite() { return color == WHITE; }
    bool squareIsValid() { return color != NO_COLOR; }
    bool squareHasPiece() { return color != NO_COLOR && piece != NONE; }
};

#endif // SQUARE_H