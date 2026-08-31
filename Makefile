all:
	g++ -o ChessGame src/util.cpp src/board.cpp src/ai.cpp src/move.cpp src/piece.cpp src/main.cpp -Wall

run:
	g++ -o ChessGame src/util.cpp src/board.cpp src/ai.cpp src/move.cpp src/piece.cpp src/main.cpp -Wall
	./ChessGame
clean:
	rm -rf ChessGame