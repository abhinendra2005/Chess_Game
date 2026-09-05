#pragma once
#include <string>

// Represents a single square on the 8x8 board.
// row: 0-7 maps to rank 1-8
// col: 0-7 maps to file a-h
struct Position {
    int row;
    int col;

    Position() : row(-1), col(-1) {}
    Position(int r, int c) : row(r), col(c) {}

    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    // e.g. (1,4) -> "e2"
    std::string toAlgebraic() const {
        if (!isValid()) return "--";
        std::string s;
        s += static_cast<char>('a' + col);
        s += static_cast<char>('1' + row);
        return s;
    }

    // e.g. "e2" -> (1,4). Returns an invalid Position on bad input.
    static Position fromAlgebraic(const std::string& s) {
        if (s.size() != 2) return Position(-1, -1);
        char fileC = s[0];
        char rankC = s[1];
        if (fileC < 'a' || fileC > 'h') return Position(-1, -1);
        if (rankC < '1' || rankC > '8') return Position(-1, -1);
        int c = fileC - 'a';
        int r = rankC - '1';
        return Position(r, c);
    }
};
