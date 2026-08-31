#include "../include/piece.h"

std::ostream& operator<< (std::ostream& out, const Piece& p)
{
    switch (p)
    {
        case NONE:
            out << " ";
            break;
        case PAWN:
            out << "P";
            break;
        case ROOK:
            out << "R";
            break;
        case KNIGHT:
            out << "N";
            break;
        case BISHOP:
            out << "B";
            break;
        case QUEEN:
            out << "Q";
            break;
        case KING:
            out << "K";
            break;
    }
    return out;
}