#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Position.h"

class Board; // forward declaration to avoid circular includes

enum class Color { WHITE, BLACK, NONE };
enum class PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE };

inline Color oppositeColor(Color c) {
    if (c == Color::WHITE) return Color::BLACK;
    if (c == Color::BLACK) return Color::WHITE;
    return Color::NONE;
}

// Abstract base class for all chess pieces.
// Encapsulation: color/moved state are protected, only accessible via accessors.
// Abstraction: exposes a piece-agnostic interface (getPseudoLegalMoves, getAttackSquares...)
// Polymorphism: Board/Game operate purely through Piece*, the concrete piece type
//               (Pawn, Knight, Bishop, Rook, Queen, King) is resolved at runtime.
class Piece {
protected:
    Color color;
    bool moved;

public:
    explicit Piece(Color c) : color(c), moved(false) {}
    virtual ~Piece() = default;

    Color getColor() const { return color; }
    bool hasMoved() const { return moved; }
    void setMoved(bool m) { moved = m; }

    virtual PieceType getType() const = 0;
    virtual char getSymbol() const = 0;                // ASCII letter (upper=white, lower=black)
    virtual std::string getUnicodeSymbol() const = 0;   // UTF-8 chess glyph, for pretty printing
    virtual int getValue() const = 0;                   // material value (pawn=1 ... queen=9, king=0)

    // Squares this piece could move to, given the current board, ignoring
    // whether the move would leave the mover's own king in check.
    // (That "does it leave me in check" filter is applied one layer up, in Board.)
    virtual std::vector<Position> getPseudoLegalMoves(const Board& board, const Position& from) const = 0;

    // Squares this piece attacks/defends/controls, regardless of what is on them
    // (used for check detection and for validating castling paths).
    virtual std::vector<Position> getAttackSquares(const Board& board, const Position& from) const = 0;

    // Virtual copy-constructor idiom, needed because Board stores unique_ptr<Piece>
    // and must be able to deep-copy itself (for move simulation, undo, save/load clone, etc).
    virtual std::unique_ptr<Piece> clone() const = 0;

    static char pieceTypeToChar(PieceType type);
    static PieceType charToPieceType(char c);
    static std::unique_ptr<Piece> createPiece(PieceType type, Color color);
};
