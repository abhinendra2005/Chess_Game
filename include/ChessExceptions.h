#pragma once
#include <stdexcept>
#include <string>

// Base class for every exception thrown by the chess engine.
// Deriving from std::runtime_error gives us what() for free while
// still letting callers catch every engine-specific error with one type.
class ChessException : public std::runtime_error {
public:
    explicit ChessException(const std::string& msg) : std::runtime_error(msg) {}
};

// Thrown when a Position/algebraic square is malformed or off-board.
class InvalidPositionException : public ChessException {
public:
    explicit InvalidPositionException(const std::string& msg = "Invalid board position")
        : ChessException(msg) {}
};

// Thrown when a requested move is not legal (illegal, blocked, leaves king in check, etc).
class InvalidMoveException : public ChessException {
public:
    explicit InvalidMoveException(const std::string& msg = "Invalid move")
        : ChessException(msg) {}
};

// Thrown when an operation expects a piece on a square but finds none,
// or the piece present does not belong to the player requesting the action.
class NoPieceException : public ChessException {
public:
    explicit NoPieceException(const std::string& msg = "No piece at that square")
        : ChessException(msg) {}
};

// Thrown for any save/load I/O or file-format failure.
class FileIOException : public ChessException {
public:
    explicit FileIOException(const std::string& msg = "File operation failed")
        : ChessException(msg) {}
};
