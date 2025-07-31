#include "gameIA.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>

GameBoard::GameBoard() {
    // Initialize the game board
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = 0; // 0 means empty, 1 for cross, 2 for circle
            boardPositions[i][j] = { j * 80, i * 80 }; // Set the position for each cell
        }
    }
}

GameBoard::~GameBoard() {
    // Cleanup if necessary
}

bool GameBoard::IsWinMove(char player) {
    // Check rows, columns, and diagonals for a win
    for (int i = 0; i < 3; ++i) {
        // Check rows ----------
        if (board[i][0] == player && board[i][1] == player && board[i][2] == 0) {
            return true; // Potential win
        }
        if (board[i][1] == player && board[i][2] == player && board[i][0] == 0) {
            return true; // Potential win
        }
        if (board[i][0] == player && board[i][2] == player && board[i][1] == 0) {
            return true; // Potential win
        }
        // Check columns ----------
        if (board[0][i] == player && board[1][i] == player && board[2][i] == 0) {
            return true; // Potential win
        }
        if (board[1][i] == player && board[2][i] == player && board[0][i] == 0) {
            return true; // Potential win
        }
        if (board[0][i] == player && board[2][i] == player && board[1][i] == 0) {
            return true; // Potential win
        }
        // ---------------------------
    }
    // Check diagonals ----------------
    if (board[0][0] == player && board[1][1] == player && board[2][2] == 0) {
        return true; // Potential win
    }
    if (board[0][0] == player && board[2][2] == player && board[1][1] == 0) {
        return true; // Potential win
    }
    if (board[1][1] == player && board[2][2] == player && board[0][0] == 0) {
        return true; // Potential win
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == 0) {
        return true; // Potential win
    }
    if (board[0][2] == player && board[2][0] == player && board[1][1] == 0) {
        return true; // Potential win
    }
    if (board[1][1] == player && board[2][0] == player && board[0][2] == 0) {
        return true; // Potential win
    }
    // ---------------------------
    return false;
}

GameIA::GameIA() {
    // Initialize AI if needed
}

//= = Value functions ========================

int GameIA::IdFromState(const GameBoard& board) const {
    int id = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            id *= 3;
            id += board.board[i][j]; // Convert board state to a unique ID
        }
    }
    return id;
}

void GameIA::load_array(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("Failed to open file");

    std::streamsize size = file.tellg();
    if (size % sizeof(float) != 0) {
        throw std::runtime_error("Invalid file size, not divisible by sizeof(float)");
    }

    length = size / sizeof(float);
    file.seekg(0, std::ios::beg);

    weightArray = new float[length];  // allocate
    if (!file.read(reinterpret_cast<char*>(weightArray), size)) {
        delete[] weightArray;
        throw std::runtime_error("Failed to read data");
    }
    
    std::cout << "Weights loaded successfully, length: " << length << "\n";
    std::cout << "First weight: " << weightArray[0] << "\n";
}

void GameIA::releaseWeights() {
    if (weightArray) {
        delete[] weightArray;
        weightArray = nullptr;
    }
}

float GameIA::ValueFromState(const GameBoard& board) const {
    int id = IdFromState(board);
    if (weightArray) {
        if (id < 0 || id >= 19683) { // 3^9 possible states for a 3x3 board
            throw std::out_of_range("State ID out of range");
        }
        return weightArray[id];
    }
    return 0.0f;
}

GamePoint GameIA::findBestMoveV(const GameBoard& board) {
    float bestScore = -1.0f;
    GamePoint bestMove = { -1, -1 };

    GameBoard boardI = board; // Copy the current board state
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (boardI.board[i][j] == 1)
                boardI.board[i][j] = 2;
            else if (boardI.board[i][j] == 2)
                boardI.board[i][j] = 1;
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board.board[i][j] == 0) { // If the cell is empty
                std::cout << ">> Evaluating move at (" << i << ", " << j << ")" << "\n";

                GameBoard newBoard = boardI;
                newBoard.board[i][j] = 1; // Make the move

                float score = ValueFromState(newBoard);

                std::cout << ">> Move at (" << i << ", " << j << ") has score " << score << "\n";
                
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = { j, i }; // Store the position
                    std::cout << ">>* New best move found at (" << i << ", " << j << ") with score " << bestScore << "\n";
                }
            }
        }
    }

    std::cout << "Best move is at (" << bestMove.y << ", " << bestMove.x << ") with score " << bestScore << "\n";
    
    return bestMove;
}