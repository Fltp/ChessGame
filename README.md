# ChessGame
A simple command-line chess game, featuring turn-based gameplay, legal move validation, a basic implementation for an AI player and check/checkmate detection.

# Features
- Full legal move validation (including castling, pawn promotion, and check rules);
- Check, checkmate, and stalemate detection;
- Draw detection by threefold repetition;
- A simple AI opponent using minimax search with alpha-beta pruning;
- Support for standard chess and [Chess960](https://en.wikipedia.org/wiki/Chess960) (Fischer Random).

# Requirements
- `g++`
- `make`

`make run` compiles and runs the game.

You will be asked if you want to play Chess960, then whether each side should be controlled by the computer.

# How to play

Moves are entered using the [algebraic notation](https://en.wikipedia.org/wiki/Chess_notation).

Examples:

| Input   | Meaning                          |
|---------|-----------------------------------|
| `b4`    | Move a pawn to b4                 |
| `Nf3`   | Move a knight to f3               |
| `Bxc6`	| Bishop captures on c6             |
| `Rae1`	| Rook on the a-file moves to e1    |
| `O-O`	  | Castle kingside                   |
| `e8=Q`	| Pawn promotes to queen on e8      |

If an invalid move is entered by a player, a message will appear that may give an indication on why the move isn't valid.
