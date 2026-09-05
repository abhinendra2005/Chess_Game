#include "Pawn.h"
#include "Board.h"

std::vector<Position> Pawn::getPseudoLegalMoves(const Board& board, const Position& from) const {
    std::vector<Position> moves;
    int dir = (color == Color::WHITE) ? 1 : -1;

    // Single push
    Position oneStep(from.row + dir, from.col);
    if (oneStep.isValid() && board.isEmpty(oneStep)) {
        moves.push_back(oneStep);

        // Double push from the starting rank, only if both squares are empty
        Position twoStep(from.row + 2 * dir, from.col);
        if (!moved && twoStep.isValid() && board.isEmpty(twoStep)) {
            moves.push_back(twoStep);
        }
    }

    // Diagonal captures (including en passant)
    for (int dc : {-1, 1}) {
        Position target(from.row + dir, from.col + dc);
        if (!target.isValid()) continue;
        if (board.isEnemyAt(target, color)) {
            moves.push_back(target);
        } else if (target == board.getEnPassantTarget()) {
            moves.push_back(target);
        }
    }
    return moves;
}

std::vector<Position> Pawn::getAttackSquares(const Board& /*board*/, const Position& from) const {
    // A pawn always "controls" its two diagonal squares, whether or not
    // anything is standing there right now - needed for correct check detection.
    std::vector<Position> attacks;
    int dir = (color == Color::WHITE) ? 1 : -1;
    for (int dc : {-1, 1}) {
        Position target(from.row + dir, from.col + dc);
        if (target.isValid()) attacks.push_back(target);
    }
    return attacks;
}
