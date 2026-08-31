#include "ai.h"

std::vector<bool> hasAI = {false, false};
std::vector<Move> possibleMoves;

void loadAiBoard(void)
{
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            aiBoard[i][j] = board[i][j];
        }
    }
}

int getScoreForPosition(const int playerToCheck)
{
    int score = 0;
    int opponent = getOpponent(playerToCheck);

    if (!hasMovesLeft(opponent) && isKingInCheck(opponent)) // results in mate
        return 100000;

    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            switch (aiBoard[i][j].getPiece())
            {
                case PAWN:
                    score += 1 * (playerToCheck == aiBoard[i][j].pieceIsWhite() ? -1 : 1);
                    break;
                case BISHOP:
                case KNIGHT:
                    score += 3 * (playerToCheck == aiBoard[i][j].pieceIsWhite() ? -1 : 1);
                    break;
                case ROOK:
                    score += 5 * (playerToCheck == aiBoard[i][j].pieceIsWhite() ? -1 : 1);
                    break;
                case QUEEN:
                    score += 9 * (playerToCheck == aiBoard[i][j].pieceIsWhite() ? -1 : 1);
                    break;
                case KING:
                    score += 100 * (playerToCheck == aiBoard[i][j].pieceIsWhite() ? -1 : 1);
                    break;
                default:
                    break;
            }
        }
    }

    if (score < 0 && !hasMovesLeft(opponent)) // go for Stalemate if it sees a deficit of pieces
        return 0;
    return score;
}

void simulateMove(Move m)
{
    int fromRow = m.numbersFrom;
    int fromCol = m.lettersFrom;
    int toRow   = m.numbersTo;
    int toCol   = m.lettersTo;

    aiBoard[toRow][toCol] = aiBoard[fromRow][fromCol];
    aiBoard[fromRow][fromCol] = Square();
}

int getScoreForMove(Move m)
{
    int numbersFrom = m.numbersFrom;
    int lettersFrom = m.lettersFrom;
    int numbersTo = m.numbersTo;
    int lettersTo = m.lettersTo;

    if (checkKingDanger(numbersFrom, lettersFrom, numbersTo, lettersTo, currPlayer)) // results in mate for self
        return -100000;

    Square savedSquare = aiBoard[numbersTo][lettersTo];

    simulateMove(m);

    int score = getScoreForPosition(currPlayer);

    aiBoard[numbersFrom][lettersFrom] = aiBoard[numbersTo][lettersTo];
    aiBoard[numbersTo][lettersTo] = savedSquare;
    return score;
}

void fillWithPossibleMoves(int playerToCheck)
{
    possibleMoves.clear();
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            for (int number = 0; number < boardSize; number++)
            {
                for (int letter = 0; letter < boardSize; letter++)
                {
                    if (!canMove(i, j, number, letter, false))
                        continue;

                    if (checkKingDanger(i, j, number, letter, playerToCheck))
                        continue;

                    possibleMoves.push_back(Move(i, j, number, letter, aiBoard[i][j].getPiece()));
                }
            }
        }
    }
}

Move getBestMove(void)
{
    Move chosen, bestMove = Move();
    int moveScoreMax = -100001, newMoveScore;
    loadAiBoard();
    fillWithPossibleMoves(currPlayer);

    for (const Move& move : possibleMoves)
    {
        int score = getScoreForMove(move);

        if (score > moveScoreMax)
        {
            moveScoreMax = score;
            bestMove = move;
        }
    }
    return bestMove;
}
