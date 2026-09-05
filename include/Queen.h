#pragma once
#include "Piece.h"

class Queen : public Piece {
public:
    explicit Queen(Color c) : Piece(c) {}

    PieceType getType() const override { return PieceType::QUEEN; }
    char getSymbol() const override { return color == Color::WHITE ? 'Q' : 'q'; }
    std::string getUnicodeSymbol() const override { return color == Color::WHITE ? "\u2655" : "\u265B"; }
    int getValue() const override { return 9; }
    std::unique_ptr<Piece> clone() const override { return std::make_unique<Queen>(*this); }

    std::vector<Position> getPseudoLegalMoves(const Board& board, const Position& from) const override;
    std::vector<Position> getAttackSquares(const Board& board, const Position& from) const override;
};
