#ifndef MOVE_H
#define MOVE_H

#include "piece.h"
#include "consts.h"

class Move {
    public:
    int numbersFrom;
    int lettersFrom;
    int numbersTo;
    int lettersTo;
    Piece piece;
    Move(void);
    Move(int numbersTo, int lettersTo, Piece piece);
    Move(int numbersFrom, int lettersFrom, int numbersTo, int lettersTo, Piece piece);
    friend std::ostream& operator<< (std::ostream& out, const Move& m);
};

enum MoveCase {
    NO_MOVE,
    PAWN_2,
};

enum MoveErrorCode {
    NO_ERROR,
    MOVE_OPENS_CHECK,
    MOVE_DOESNT_PREVENT_CHECK,
};

#endif // MOVE_H