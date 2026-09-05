#pragma once
#include <vector>
#include <string>
#include "Board.h"
#include "Move.h"

enum class GameState { ONGOING, CHECK, CHECKMATE, STALEMATE, DRAW_50MOVE };

// Game is the engine's orchestration layer: it owns a Board, tracks whose
// turn it is, records move history, offers undo via board snapshots, and
// handles the console I/O loop, exception handling, and save/load.
class Game {
private:
    Board board;
    Color currentTurn;
    std::vector<Move> moveHistory;
    std::vector<Board> boardHistory; // snapshot before each move, enables undo
    GameState state;
    int halfMoveClock;               // half-moves since last pawn move/capture (50-move rule)
    bool gameOver;

    void switchTurn();
    GameState evaluateState();
    void handlePromotion(Move& move);
    void printMoveHistory() const;
    void printHelp() const;
    void undoLastMove();

public:
    Game();

    void run();                                  // main interactive loop
    void makeMove(const std::string& input);      // e.g. "e2 e4"
    void saveGame(const std::string& filename) const;
    void loadGame(const std::string& filename);
    void printBoard() const;

    bool isGameOver() const { return gameOver; }
};
