#include "../include/board.h"

Color currPlayer = WHITE;
Square board[boardSize][boardSize];
Square aiBoard[boardSize][boardSize];
bool isCastling = false;
bool isCapture = false;
Piece promotionPiece = NONE;
std::vector<int> kingPos = {0, 0};
std::vector<Move> lastMove = {Move(), Move()};
std::vector<MoveCase> lastMoveCase = {NO_MOVE, NO_MOVE};
bool rookMoved[numPlayers][2] = {false};
std::vector<int> rookStartingPos = {0, 0};
bool runningAICalcs = false;

Square *getBoardPosition(int number, int letter)
{
    if (runningAICalcs)
        return &aiBoard[number][letter];
    return &board[number][letter];
}

Color getPlayerColor(const int player)
{
    switch (player)
    {
    case 0:
        return WHITE;
    case 1:
        return BLACK;
    default:
        return NO_COLOR;
    }
}

bool isValidPieceArrangement(std::vector<Piece> pieceArrangement)
{
    int rooks = 0;
    int bishop = 0;
    for (int i = 0; i < boardSize; i++)
    {
        switch (pieceArrangement[i])
        {
            case ROOK:
                rooks++;
                if (rooks > 1)
                    return false;
                break;
            case KING:
                rooks--;
                if (rooks < 0)
                    return false;
                break;
            case BISHOP:
                if (bishop && i % 2 == bishop) // same color of square
                    return false;
                else
                    bishop = i % 2;
                break;
            default:
                break;
        }
    }
    return true;
}

void initializeBoard(const int setting)
{
    int i, j;
    std::vector<Piece> pieceArrangement = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };
    for (i = 0; i < numPlayers; i++)
    {
        lastMove[i] = Move(0, 0, NONE);
        lastMoveCase[i] = NO_MOVE;
        rookMoved[i][0] = rookMoved[i][1] = false;
    }
    currPlayer = WHITE;
    isCastling = false;
    isCapture = false;
    promotionPiece = NONE;
    if (setting > CHESS_REGULAR)
    {
        int minimumSwaps = 10;

        for (int nswaps = 0; (nswaps < minimumSwaps || (setting == CHESS_960 && !isValidPieceArrangement(pieceArrangement))); nswaps++)
        {
            i = rand() % boardSize;
            do
            {
                j = rand() % boardSize;
            } while (j == i);
            swap(pieceArrangement[i], pieceArrangement[j]);
        }
    }
    int rookNum = 0;
    for (i = 0; i < boardSize; i++)
    {
        for (j = 0; j < boardSize; j++)
        {
            Piece piece = pieceArrangement[j];
            switch (i)
            {
                default:
                    board[i][j].setPiece(NONE, NO_COLOR);
                    break;
                case 0: case 7:
                    board[i][j].setPiece(piece, (i < 2) ? WHITE : BLACK);
                    if (piece == KING)
                        kingPos[i == 7] = i + j * boardSize;
                    if (piece == ROOK)
                        rookStartingPos[rookNum++] = j;
                    break;
                case 1: case 6:
                    board[i][j].setPiece(PAWN, (i < 2) ? WHITE : BLACK);
                    break;
            }
        }
    }
}

void printBoard(void)
{
    std::cout << "--+---------+---------+---------+---------+---------+---------+---------+---------+";
    std::cout << "\n";
    for (int i = boardSize; i >= 1; i--)
    {
        // std::cout << "  |         |         |         |         |         |         |         |         |\n";
        std::cout << i << " | ";
        for (int j = 0; j < boardSize; j++)
        {
            if (board[i - 1][j].pieceIsWhite())
                std::cout << "White ";
            else if (board[i - 1][j].squareHasPiece())
                std::cout << "Black ";
            else
                std::cout << "      ";
            std::cout << board[i - 1][j].getPiece() << " | ";
        }
        std::cout << "\n";
        // std::cout << "  |         |         |         |         |         |         |         |         |\n";
        std::cout << "--+---------+---------+---------+---------+---------+---------+---------+---------+";
        std::cout << "\n";
    }
    std::cout << "  |    a    |    b    |    c    |    d    |    e    |    f    |    g    |    h    |\n";
    std::cout << "\n";
}

bool checkKingDanger(int numberFrom, int letterFrom, int numberTo, int letterTo, const int playerToCheck)
{
    // Moving piece away would cause King to be in check?
    Square temp = board[numberFrom][letterFrom];
    if (!isKingInCheck(playerToCheck))
    {
        board[numberFrom][letterFrom].setEmpty();
        if (isKingInCheck(playerToCheck))
        {
            board[numberFrom][letterFrom] = temp;
            return true;
        }
    }
    board[numberFrom][letterFrom].setEmpty();
    int kingPosition = kingPos[playerToCheck];
    if (temp.getPiece() == KING)
        kingPos[playerToCheck] = numberTo + letterTo * boardSize;

    // Moving piece to this spot doesn't prevent check?
    Square temp2 = board[numberTo][letterTo];
    board[numberTo][letterTo] = temp;
    if (isKingInCheck(playerToCheck))
    {
        board[numberFrom][letterFrom] = temp;
        board[numberTo][letterTo] = temp2;
        kingPos[playerToCheck] = kingPosition;
        return true;
    }
    board[numberFrom][letterFrom] = temp;
    board[numberTo][letterTo] = temp2;
    kingPos[playerToCheck] = kingPosition;
    return false;
}

bool hasMovesLeft(const int playerToCheck)
{
    int startingNums = 0;
    int startingLets = 0;
    int endingNums = boardSize - 1;
    int endingLets = boardSize - 1;
    Square temp;
    Square temp2;

    for (int square = 0; square < boardSize * boardSize; square++)
    {
        int number = square % boardSize;
        int letter = square / boardSize;
        for (int i = startingNums; i <= endingNums; i++)
        {
            for (int j = startingLets; j <= endingLets; j++)
            {
                if (i == number && j == letter)
                    continue;
                if (!board[i][j].squareHasPiece() || board[i][j].pieceIsWhite() != (playerToCheck == WHITE))
                    continue;

                if (checkKingDanger(i, j, number, letter, playerToCheck))
                    continue;

                if (canMove(i, j, number, letter, false))
                    return true;
            }
        }
    }
    return false;
}

int getCastlingRookLetter(int numbersFrom, int lettersFrom, int kingLettersTo)
{
    bool fromIsWhite = board[numbersFrom][lettersFrom].pieceIsWhite();
    bool movingToC = kingLettersTo == LETTER_c;
    int lettersFromRook = movingToC ? LETTER_a : LETTER_h;

    while (lettersFromRook != rookStartingPos[!movingToC] || fromIsWhite != board[numbersFrom][lettersFromRook].pieceIsWhite())
    {
        if (movingToC)
            lettersFromRook++;
        else
            lettersFromRook--;
    }
    return lettersFromRook;
}

bool isCastlingMovementInCheck(Move m, const int playerToCheck)
{
    int kingPosition = kingPos[playerToCheck];
    int numbersFrom = kingPosition % 8;
    int lettersFrom = kingPosition / 8;
    int lettersFromRook = getCastlingRookLetter(numbersFrom, lettersFrom, m.lettersTo);
    int lettersToRook = (m.lettersTo == LETTER_c) ? LETTER_d : LETTER_f;
    int rangeMin = min(lettersFrom, m.lettersTo);
    int rangeMax = max(lettersFrom, m.lettersTo);

    for (int letterToCheck = rangeMin; letterToCheck <= rangeMax; letterToCheck++)
    {
        if (isSquareInCheck(numbersFrom, letterToCheck, playerToCheck))
            return true;
    }
    return false;
}

bool isKingInCheck(const int playerToCheck)
{
    int kingPosition = kingPos[playerToCheck];
    return isSquareInCheck(kingPosition % 8, kingPosition / 8, playerToCheck);
}

bool isSquareInCheck(int number, int letter, const int playerAction)
{
    int startingNums = 0;
    int startingLets = 0;
    int endingNums = boardSize - 1;
    int endingLets = boardSize - 1;
    bool isKingWhite = playerAction == WHITE;
    for (int i = startingNums; i <= endingNums; i++)
    {
        for (int j = startingLets; j <= endingLets; j++)
        {
            if (i == number && j == letter)
                continue;
            if (board[i][j].pieceIsWhite() == isKingWhite)
                continue;
            if (canMove(i, j, number, letter, false))
                return true;
        }
    }
    return false;
}

bool isMovementObstructedCastling(int numbersFrom, int lettersFrom, int numbersTo, int lettersTo, int letterException)
{
    int additiveNums = signal(numbersTo - numbersFrom);
    int additiveLets = signal(lettersTo - lettersFrom);
    Piece pieceFrom = board[numbersFrom][lettersFrom].getPiece();
    if (numbersFrom == numbersTo || lettersFrom == lettersTo)
    {
        for (int x = numbersFrom; x != numbersTo + additiveNums; x += additiveNums)
        {
            for (int y = lettersFrom; y != lettersTo + additiveLets; y += additiveLets)
            {
                if (x == numbersFrom && y == lettersFrom)
                    continue;
                if (y == letterException)
                    continue;
                if (x == numbersTo && y == lettersTo
                 && pieceFrom != PAWN
                 && ((letterException != boardSize
                   && board[x][y].squareHasPiece()
                   && board[x][y].pieceIsWhite() != board[numbersFrom][lettersFrom].pieceIsWhite())
                  || letterException == boardSize))
                    return false;
                if (board[x][y].squareHasPiece())
                    return true;
            }
        }
    }
    else
    {
        for (int i = 1; additiveNums * i + numbersFrom != numbersTo + additiveNums; i++)
        {
            int x = additiveNums * i + numbersFrom;
            int y = additiveLets * i + lettersFrom;
            if (x == numbersTo)
                return false;
            if (board[x][y].squareHasPiece())
                return true;
        }
    }
    return false;    
}

bool isMovementObstructed(int numbersFrom, int lettersFrom, int numbersTo, int lettersTo)
{
    return isMovementObstructedCastling(numbersFrom, lettersFrom, numbersTo, lettersTo, boardSize);
}

bool canKnightMove(int distNums, int distLets)
{
    if (distNums < 1)
        return false;
    if (distLets < 1)
        return false;
    if (distNums + distLets != 3)
        return false;
    return true;
}

bool canMove(int numbersFrom, int lettersFrom, int numbersTo, int lettersTo, bool doingMove)
{
    Square from = board[numbersFrom][lettersFrom];
    Color fromColor = from.getColor();
    bool fromIsWhite = fromColor == WHITE;
    Square to = board[numbersTo][lettersTo];
    Piece piece = board[numbersFrom][lettersFrom].getPiece();
    int opponent = getOpponent(currPlayer);

    // Square to move to is occupied by allied piece
    if ((piece != KING || !isCastling)
     && to.squareHasPiece()
     && to.pieceIsWhite() == fromIsWhite)
        return false;

    switch (piece)
    {
        case PAWN:
            if ((numbersFrom == 1 || numbersFrom == 6)
             && getTrueNumDist(numbersFrom, numbersTo, fromIsWhite) == 2
             && lettersFrom == lettersTo
             && ((numbersFrom == 1) == fromIsWhite)
             && !isMovementObstructed(numbersFrom, lettersFrom, numbersTo, lettersTo))
            {
                if (doingMove)
                    lastMoveCase[currPlayer] = PAWN_2;
                return true;
            }
            else if (getTrueNumDist(numbersFrom, numbersTo, fromIsWhite) == 1
                  && lettersFrom == lettersTo
                  && !to.squareHasPiece())
                return true;
            else if (getTrueNumDist(numbersFrom, numbersTo, fromIsWhite) == 1
                  && moduleOf(lettersFrom - lettersTo) == 1
                  && to.squareHasPiece()
                  && to.pieceIsWhite() != fromIsWhite)
                return true;
            else if (lastMoveCase[opponent] == PAWN_2
                  && lastMove[opponent].piece == PAWN
                  && moduleOf(lettersFrom - lettersTo) == 1
                  && lettersTo == lastMove[opponent].lettersTo
                  && numbersFrom == lastMove[opponent].numbersTo
                  && fromIsWhite != board[numbersFrom][lettersTo].pieceIsWhite())
            {
                if (doingMove)
                    board[numbersFrom][lettersTo].setEmpty();
                return true;
            }
            return false;
            break;
        case KNIGHT:
            return canKnightMove(moduleOf(numbersFrom - numbersTo), moduleOf(lettersFrom - lettersTo));
            break;
        case KING:
            if (isCastling
             && numbersFrom - numbersTo == 0)
            {
                int lettersToRook = lettersTo == LETTER_c ? LETTER_d : LETTER_f;
                int lettersFromRook = getCastlingRookLetter(numbersFrom, lettersFrom, lettersTo);
                // Rook already moved
                if (rookMoved[currPlayer][lettersTo != LETTER_c])
                    return false;
                // No movement
                if (lettersFromRook == lettersToRook && lettersFrom == lettersTo)
                    return false;
                // King movement is obstructed
                if (isMovementObstructedCastling(numbersFrom, lettersFrom, numbersTo, lettersTo, lettersFromRook))
                    return false;
                // Rook movement is obstructed
                if (isMovementObstructedCastling(numbersFrom, lettersFromRook, numbersTo, lettersToRook, lettersFrom))
                    return false;
                if (doingMove)
                {
                    board[numbersFrom][lettersToRook].setPiece(ROOK, fromColor);
                    if (lettersToRook != lettersFromRook)
                        board[numbersFrom][lettersFromRook].setEmpty();
                }
                return true;
            }
            else if (moduleOf(numbersFrom - numbersTo) > 1 || moduleOf(lettersFrom - lettersTo) > 1)
                return false;
            // fallthrough
        case QUEEN:
        case ROOK:
            if ((numbersFrom - numbersTo == 0 || lettersFrom - lettersTo == 0)
             && !isMovementObstructed(numbersFrom, lettersFrom, numbersTo, lettersTo))
                return true;
            if (piece == ROOK)
                return false;
            // fallthrough
        case BISHOP:
            if (moduleOf(numbersFrom - numbersTo) == moduleOf(lettersFrom - lettersTo)
             && !isMovementObstructed(numbersFrom, lettersFrom, numbersTo, lettersTo))
                return true;
            return false;
        default:
            break;
    }
    return false;
}

bool tryMove(int numbersFrom, int lettersFrom, Move move, MoveErrorCode& err)
{
    if (board[numbersFrom][lettersFrom].getPiece() != move.piece || board[numbersFrom][lettersFrom].pieceIsWhite() != (playerIsWhite(currPlayer)))
        return false;

    if (!canMove(numbersFrom, lettersFrom, move.numbersTo, move.lettersTo, false))
        return false;

    if (checkKingDanger(numbersFrom, lettersFrom, move.numbersTo, move.lettersTo, currPlayer))
    {
        if (canMove(numbersFrom, lettersFrom, move.numbersTo, move.lettersTo, false))
        {
            if (isKingInCheck(currPlayer))
                err = MOVE_DOESNT_PREVENT_CHECK;
            else
                err = MOVE_OPENS_CHECK;
        }
        return false;
    }

    return true;
}

Piece interpretPiece(std::string input)
{
    if (input[0] == '\0')
        return NONE;
    switch (input[0])
    {
        // case 'P':
        //     return PAWN;
        // case 'K':
        //     return KING;
        case 'Q':
            return QUEEN;
        case 'N':
            return KNIGHT;
        case 'B':
            return BISHOP;
        case 'R':
            return ROOK;
        default:
            return NONE;
    }
}

void interpretMove(Move m)
{
    int numbersMoveFrom = boardSize, lettersMoveFrom = boardSize;
    int startingNums = m.numbersFrom != boardSize ? m.numbersFrom : 0;
    int startingLets = m.lettersFrom != boardSize ? m.lettersFrom : 0;
    int endingNums = m.numbersFrom != boardSize ? m.numbersFrom : boardSize - 1;
    int endingLets = m.lettersFrom != boardSize ? m.lettersFrom : boardSize - 1;
    MoveErrorCode errorCode = NO_ERROR;

    if (startingNums < 0 || startingLets < 0 || endingNums < 0 || endingLets < 0 || startingNums >= boardSize || startingLets >= boardSize || endingNums >= boardSize || endingLets >= boardSize)
        ERROR_MACRO("Move out of board bounds.");
    if (m.numbersTo < 0 || m.lettersTo < 0 || m.numbersTo >= boardSize || m.lettersTo >= boardSize)
        ERROR_MACRO("Move out of board bounds.");
    if (isCastling && rookMoved[currPlayer][m.lettersTo == LETTER_g])
        ERROR_MACRO(m << "Rook or King has already moved.");
    if (isCastling && isCastlingMovementInCheck(m, currPlayer))
        ERROR_MACRO(m << "Move goes through Check.");
    if (m.piece == KING && isSquareInCheck(m.numbersTo, m.lettersTo, currPlayer))
        ERROR_MACRO(m << "Moving to that space would put you in Check.");

    for (int i = startingNums; i <= endingNums; i++)
    {
        for (int j = startingLets; j <= endingLets; j++)
        {
            if (tryMove(i, j, m, errorCode))
            {
                if (numbersMoveFrom != boardSize)
                    ERROR_MACRO(m << "Move specified is too vague, please include the coordinates of the piece you want to move.");
                numbersMoveFrom = i;
                lettersMoveFrom = j;
            }
        }
    }
    
    if (numbersMoveFrom == boardSize)
    {
        if (board[m.numbersTo][m.lettersTo].squareHasPiece() && board[m.numbersTo][m.lettersTo].pieceIsWhite() == playerIsWhite(currPlayer))
            ERROR_MACRO(m << "Space is occupied by a piece of the same color.");

        switch (errorCode)
        {
            case NO_ERROR:
                ERROR_MACRO(m << "Couldn't find the specified move.");
                break;
            case MOVE_OPENS_CHECK:
                ERROR_MACRO(m << "Move would open Check.");
                break;
            case MOVE_DOESNT_PREVENT_CHECK:
                ERROR_MACRO(m << "Move does not get out of Check.");
                break;
        }
    }
    
    if (isCapture != (board[m.numbersTo][m.lettersTo].squareHasPiece() && board[m.numbersTo][m.lettersTo].pieceIsWhite() != playerIsWhite(currPlayer)))
        ERROR_MACRO(m << "Wrong indication of whether move is a capture or not");

    lastMove[currPlayer] = Move(0, 0, NONE);
    lastMoveCase[currPlayer] = NO_MOVE;
    canMove(numbersMoveFrom, lettersMoveFrom, m.numbersTo, m.lettersTo, true);
    board[m.numbersTo][m.lettersTo].setPiece(m.piece, getPlayerColor(currPlayer));
    if (board[numbersMoveFrom][lettersMoveFrom].getPiece() == m.piece)
        board[numbersMoveFrom][lettersMoveFrom].setEmpty();

    if (m.piece == PAWN && m.numbersTo == (playerIsWhite(currPlayer) * 7))
    {
        while (promotionPiece == NONE)
        {
            std::cout << "Select promotion piece: ";
            std::string input;
            std::cin >> input;
            std::cout << "\n";
            promotionPiece = interpretPiece(input);
        }
        board[m.numbersTo][m.lettersTo].setPiece(promotionPiece, getPlayerColor(currPlayer));
    }
    else if (m.piece == KING)
    {
        rookMoved[currPlayer][0] = rookMoved[currPlayer][1] = true;
        kingPos[currPlayer] = m.numbersTo + m.lettersTo * boardSize;
    }
    else if (m.piece == ROOK && numbersMoveFrom % 7 == 0)
    {
        if (lettersMoveFrom == rookStartingPos[0])
            rookMoved[currPlayer][0] = true;
        else if (lettersMoveFrom == rookStartingPos[1])
            rookMoved[currPlayer][1] = true;
    }
    else if (m.numbersTo == (currPlayer ^ 1) * 7)
    {
        if (m.lettersTo == rookStartingPos[0])
            rookMoved[currPlayer ^ 1][0] = true;
        else if (m.lettersTo == rookStartingPos[1])
            rookMoved[currPlayer ^ 1][1] = true;
    }
    lastMove[currPlayer] = m;
    currPlayer = getPlayerColor(getOpponent(currPlayer));
    isCastling = false;
    isCapture = false;
    promotionPiece = NONE;
    if (!hasMovesLeft(currPlayer))
    {
        if (isKingInCheck(currPlayer))
            std::cout << "\nCHECKMATE! " << (getPlayerName(getOpponent(currPlayer))) << " wins!\n\n";
        else
            std::cout << "\nSTALEMATE! " << (getPlayerName(getOpponent(currPlayer))) << " has no moves left!\n\n";
    }
    else if (isKingInCheck(currPlayer))
    {
        std::cout << "\nCHECK!\n\n";
    }
}

Move interpretInput(std::string input)
{
    Move move;
    int eval = 0;
    isCastling = false;
    isCapture = false;
    promotionPiece = NONE;
    if (input[eval] == '\0')
        return move;
    switch (input[eval])
    {
        case '0':
        case 'o':
        case 'O':
            // Castling
            move.piece = KING;
            isCastling = true;
            break;
        case 'K':
            move.piece = KING;
            break;
        case 'Q':
            move.piece = QUEEN;
            break;
        case 'R':
            move.piece = ROOK;
            break;
        case 'N':
            move.piece = KNIGHT;
            break;
        case 'B':
            move.piece = BISHOP;
            break;
        default:
            move.piece = PAWN;
            break;
    }
    if (move.piece != PAWN)
        eval++;

    if (isCastling)
    {
        if (input[eval++] == '-' && input[eval++] == input[0])
        {
            // Verify if short or long castling
            if (input[eval] == '\0')
            {
                move.lettersTo = LETTER_g;
                move.numbersTo = (currPlayer != 0) * 7;
            }
            else if (input[eval++] == '-' && input[eval++] == input[0] && input[eval] == '\0')
            {
                move.lettersTo = LETTER_c;
                move.numbersTo = (currPlayer != 0) * 7;
            }
        }
        else
        {
            isCastling = false;
        }
        return move;
    }
    if (input[eval] == 'x')
    {
        isCapture = true;
        eval++;
    }
    else if (input[eval] != '\0' && (isDesambiguationChar(input[eval + 1]) || (input[eval + 1] != '\0' && isDesambiguationChar(input[eval + 2]))))
    {
        // Desambiguation
        if (input[eval] >= 'a' && input[eval] <= 'h')
        {
            move.lettersFrom = input[eval++] - 'a';
        }
        if (input[eval] >= '1' && input[eval] <= '8')
        {
            move.numbersFrom = input[eval++] - '1';
        }
        if (isDesambiguationChar(input[eval]))
        {
            if (input[eval] == 'x')
                isCapture = true;
            eval++;
        }
    }
    if (input[eval] >= 'a' && input[eval] <= 'h')
    {
        move.lettersTo = input[eval++] - 'a';
    }
    if (input[eval] >= '1' && input[eval] <= '8')
    {
        move.numbersTo = input[eval++] - '1';
    }
    // Promotion
    if (move.piece == PAWN && move.numbersTo == (playerIsWhite(currPlayer) * 7) && input[eval++] == '=')
    {
        promotionPiece = interpretPiece(input[eval] + " ");
    }

    return move;
}
