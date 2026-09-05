#pragma once
#include "Piece.h"

// Note: castling is NOT generated here. It depends on rook state and
// square-attack checks that only Board has full context for, so Board
// appends legal castling moves on top of King's normal one-step moves.
class King : public Piece {
public:
    explicit King(Color c) : Piece(c) {}

    PieceType getType() const override { return PieceType::KING; }
    char getSymbol() const override { return color == Color::WHITE ? 'K' : 'k'; }
    std::string getUnicodeSymbol() const override { return color == Color::WHITE ? "\u2654" : "\u265A"; }
    int getValue() const override { return 0; }
    std::unique_ptr<Piece> clone() const override { return std::make_unique<King>(*this); }

    std::vector<Position> getPseudoLegalMoves(const Board& board, const Position& from) const override;
    std::vector<Position> getAttackSquares(const Board& board, const Position& from) const override;
};
