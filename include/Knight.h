#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
    explicit Knight(Color c) : Piece(c) {}

    PieceType getType() const override { return PieceType::KNIGHT; }
    char getSymbol() const override { return color == Color::WHITE ? 'N' : 'n'; }
    std::string getUnicodeSymbol() const override { return color == Color::WHITE ? "\u2658" : "\u265E"; }
    int getValue() const override { return 3; }
    std::unique_ptr<Piece> clone() const override { return std::make_unique<Knight>(*this); }

    std::vector<Position> getPseudoLegalMoves(const Board& board, const Position& from) const override;
    std::vector<Position> getAttackSquares(const Board& board, const Position& from) const override;
};
