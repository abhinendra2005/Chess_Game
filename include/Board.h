#pragma once
#include <array>
#include <memory>
#include <vector>
#include <string>
#include <iosfwd>
#include "Piece.h"
#include "Move.h"

// Board encapsulates all board state (the 8x8 grid of pieces, en-passant state)
// and every rule that depends purely on "what does the board look like right now":
// legality of moves (including check-safety, castling, en passant, promotion),
// check/checkmate/stalemate detection, and serialization for save/load.
class Board {
private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;
    Position enPassantTarget; // square a pawn can be captured en passant on, invalid if none

    void deepCopyFrom(const Board& other);

public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    Board(Board&&) noexcept = default;
    Board& operator=(Board&&) noexcept = default;
    ~Board() = default;

    void setupInitialPosition();
    void clearBoard();

    Piece* getPiece(const Position& pos) const;
    void setPiece(const Position& pos, std::unique_ptr<Piece> piece);
    std::unique_ptr<Piece> removePiece(const Position& pos);

    bool isEmpty(const Position& pos) const;
    bool isEnemyAt(const Position& pos, Color color) const;
    bool isAllyAt(const Position& pos, Color color) const;

    Position getEnPassantTarget() const { return enPassantTarget; }
    void setEnPassantTarget(const Position& pos) { enPassantTarget = pos; }
    void clearEnPassantTarget() { enPassantTarget = Position(); }

    Position findKing(Color color) const;
    bool isSquareAttacked(const Position& pos, Color byColor) const;
    bool isInCheck(Color color) const;

    // Fully legal moves for the piece on 'from' (pseudo-legal moves filtered
    // by "does this leave my own king in check", plus castling when applicable).
    std::vector<Move> getLegalMoves(const Position& from) const;

    // All legal moves for every piece of the given color - empty means
    // checkmate (if in check) or stalemate (if not).
    std::vector<Move> getAllLegalMoves(Color color) const;

    // Applies an already-validated Move to the board (handles capture removal,
    // castling rook hop, en passant capture, promotion, and en-passant-target bookkeeping).
    void executeMove(const Move& move);

    Board clone() const;

    void print(bool useUnicode = false) const;

    // Simple, human-readable serialization used by save/load.
    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
};
