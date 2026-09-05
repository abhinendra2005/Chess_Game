#include "King.h"
#include "MoveGenUtils.h"

namespace {
const std::vector<std::pair<int, int>> KING_OFFSETS = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};
}

std::vector<Position> King::getPseudoLegalMoves(const Board& board, const Position& from) const {
    return offsetPseudoLegalMoves(board, from, color, KING_OFFSETS);
}

std::vector<Position> King::getAttackSquares(const Board& /*board*/, const Position& from) const {
    return offsetAttackSquares(from, KING_OFFSETS);
}
