#include "Bishop.h"
#include "MoveGenUtils.h"

namespace {
const std::vector<std::pair<int, int>> BISHOP_DIRS = {
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};
}

std::vector<Position> Bishop::getPseudoLegalMoves(const Board& board, const Position& from) const {
    return slidingPseudoLegalMoves(board, from, color, BISHOP_DIRS);
}

std::vector<Position> Bishop::getAttackSquares(const Board& board, const Position& from) const {
    return slidingAttackSquares(board, from, BISHOP_DIRS);
}
