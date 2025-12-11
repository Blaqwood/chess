#include <raylib.h>
#include <stdio.h>
#include <string.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>


#define Rectangle(x, y, w, h) (Rectangle) { x, y, w, h }
#define Vector2(x, y) (Vector2) { x, y }

enum Type { King = 75, Queen = 81, Bishop = 66, Knight = 107, Rook = 82, Pawn = 80 };

enum Colour { White, Black };

enum State {
  PLAY,
  PAUSE,
  EXIT
};

struct Piece {
  enum Type type;
  int colour;
  int x;
  int y;
  Texture2D texture;
};

void init_board(struct Piece *pieces);
void setup_board(struct Piece *pieces, struct Piece *board[8][8]);
int in_bounds(int x, int y);
int path_clear(struct Piece *board[8][8], int x1, int y1, int x2, int y2);
int is_valid_move(struct Piece *board[8][8], struct Piece *p, int new_x, int new_y);
struct Move *get_valid_moves(struct Piece *board[8][8], struct Piece *p, int *count_out);

int main() {
  int in_game = false;

  int width = 1280;
  int height = 720;
  int bheight = height - 120;
  int sheight = bheight / 8;
  int iheight = sheight - 10;
  int owidth = width - height;

  InitWindow(width, height, "test");
  Font chess_icon_font = LoadFont("assets/fonts/BoldPixels.ttf");

  struct Piece pieces[32];
  struct Piece *board[8][8];
  struct Piece *selected = NULL;

  init_board(pieces);
  init_board(pieces);
  setup_board(pieces, board);

  SetTargetFPS(30);
  int reset = 0;
  int quit = 0;
  enum State state = PLAY;

  while (!WindowShouldClose() && state != EXIT) {
    Vector2 mouse_pos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      /*
      if (mouse_pos.x < 60 || mouse_pos.x > bheight + 60 || mouse_pos.y < 60 || mouse_pos.y > bheight + 60)
              continue;
      */
      int col = -1;
      int row = -1;

      if (mouse_pos.x > 60 && mouse_pos.x < bheight + 60 && mouse_pos.y > 60 && mouse_pos.y < bheight + 60) {
        col = mouse_pos.x / sheight - 1;
        row = mouse_pos.y / sheight - 1;
      }

      if (col != -1 && row != -1) {
        struct Piece *temp = board[row][col];
        if (selected == temp) {
          selected = NULL;
          printf("deselected: (%d, %d)\n", col, row);
        }
        else if (selected == NULL) {
          selected = temp;
          printf("selected: (%d, %d)\n", col, row);
        }
      }
    }
    
    if (IsKeyPressed(KEY_SPACE)) {
      init_board(pieces);
      puts("Board reset");
    }
    
    BeginDrawing();
    ClearBackground(WHITE);


    // draw grid
    for (int i = 0; i < 8; i += 1) {
      for (int j = 0; j < 8; j += 1) {
        DrawRectangleLines(sheight * i + 60, sheight * j + 60, sheight, sheight, BLACK);
      }
    }

    // draw sprites
    for (int i = 0; i < 32; i += 1) {
      struct Piece *p = &pieces[i];
      Texture2D t = p->texture;
      DrawTexturePro(t, Rectangle(0, 0, t.width, t.height), Rectangle((p->x * sheight) + 65, (p->y * sheight) + 65, iheight, iheight), Vector2(0, 0), 0.0, WHITE);
    }
    
    
    // draw gui
    // ************************
    if (GuiButton(Rectangle(height + owidth / 2, 200, 100, 70), GuiIconText(ICON_RESTART, "Reset"))) {
      reset = 1;
      continue;
    }
    quit = GuiButton(Rectangle(height + owidth / 2, 100, 100, 70), GuiIconText(ICON_UNDO, "Quit"));

    GuiLabel(Rectangle(height + owidth / 2, 500, 100, 70), TextFormat("Mouse (%d, %d)", mouse_pos.x, mouse_pos.y));
      
    if (selected != NULL) {
      GuiLabel(Rectangle(height + owidth / 2, 300, 100, 70), TextFormat("Selected: (%d, %d)", selected->x + 1, selected->y + 1));
      DrawRectangleLinesEx(Rectangle(selected->x * sheight + 60, selected->y * sheight + 60, sheight, sheight), 5, BLUE);
    }

    
    // ************************
    if (quit) {
      int o = GuiMessageBox(Rectangle(width / 2, height / 2, 300, 200), "Quit", "Are you sure you want to quit?", "Yes;No");
      if (o) {
        break;
      }
    }

    // react to gui events
    if (reset) {
      if (GuiMessageBox(Rectangle(width / 2, height / 2, 300, 200), "Reset", "Are you sure you want to reset the board?", "Yes;No")) {
        init_board(pieces);
      }
      reset = 0;
    }
    
    EndDrawing();
  }

  for (int i = 0; i < 32; i += 1) {
    UnloadTexture(pieces[i].texture);
  }

  CloseWindow();

  return 0;
}

void init_board(struct Piece *pieces) {
  static int texture_loads = 0;

  // if textures was loaded before
  if (texture_loads > 0) {
    int tmp[32][2] = {
      {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1},
      {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7}, {7, 7}
    };
    for (int i = 0; i < 32; i += 1) {
        pieces[i].x = tmp[i][0];
        pieces[i].y = tmp[i][1];
    }
  }
  // otherwise load textures for the first time
  else {
    struct Piece tmp[32] = {
      (struct Piece) {Rook, Black, 0, 0, LoadTexture("assets/sprites/rook_b.png")}, (struct Piece) {Knight, Black, 1, 0, LoadTexture("assets/sprites/knight_b.png")},
      (struct Piece) {Bishop, Black, 2, 0, LoadTexture("assets/sprites/rook_b.png")}, (struct Piece) {Queen, Black, 3, 0, LoadTexture("assets/sprites/queen_b.png")},
      (struct Piece) {King, Black, 4, 0, LoadTexture("assets/sprites/king_b.png")}, (struct Piece) {Bishop, Black, 5, 0, LoadTexture("assets/sprites/bishop_b.png")},
      (struct Piece) {Knight, Black, 6, 0, LoadTexture("assets/sprites/knight_b.png")}, (struct Piece) {Rook, Black, 7, 0, LoadTexture("assets/sprites/rook_b.png")},
      (struct Piece) {Pawn, Black, 0, 1, LoadTexture("assets/sprites/pawn_b.png")}, (struct Piece) {Pawn, Black, 1, 1, LoadTexture("assets/sprites/pawn_b.png")},
      (struct Piece) {Pawn, Black, 2, 1, LoadTexture("assets/sprites/pawn_b.png")}, (struct Piece) {Pawn, Black, 3, 1, LoadTexture("assets/sprites/pawn_b.png")},
      (struct Piece) {Pawn, Black, 4, 1, LoadTexture("assets/sprites/pawn_b.png")}, (struct Piece) {Pawn, Black, 5, 1, LoadTexture("assets/sprites/pawn_b.png")},
      (struct Piece) {Pawn, Black, 6, 1, LoadTexture("assets/sprites/pawn_b.png")}, (struct Piece) {Pawn, Black, 7, 1, LoadTexture("assets/sprites/pawn_b.png")},
      (struct Piece) {Rook, White, 0, 7, LoadTexture("assets/sprites/rook_w.png")}, (struct Piece) {Knight, White, 1, 7, LoadTexture("assets/sprites/knight_w.png")},
      (struct Piece) {Bishop, White, 2, 7, LoadTexture("assets/sprites/bishop_w.png")}, (struct Piece) {Queen, White, 3, 7, LoadTexture("assets/sprites/queen_w.png")},
      (struct Piece) {King, White, 4, 7, LoadTexture("assets/sprites/king_w.png")}, (struct Piece) {Bishop, White, 5, 7, LoadTexture("assets/sprites/bishop_w.png")},
      (struct Piece) {Knight, White, 6, 7, LoadTexture("assets/sprites/knight_w.png")}, (struct Piece) {Rook, White, 7, 7, LoadTexture("assets/sprites/rook_w.png")},
      (struct Piece) {Pawn, White, 0, 6, LoadTexture("assets/sprites/pawn_w.png")}, (struct Piece) {Pawn, White, 1, 6, LoadTexture("assets/sprites/pawn_w.png")},
      (struct Piece) {Pawn, White, 2, 6, LoadTexture("assets/sprites/pawn_w.png")}, (struct Piece) {Pawn, White, 3, 6, LoadTexture("assets/sprites/pawn_w.png")},
      (struct Piece) {Pawn, White, 4, 6, LoadTexture("assets/sprites/pawn_w.png")}, (struct Piece) {Pawn, White, 5, 6, LoadTexture("assets/sprites/pawn_w.png")},
      (struct Piece) {Pawn, White, 6, 6, LoadTexture("assets/sprites/pawn_w.png")}, (struct Piece) {Pawn, White, 7, 6, LoadTexture("assets/sprites/pawn_w.png")}
    };
  
    memcpy(pieces, tmp, sizeof(tmp));
  }
  
  texture_loads += 1;
}

void setup_board(struct Piece *pieces, struct Piece *board[8][8]) {
  for (int i = 0; i < 8; i += 1) {
    for (int j = 0; j < 8; j += 1) {
      board[i][j] = NULL;
    }
  }

  for (int i = 0; i < 32; i += 1) {
    int x = pieces[i].x;
    int y = pieces[i].y;
    board[y][x] = &pieces[i];
  }
}
