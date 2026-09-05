#include "Knight.h"
#include "MoveGenUtils.h"

namespace {
const std::vector<std::pair<int, int>> KNIGHT_OFFSETS = {
    {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
    {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
};
}

std::vector<Position> Knight::getPseudoLegalMoves(const Board& board, const Position& from) const {
    return offsetPseudoLegalMoves(board, from, color, KNIGHT_OFFSETS);
}

std::vector<Position> Knight::getAttackSquares(const Board& /*board*/, const Position& from) const {
    return offsetAttackSquares(from, KNIGHT_OFFSETS);
}
