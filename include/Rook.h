#pragma once
#include "Piece.h"

class Rook : public Piece {
public:
    explicit Rook(Color c) : Piece(c) {}

    PieceType getType() const override { return PieceType::ROOK; }
    char getSymbol() const override { return color == Color::WHITE ? 'R' : 'r'; }
    std::string getUnicodeSymbol() const override { return color == Color::WHITE ? "\u2656" : "\u265C"; }
    int getValue() const override { return 5; }
    std::unique_ptr<Piece> clone() const override { return std::make_unique<Rook>(*this); }

    std::vector<Position> getPseudoLegalMoves(const Board& board, const Position& from) const override;
    std::vector<Position> getAttackSquares(const Board& board, const Position& from) const override;
};
