#include "Piece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include <cctype>

char Piece::pieceTypeToChar(PieceType type) {
    switch (type) {
        case PieceType::PAWN:   return 'P';
        case PieceType::KNIGHT: return 'N';
        case PieceType::BISHOP: return 'B';
        case PieceType::ROOK:   return 'R';
        case PieceType::QUEEN:  return 'Q';
        case PieceType::KING:   return 'K';
        default:                return '?';
    }
}

PieceType Piece::charToPieceType(char c) {
    switch (std::toupper(static_cast<unsigned char>(c))) {
        case 'P': return PieceType::PAWN;
        case 'N': return PieceType::KNIGHT;
        case 'B': return PieceType::BISHOP;
        case 'R': return PieceType::ROOK;
        case 'Q': return PieceType::QUEEN;
        case 'K': return PieceType::KING;
        default:  return PieceType::NONE;
    }
}

// Factory method (creational pattern) - the rest of the engine never needs
// to know the concrete derived class name, only the PieceType enum value.
std::unique_ptr<Piece> Piece::createPiece(PieceType type, Color color) {
    switch (type) {
        case PieceType::PAWN:   return std::make_unique<Pawn>(color);
        case PieceType::KNIGHT: return std::make_unique<Knight>(color);
        case PieceType::BISHOP: return std::make_unique<Bishop>(color);
        case PieceType::ROOK:   return std::make_unique<Rook>(color);
        case PieceType::QUEEN:  return std::make_unique<Queen>(color);
        case PieceType::KING:   return std::make_unique<King>(color);
        default:                return nullptr;
    }
}
