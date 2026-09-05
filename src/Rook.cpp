#include "Rook.h"
#include "MoveGenUtils.h"

namespace {
const std::vector<std::pair<int, int>> ROOK_DIRS = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}
};
}

std::vector<Position> Rook::getPseudoLegalMoves(const Board& board, const Position& from) const {
    return slidingPseudoLegalMoves(board, from, color, ROOK_DIRS);
}

std::vector<Position> Rook::getAttackSquares(const Board& board, const Position& from) const {
    return slidingAttackSquares(board, from, ROOK_DIRS);
}
