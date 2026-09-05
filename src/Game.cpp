#include "Game.h"
#include "ChessExceptions.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cctype>

Game::Game()
    : currentTurn(Color::WHITE), state(GameState::ONGOING), halfMoveClock(0), gameOver(false) {
    board.setupInitialPosition();
}

void Game::switchTurn() {
    currentTurn = oppositeColor(currentTurn);
}

GameState Game::evaluateState() {
    bool inCheck = board.isInCheck(currentTurn);
    std::vector<Move> moves = board.getAllLegalMoves(currentTurn);

    if (moves.empty()) {
        state = inCheck ? GameState::CHECKMATE : GameState::STALEMATE;
    } else if (halfMoveClock >= 100) { // 50 full moves with no pawn move / capture
        state = GameState::DRAW_50MOVE;
    } else if (inCheck) {
        state = GameState::CHECK;
    } else {
        state = GameState::ONGOING;
    }
    return state;
}

void Game::handlePromotion(Move& move) {
    std::cout << "Pawn promotion! Choose piece - (Q)ueen, (R)ook, (B)ishop, (N)ight: ";
    std::string choice;
    std::getline(std::cin, choice);
    char c = choice.empty() ? 'Q' : static_cast<char>(std::toupper(static_cast<unsigned char>(choice[0])));
    PieceType t = Piece::charToPieceType(c);
    if (t != PieceType::QUEEN && t != PieceType::ROOK && t != PieceType::BISHOP && t != PieceType::KNIGHT) {
        std::cout << "Unrecognized choice, defaulting to Queen.\n";
        t = PieceType::QUEEN;
    }
    move.promotionType = t;
}

void Game::printMoveHistory() const {
    if (moveHistory.empty()) {
        std::cout << "No moves have been played yet.\n";
        return;
    }
    std::cout << "Move history:\n";
    for (size_t i = 0; i < moveHistory.size(); ++i) {
        if (i % 2 == 0) std::cout << (i / 2 + 1) << ". ";
        std::cout << moveHistory[i].toAlgebraicNotation() << "  ";
        if (i % 2 == 1) std::cout << "\n";
    }
    std::cout << "\n";
}

void Game::printHelp() const {
    std::cout <<
        "Commands:\n"
        "  e2 e4          - move the piece on e2 to e4\n"
        "  save <file>    - save the current game to a file\n"
        "  load <file>    - load a game from a file\n"
        "  history        - show move history\n"
        "  undo           - undo the last move\n"
        "  resign         - resign the game\n"
        "  help           - show this message\n"
        "  quit           - exit the program\n";
}

void Game::undoLastMove() {
    if (boardHistory.empty()) {
        throw ChessException("Nothing to undo.");
    }
    board = std::move(boardHistory.back());
    boardHistory.pop_back();
    moveHistory.pop_back();
    switchTurn();
    gameOver = false;
    std::cout << "Move undone.\n";
}

void Game::printBoard() const {
    board.print(false);
}

void Game::makeMove(const std::string& input) {
    std::istringstream iss(input);
    std::string fromStr, toStr;
    iss >> fromStr >> toStr;
    if (fromStr.empty() || toStr.empty()) {
        throw InvalidMoveException("A move needs two squares, e.g. 'e2 e4'");
    }

    Position from = Position::fromAlgebraic(fromStr);
    Position to = Position::fromAlgebraic(toStr);
    if (!from.isValid() || !to.isValid()) {
        throw InvalidPositionException("'" + fromStr + "' or '" + toStr + "' is not a valid square (use a1-h8)");
    }

    Piece* p = board.getPiece(from);
    if (!p) throw NoPieceException("There is no piece on " + fromStr);
    if (p->getColor() != currentTurn) {
        throw InvalidMoveException("That piece does not belong to " +
            std::string(currentTurn == Color::WHITE ? "White" : "Black"));
    }

    std::vector<Move> legalMoves = board.getLegalMoves(from);
    Move* chosen = nullptr;
    for (auto& m : legalMoves) {
        if (m.to == to) { chosen = &m; break; }
    }
    if (!chosen) {
        throw InvalidMoveException("Illegal move: " + fromStr + " to " + toStr);
    }

    if (chosen->type == MoveType::PROMOTION) {
        handlePromotion(*chosen);
    }

    boardHistory.push_back(board.clone());
    board.executeMove(*chosen);
    moveHistory.push_back(*chosen);

    if (chosen->movedType == PieceType::PAWN || chosen->isCapture()) {
        halfMoveClock = 0;
    } else {
        ++halfMoveClock;
    }

    switchTurn();
}

void Game::saveGame(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) throw FileIOException("Could not open '" + filename + "' for writing");

    out << "CHESSSAVE v1\n";
    out << "TURN " << (currentTurn == Color::WHITE ? "WHITE" : "BLACK") << "\n";
    out << "HALFMOVE " << halfMoveClock << "\n";
    board.serialize(out);
    out << "HISTORY " << moveHistory.size() << "\n";
    for (const auto& m : moveHistory) {
        out << m.toAlgebraicNotation() << "\n";
    }
    out << "END\n";

    if (!out) throw FileIOException("An error occurred while writing to '" + filename + "'");
}

void Game::loadGame(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw FileIOException("Could not open '" + filename + "' for reading");

    std::string line;
    if (!std::getline(in, line) || line.rfind("CHESSSAVE", 0) != 0) {
        throw FileIOException("'" + filename + "' is not a valid chess save file");
    }

    if (!std::getline(in, line)) throw FileIOException("Malformed save file: missing TURN line");
    {
        std::istringstream ss(line);
        std::string tag, turnStr;
        ss >> tag >> turnStr;
        currentTurn = (turnStr == "WHITE") ? Color::WHITE : Color::BLACK;
    }

    if (!std::getline(in, line)) throw FileIOException("Malformed save file: missing HALFMOVE line");
    {
        std::istringstream ss(line);
        std::string tag;
        int val = 0;
        ss >> tag >> val;
        halfMoveClock = val;
    }

    board.deserialize(in);

    if (!std::getline(in, line) || line.rfind("HISTORY", 0) != 0) {
        throw FileIOException("Malformed save file: missing HISTORY line");
    }
    int count = 0;
    {
        std::istringstream ss(line);
        std::string tag;
        ss >> tag >> count;
    }

    moveHistory.clear();
    for (int i = 0; i < count; ++i) {
        if (!std::getline(in, line)) throw FileIOException("Malformed save file: incomplete history");
        Move m;
        m.notationOverride = line; // notation-only record; exact undo across a load is not supported
        moveHistory.push_back(m);
    }

    boardHistory.clear(); // undo history does not survive a save/load round trip
    gameOver = false;
}

void Game::run() {
    std::cout << "===================================\n";
    std::cout << "        CONSOLE CHESS ENGINE\n";
    std::cout << "===================================\n";
    printHelp();
    std::cout << "\n";

    while (!gameOver) {
        printBoard();

        GameState st = evaluateState();
        if (st == GameState::CHECKMATE) {
            std::cout << (currentTurn == Color::WHITE ? "White" : "Black") << " is checkmated. "
                      << (currentTurn == Color::WHITE ? "Black" : "White") << " wins!\n";
            gameOver = true;
            break;
        } else if (st == GameState::STALEMATE) {
            std::cout << "Stalemate - the game is a draw.\n";
            gameOver = true;
            break;
        } else if (st == GameState::DRAW_50MOVE) {
            std::cout << "Draw by the fifty-move rule.\n";
            gameOver = true;
            break;
        } else if (st == GameState::CHECK) {
            std::cout << (currentTurn == Color::WHITE ? "White" : "Black") << " is in check!\n";
        }

        std::cout << (currentTurn == Color::WHITE ? "White" : "Black") << " to move> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string first;
        iss >> first;

        try {
            if (first == "quit" || first == "exit") {
                std::cout << "Goodbye!\n";
                break;
            } else if (first == "save") {
                std::string f;
                iss >> f;
                if (f.empty()) f = "chess_save.txt";
                saveGame(f);
                std::cout << "Game saved to " << f << "\n";
            } else if (first == "load") {
                std::string f;
                iss >> f;
                if (f.empty()) f = "chess_save.txt";
                loadGame(f);
                std::cout << "Game loaded from " << f << "\n";
            } else if (first == "history") {
                printMoveHistory();
            } else if (first == "undo") {
                undoLastMove();
            } else if (first == "resign") {
                std::cout << (currentTurn == Color::WHITE ? "White" : "Black") << " resigns. "
                          << (currentTurn == Color::WHITE ? "Black" : "White") << " wins!\n";
                gameOver = true;
            } else if (first == "help") {
                printHelp();
            } else {
                std::string second;
                iss >> second;
                makeMove(first + " " + second);
            }
        } catch (const ChessException& e) {
            std::cout << "Error: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << "\n";
        }

        std::cout << "\n";
    }
}
