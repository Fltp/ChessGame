#include "../include/ai.h"

std::vector<bool> hasAI = {false, false};
std::vector<Move> possibleMoves;
int aiSearchDepth = 4;

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

int pieceValue(Piece p)
{
    switch (p)
    {
        case PAWN:      return 1;
        case KNIGHT:    return 3;
        case BISHOP:    return 3;
        case ROOK:      return 5;
        case QUEEN:     return 9;
        case KING:      return 100;
        default:
            break;
    }
    return 0;
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
            if (!aiBoard[i][j].squareHasPiece())
                continue;

            bool isPlayerPiece = aiBoard[i][j].pieceIsWhite() == (playerToCheck == WHITE);
            score += pieceValue(aiBoard[i][j].getPiece()) * (isPlayerPiece ? 1 : -1);
        }
    }

    if (score < 0 && !hasMovesLeft(opponent)) // go for Stalemate if it sees a deficit of pieces
        return 0;
    return score;
}

std::vector<Move> generatePossibleMoves(int playerToCheck)
{
    std::vector<Move> moves;
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (!aiBoard[i][j].squareHasPiece() || aiBoard[i][j].pieceIsWhite() != (playerToCheck == WHITE))
                continue;

            for (int number = 0; number < boardSize; number++)
            {
                for (int letter = 0; letter < boardSize; letter++)
                {
                    if (!canMove(i, j, number, letter, false))
                        continue;
                    if (checkKingDanger(i, j, number, letter, playerToCheck))
                        continue;
                    moves.push_back(Move(i, j, number, letter, aiBoard[i][j].getPiece()));
                }
            }
        }
    }
    return moves;
}

void fillWithPossibleMoves(int playerToCheck)
{
    possibleMoves = generatePossibleMoves(playerToCheck);
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

void restoreSimulatedMove(Move m, Square savedSquare)
{
    int fromRow = m.numbersFrom;
    int fromCol = m.lettersFrom;
    int toRow   = m.numbersTo;
    int toCol   = m.lettersTo;

    aiBoard[fromRow][fromCol] = aiBoard[toRow][toCol];
    aiBoard[toRow][toCol] = savedSquare;
}

int negamax(int playerToMove, int depth, int alpha, int beta)
{
    if (depth == 0)
        return getScoreForPosition(playerToMove);

    std::vector<Move> moves = generatePossibleMoves(playerToMove);
    if (moves.empty())
        return isKingInCheck(playerToMove) ? -100000 : 0; // checkmate vs stalemate

    int best = -1000001;
    for (const Move& m : moves)
    {
        Square savedSquare = aiBoard[m.numbersTo][m.lettersTo];
        simulateMove(m);

        int score = -negamax(getOpponent(playerToMove), depth - 1, -beta, -alpha);

        restoreSimulatedMove(m, savedSquare);

        if (score > best)
            best = score;
        if (best > alpha)
            alpha = best;

        // Opponent already has a better option elsewhere, stop searching this branch
        if (alpha >= beta)
            break;
    }
    return best;
}

Move getBestMove(void)
{
    loadAiBoard();
    possibleMoves = generatePossibleMoves(currPlayer);

    int alpha = -1000001;
    int beta = 1000001;
    std::vector<Move> bestMoves;

    for (const Move& move : possibleMoves)
    {
        Square savedSquare = aiBoard[move.numbersTo][move.lettersTo];
        simulateMove(move);

        int score = -negamax(getOpponent(currPlayer), aiSearchDepth, -beta, -alpha);

        restoreSimulatedMove(move, savedSquare);

        if (score > alpha)
        {
            alpha = score;
            bestMoves.clear();
            bestMoves.push_back(move);
        }
        else if (score == alpha)
        {
            bestMoves.push_back(move);
        }
    }

    if (bestMoves.empty())
        return Move();

    return bestMoves[rand() % bestMoves.size()];
}
