#pragma once
#include <vector>
#include <utility>
#include "Position.h"
#include "Piece.h"
#include "Board.h"

// Shared helpers used by Bishop/Rook/Queen (sliding pieces) and by
// Knight/King (fixed-offset pieces) so the move-generation logic for
// "slide until blocked" and "jump to fixed offsets" is written once.

inline std::vector<Position> slidingPseudoLegalMoves(const Board& board, const Position& from, Color color,
                                                       const std::vector<std::pair<int, int>>& directions) {
    std::vector<Position> moves;
    for (const auto& d : directions) {
        Position cur(from.row + d.first, from.col + d.second);
        while (cur.isValid()) {
            if (board.isEmpty(cur)) {
                moves.push_back(cur);
            } else {
                if (board.isEnemyAt(cur, color)) moves.push_back(cur);
                break; // blocked, whether by ally or enemy
            }
            cur = Position(cur.row + d.first, cur.col + d.second);
        }
    }
    return moves;
}

inline std::vector<Position> slidingAttackSquares(const Board& board, const Position& from,
                                                     const std::vector<std::pair<int, int>>& directions) {
    std::vector<Position> attacks;
    for (const auto& d : directions) {
        Position cur(from.row + d.first, from.col + d.second);
        while (cur.isValid()) {
            attacks.push_back(cur);
            if (!board.isEmpty(cur)) break; // controls up to and including first blocker
            cur = Position(cur.row + d.first, cur.col + d.second);
        }
    }
    return attacks;
}

inline std::vector<Position> offsetPseudoLegalMoves(const Board& board, const Position& from, Color color,
                                                       const std::vector<std::pair<int, int>>& offsets) {
    std::vector<Position> moves;
    for (const auto& o : offsets) {
        Position target(from.row + o.first, from.col + o.second);
        if (target.isValid() && !board.isAllyAt(target, color)) {
            moves.push_back(target);
        }
    }
    return moves;
}

inline std::vector<Position> offsetAttackSquares(const Position& from,
                                                    const std::vector<std::pair<int, int>>& offsets) {
    std::vector<Position> attacks;
    for (const auto& o : offsets) {
        Position target(from.row + o.first, from.col + o.second);
        if (target.isValid()) attacks.push_back(target);
    }
    return attacks;
}
