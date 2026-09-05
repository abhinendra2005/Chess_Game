# Console Chess Engine (C++17)

A complete, console-based two-player chess game written in modern C++17,
built around a modular, object-oriented class hierarchy.

## Features

- **Full legal move validation** — every rule-legal move for every piece,
  including moves that must be rejected because they'd leave your own king
  in check (pins are handled automatically as a side-effect of this).
- **Check / checkmate / stalemate detection**
- **Castling** — both kingside and queenside, with all real rules enforced
  (king/rook unmoved, empty in-between squares, king not currently in check,
  king does not pass through or land on an attacked square).
- **En passant capture**
- **Pawn promotion** — interactive choice of Queen / Rook / Bishop / Knight
- **Move history** — recorded and viewable in simple algebraic notation
- **Undo** — step back through the game via board snapshots
- **Fifty-move rule** draw detection
- **File-based save/load** — human-readable save format
- **Exception-based error handling** — a small custom exception hierarchy
  (`ChessException` and friends) instead of error codes or crashes
- Built entirely on the STL (`vector`, `array`, `unique_ptr`, streams) —
  no manual `new`/`delete` anywhere in the codebase

## Design / OOP principles

| Principle       | Where it shows up |
|------------------|--------------------|
| **Abstraction**   | `Piece` exposes a piece-agnostic interface (`getPseudoLegalMoves`, `getAttackSquares`, `getSymbol`, ...). Callers (`Board`, `Game`) never care what concrete piece they're holding. |
| **Encapsulation** | `Piece::color` / `moved` are `protected` and only reachable through accessors. `Board`'s grid is `private`; all access goes through validated methods that throw on bad input. |
| **Inheritance**   | `Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King` all derive from the abstract `Piece` base class. |
| **Runtime polymorphism** | `Board` stores `std::unique_ptr<Piece>` and calls virtual methods (`getPseudoLegalMoves`, `getAttackSquares`, `clone`, ...) — the correct derived-class behavior is resolved at runtime for every square on the board. |

### Class hierarchy

```
Piece (abstract)
 ├── Pawn
 ├── Knight
 ├── Bishop
 ├── Rook
 ├── Queen
 └── King

Board        — owns the 8x8 grid, all legality/check logic, serialization
Move         — a single ply: from/to squares + metadata (capture, castle, promotion...)
Game         — turn management, move history, undo, save/load, the console I/O loop
ChessException hierarchy — InvalidMoveException, InvalidPositionException,
                            NoPieceException, FileIOException
```

### Key architectural decisions

- **`getPseudoLegalMoves` vs. `getAttackSquares`.** Each piece implements two
  move-related methods: one for *moves it could actually make* (respecting
  occupancy/capture rules) and one for *squares it controls*, used purely for
  check/attacked-square detection (this distinction matters for pawns, whose
  diagonal "attack" doesn't require an enemy piece to be sitting there for the
  square to count as attacked — e.g. for castling-through-check checks).
- **Check-safety is a filter, not special-cased per piece.** `Board::getLegalMoves`
  takes every pseudo-legal move, simulates it on a cloned board, and discards
  it if the mover's own king ends up in check. This single mechanism handles
  pins, discovered checks, and "can't move into check" uniformly for every
  piece type, including the king itself.
- **`Board` is deep-copyable** (`clone()` / copy constructor) via each piece's
  virtual `clone()` method (the "virtual copy constructor" idiom), which is
  what makes move-simulation, undo snapshots, and save/load all simple to
  implement correctly with `unique_ptr`-owned pieces.

## Building

Requires a C++17 compiler (g++ 9+ or clang++ 8+ recommended).

```bash
make          # builds ./chess
make clean    # removes build artifacts
```

Or compile directly:

```bash
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -o chess src/*.cpp
```

## Running

```bash
./chess
```

### In-game commands

| Command             | Effect |
|----------------------|--------|
| `e2 e4`              | Move the piece on e2 to e4 |
| `save <filename>`    | Save the current game |
| `load <filename>`    | Load a game from disk |
| `history`            | Show move history |
| `undo`               | Undo the last move |
| `resign`             | Resign the game |
| `help`               | Show the command list |
| `quit` / `exit`      | Exit the program |

On a promotion move, you'll be prompted to choose Queen / Rook / Bishop / Knight.

## Project layout

```
chess_game/
├── include/            # headers
│   ├── Position.h
│   ├── ChessExceptions.h
│   ├── Piece.h
│   ├── MoveGenUtils.h  # shared sliding/offset move-generation helpers
│   ├── Pawn.h / Knight.h / Bishop.h / Rook.h / Queen.h / King.h
│   ├── Move.h
│   ├── Board.h
│   └── Game.h
├── src/                # implementations
│   ├── Piece.cpp
│   ├── Pawn.cpp / Knight.cpp / Bishop.cpp / Rook.cpp / Queen.cpp / King.cpp
│   ├── Board.cpp
│   ├── Game.cpp
│   └── main.cpp
├── Makefile
└── README.md
```

## Extending it

The architecture is deliberately open for extension:

- **New piece type**: derive from `Piece`, implement the five pure-virtual
  methods, add one line to `Piece::createPiece`'s switch. No other file needs
  to change.
- **New draw rule** (e.g. threefold repetition): add a check inside
  `Game::evaluateState()`.
- **AI opponent**: `Board::getAllLegalMoves(Color)` already returns every
  legal move for a side — a search algorithm (minimax, etc.) can be layered
  on top of `Board::clone()` / `executeMove()` without touching move-generation
  code at all.
- **Alternate front end** (GUI, network play): `Game`'s public interface
  (`makeMove`, `saveGame`, `loadGame`, `printBoard`) is independent of the
  console loop in `run()`, so a different front end can drive the same engine.

## Known limitations

- Threefold-repetition draws are not implemented (fifty-move rule is).
- `undo` snapshots are in-memory only and do not persist across a save/load
  round trip (a reloaded game's history is notation-only, for display).
- Algebraic notation output is simplified (`e2-e4`, `e4xd5`, `O-O`, `e7-e8=Q`)
  rather than full disambiguated SAN.
