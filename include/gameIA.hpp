#pragma once
#include "gameMath.hpp"

struct GameBoard {
    char board[3][3]; // 3x3 game board for XO game
    GamePoint boardPositions[3][3]; // Store positions for each cell

    bool IsWinMove(char player);

    GameBoard();
    ~GameBoard();
};

struct GameIA {
    GameIA();
    // search tree heuristic
    int heuristic(GameBoard& board, char player);
    GamePoint findBestMove(const GameBoard& board, char player);
    int minimax(GameBoard& board, int depth, bool isMaximizing, char player);
};