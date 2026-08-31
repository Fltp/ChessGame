#include "main.h"

int main()
{
    int setting = CHESS_REGULAR;
    std::string input;
    std::cout << "Play Chess 960? ";
    std::cin >> input;
    if (input == "Yes" || input == "yes" || input == "y" || input == "Y")
        setting = CHESS_960;
    else if (input == "Fischer" || input == "fischer")
        setting = CHESS_SECRET;
    srand(time(NULL));
    initializeBoard(setting);
    for (int i = 0; i < numPlayers; i++)
    {
        // std::cout << "Should " << getPlayerName(i) << " be controlled by the computer?\n";
        // std::cin >> input;
        // if (input == "Yes" || input == "yes" || input == "y" || input == "Y")
        //     hasAI[i] = true;
        // else
            hasAI[i] = false;
    }
    printBoard();
    while (hasMovesLeft(currPlayer))
    {
        int playerTurn = (int)currPlayer;
        if (!hasAI[playerTurn])
        {
            std::cout << getPlayerName(playerTurn) << "'s turn. Choose a move.\n";
            while (playerTurn == (int)currPlayer)
            {
                std::cin >> input;
                interpretMove(interpretInput(input));
            }
        }
        else
        {
            runningAICalcs = true;
            interpretMove(getBestMove());
            runningAICalcs = false;
        }
        printBoard();
    }
    return 0;
}
