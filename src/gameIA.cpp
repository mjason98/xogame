#include "gameIA.hpp"
#include <algorithm>
#include <iostream>

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


int GameIA::heuristic(GameBoard& _board, char player) {
    int winScore = 20; // Score for winning
    int loseScore = -20; // Score for losing

    // Check for winning moves
    if (_board.IsWinMove(player)) {
        return winScore;
    }
    if (_board.IsWinMove(player == 1 ? 2 : 1)) {
        return loseScore;
    }

    // check for some cell empty
    bool hasEmptyCell = false;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (_board.board[i][j] == 0) {
                hasEmptyCell = true;
                break;
            }
        }
        if (hasEmptyCell) break;
    }
    if (!hasEmptyCell) {
        return 0; // Draw
    }

    return 0; // Neutral score if no win or lose condition met
}

GamePoint GameIA::findBestMove(const GameBoard& board, char player) {
    int bestScore = -1000;
    GamePoint bestMove = { -1, -1 };

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board.board[i][j] == 0) { // If the cell is empty
                std::cout << ">> Evaluating move at (" << i << ", " << j << ") for player " << (int)player << "\n";

                GameBoard newBoard = board;
                newBoard.board[i][j] = player; // Make the move
                int score = minimax(newBoard, 0, false, player);

                std::cout << ">> Move at (" << i << ", " << j << ") has score " << score << "\n";
                
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = { j, i }; // Store the position

                    std::cout << ">>* New best move found at (" << i << ", " << j << ") with score " << bestScore << "\n";
                }
            }
        }
    }
    std::cout << "Best move for player " << (int)player << " is at (" << bestMove.y << ", " << bestMove.x << ") with score " << bestScore << "\n";
    
    return bestMove;
}

int GameIA::minimax(GameBoard& board, int depth, bool isMaximizing, char player) {
    char opponent = (player == 1) ? 2 : 1; // Switch player
    int score = heuristic(board, player);

    // Check for terminal states
    if (score >= 20) return score - depth; // Maximizing player wins
    if (score <= -20) return score + depth; // Minimizing player wins
    if (depth >= 9) return 0; // Draw or max depth reached

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board.board[i][j] == 0) { // If the cell is empty
                    board.board[i][j] = player; // Make the move
                    bestScore = std::max(bestScore, minimax(board, depth + 1, false, player));
                    board.board[i][j] = 0; // Undo the move
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board.board[i][j] == 0) { // If the cell is empty
                    board.board[i][j] = opponent; // Make the move
                    bestScore = std::min(bestScore, minimax(board, depth + 1, true, player));
                    board.board[i][j] = 0; // Undo the move
                }
            }
        }
        return bestScore;
    }
}