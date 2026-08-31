#include "../include/move.h"

Move::Move(void) : numbersFrom(boardSize), lettersFrom(boardSize), numbersTo(boardSize + 1), lettersTo(boardSize + 1), piece(NONE) {}
Move::Move(int numbersTo, int lettersTo, Piece piece) : numbersFrom(boardSize), lettersFrom(boardSize), numbersTo(numbersTo), lettersTo(lettersTo), piece(piece) {}
Move::Move(int numbersFrom, int lettersFrom, int numbersTo, int lettersTo, Piece piece) : numbersFrom(numbersFrom), lettersFrom(lettersFrom), numbersTo(numbersTo), lettersTo(lettersTo), piece(piece) {}

std::ostream& operator<< (std::ostream& out, const Move& m)
{
    out << "Move: " << m.piece << " moves to " << (char)(m.lettersTo + 'a') << (m.numbersTo + 1) << " - ";
    return out;
}
