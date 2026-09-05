#pragma once
#include "Piece.h"

class Bishop : public Piece {
public:
    explicit Bishop(Color c) : Piece(c) {}

    PieceType getType() const override { return PieceType::BISHOP; }
    char getSymbol() const override { return color == Color::WHITE ? 'B' : 'b'; }
    std::string getUnicodeSymbol() const override { return color == Color::WHITE ? "\u2657" : "\u265D"; }
    int getValue() const override { return 3; }
    std::unique_ptr<Piece> clone() const override { return std::make_unique<Bishop>(*this); }

    std::vector<Position> getPseudoLegalMoves(const Board& board, const Position& from) const override;
    std::vector<Position> getAttackSquares(const Board& board, const Position& from) const override;
};
