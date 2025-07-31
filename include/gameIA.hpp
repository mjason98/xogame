#pragma once
#include "gameMath.hpp"
#include <cstddef> 
#include <string>

struct GameBoard {
    char board[3][3]; // 3x3 game board for XO game
    GamePoint boardPositions[3][3]; // Store positions for each cell

    bool IsWinMove(char player);

    GameBoard();
    ~GameBoard();
};

struct GameIA {
    GameIA();
    
    // search tree heuristic ==================
    int heuristic(GameBoard& board, char player);
    GamePoint findBestMove(const GameBoard& board, char player);
    int minimax(GameBoard& board, int depth, bool isMaximizing, char player);
    // ========================================

    // value functions ========================
    float * weightArray = nullptr; // Pointer to the weights array
    size_t length; // Reference to the length of the weights array
    void releaseWeights();
    void load_array(const std::string& filename);
    int IdFromState(const GameBoard& board) const;
    int ValueFromState(const GameBoard& board, char player) const;
    // ========================================
};