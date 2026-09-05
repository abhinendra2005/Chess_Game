#include "Queen.h"
#include "MoveGenUtils.h"

namespace {
const std::vector<std::pair<int, int>> QUEEN_DIRS = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
};
}

std::vector<Position> Queen::getPseudoLegalMoves(const Board& board, const Position& from) const {
    return slidingPseudoLegalMoves(board, from, color, QUEEN_DIRS);
}

std::vector<Position> Queen::getAttackSquares(const Board& board, const Position& from) const {
    return slidingAttackSquares(board, from, QUEEN_DIRS);
}
