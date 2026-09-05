#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    explicit Pawn(Color c) : Piece(c) {}

    PieceType getType() const override { return PieceType::PAWN; }
    char getSymbol() const override { return color == Color::WHITE ? 'P' : 'p'; }
    std::string getUnicodeSymbol() const override { return color == Color::WHITE ? "\u2659" : "\u265F"; }
    int getValue() const override { return 1; }
    std::unique_ptr<Piece> clone() const override { return std::make_unique<Pawn>(*this); }

    std::vector<Position> getPseudoLegalMoves(const Board& board, const Position& from) const override;
    std::vector<Position> getAttackSquares(const Board& board, const Position& from) const override;
};
