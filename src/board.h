#include <stdio.h>
#include <stdbool.h>

// Your provided Enum
enum Piece {
    WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WPAWN,
    BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BPAWN,
    EMPTY
}; 

// Simple coordinate struct
struct Square {
    int row;
    int col;
};

// A list to hold the possible moves (Max moves for any piece is rarely > 27)
struct MoveList {
    struct Square squares[64];
    int count;
};

bool IsOnBoard(int r, int c) {
    return (r >= 0 && r < 8 && c >= 0 && c < 8);
}

bool IsWhite(enum Piece p) {
    return (p >= WROOK && p <= WPAWN);
}

bool IsBlack(enum Piece p) {
    return (p >= BROOK && p <= BPAWN);
}

// Returns true if p1 (mover) captures p2 (target)
bool IsOpponent(enum Piece mover, enum Piece target) {
    if (target == EMPTY)
      return false;
    else
      return (IsWhite(mover) && IsBlack(target)) || (IsBlack(mover) && IsWhite(target));
}

void GetSlidingMoves(const enum Piece board[8][8], struct Square start, struct MoveList* list, int dirs[][2], int numDirs) {
    enum Piece myPiece = board[start.row][start.col];

    for (int i = 0; i < numDirs; i++) {
        int r = start.row + dirs[i][0];
        int c = start.col + dirs[i][1];

        while (IsOnBoard(r, c)) {
            enum Piece target = board[r][c];

            // 1. Empty square: Valid move, continue sliding
            if (target == EMPTY) {
                list->squares[list->count++] = (struct Square){r, c};
            }
            // 2. Occupied square
            else {
                // If enemy, we can capture (valid move), then stop sliding
                if (IsOpponent(myPiece, target)) {
                    list->squares[list->count++] = (struct Square){r, c};
                }
                // If friend or enemy, the path is blocked here. Stop.
                break; 
            }

            // Move to next square in this direction
            r += dirs[i][0];
            c += dirs[i][1];
        }
    }
}

void GetSteppingMoves(const enum Piece board[8][8], struct Square start, struct MoveList* list, int offsets[][2], int numOffsets) {
    enum Piece myPiece = board[start.row][start.col];

    for (int i = 0; i < numOffsets; i++) {
        int r = start.row + offsets[i][0];
        int c = start.col + offsets[i][1];

        if (IsOnBoard(r, c)) {
            enum Piece target = board[r][c];
            // Valid if empty OR enemy (capture)
            if (target == EMPTY || IsOpponent(myPiece, target)) {
                list->squares[list->count++] = (struct Square){r, c};
            }
        }
    }
}

void init_board(enum Piece board[8][8]) {
  enum Piece tmp[8][8] = {
    { BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK },
    { BPAWN, BPAWN,   BPAWN,   BPAWN,  BPAWN, BPAWN,   BPAWN,   BPAWN },
    { EMPTY, EMPTY,   EMPTY,   EMPTY,  EMPTY, EMPTY,   EMPTY,   EMPTY },
    { EMPTY, EMPTY,   EMPTY,   EMPTY,  EMPTY, EMPTY,   EMPTY,   EMPTY },
    { EMPTY, EMPTY,   EMPTY,   EMPTY,  EMPTY, EMPTY,   EMPTY,   EMPTY },
    { EMPTY, EMPTY,   EMPTY,   EMPTY,  EMPTY, EMPTY,   EMPTY,   EMPTY },
    { WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK },
    { WPAWN, WPAWN,   WPAWN,   WPAWN,  WPAWN, WPAWN,   WPAWN,   WPAWN }
  };
  
  memcpy(board, tmp, sizeof(tmp));
  printf("board reset\n");
}
