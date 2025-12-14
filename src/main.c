#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "board.h"

#define rec(x, y, w, h) (Rectangle) { x, y, w, h }
#define vec2(x, y) (Vector2) { x, y }

int main() {
  const int width = 1280;
  const int height = 720;
  const int bheight = height - 120;
  const int sheight = bheight / 8;
  const int iheight = sheight - 10;
  const int owidth = width - height;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(width, height, "Chess");
  SetTargetFPS(60);

  Texture2D icons[13] = {
    LoadTexture("assets/sprites/rook_w.png"), LoadTexture("assets/sprites/knight_w.png"), LoadTexture("assets/sprites/bishop_w.png"),
    LoadTexture("assets/sprites/queen_w.png"), LoadTexture("assets/sprites/king_w.png"), LoadTexture("assets/sprites/pawn_w.png"),
    LoadTexture("assets/sprites/rook_b.png"), LoadTexture("assets/sprites/knight_b.png"), LoadTexture("assets/sprites/bishop_b.png"),
    LoadTexture("assets/sprites/queen_b.png"), LoadTexture("assets/sprites/king_b.png"), LoadTexture("assets/sprites/pawn_b.png"),
    LoadTextureFromImage(GenImageColor(0, 0, BLANK)) // last one is an empty texture for empty squares
  };
  
  // chess grid
  enum Piece board[8][8];
  
  // square selected by the player ( -1 means not selected )
  int selected = 1;
  int selected_x = 2;
  int selected_y = 6;
  Vector2 mouse;
  
  float time = 0.0f;
  float alpha;
  int alpha_rev = 0;
  
  // setup board
  init_board(board);

  while (!WindowShouldClose()) {
    /*
    ================ Update ================
    */
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      mouse = GetMousePosition();
      selected = !selected;
    }
    
    const int gridx = (GetScreenWidth() / 2) - (GetScreenHeight() / 2);
    float time = GetFrameTime();
    
    if (!alpha_rev)
      alpha += time * 200.0;
    else
      alpha -= time * 200.0;
    
    if (alpha > 255) {
      alpha_rev = 1;
      alpha = 255;
    }
    else if (alpha < 0) {
      alpha_rev = 0;
      alpha = 0;
    }
    
    /*
    ================= Draw =================
    */
    BeginDrawing();
    ClearBackground(WHITE);
    
    // draw grid
    for (int i = 0; i <= 8; i++) { // <= to draw the closing line
      DrawLineEx(vec2(gridx + i * (height / 8), 0), vec2(gridx + i * (height / 8), height), 5, BLACK);
      DrawLineEx(vec2(gridx, i * (height / 8)),     vec2(gridx + height, i * (height / 8)), 5, BLACK);
    }
    
    // draw sprites
    for (int i = 0; i < 8; i += 1) {
      for (int j = 0; j < 8; j += 1) {
        // index into the icons array and match the type of piece to the correct texture
        Texture2D texture;
        Rectangle original;
        Rectangle scaled;
        
        /*
        // old method that checks type individually
        switch (board[i][j]) {
          case EMPTY:   continue; // skip loop and skip drawing texture
          case WROOK:   texture = icons[0]; break;
          case WKNIGHT: texture = icons[1]; break;
          case WBISHOP: texture = icons[2]; break;
          case WQUEEN:  texture = icons[3]; break;
          case WKING:   texture = icons[4]; break;
          case WPAWN:   texture = icons[5]; break;
          case BROOK:   texture = icons[6]; break;
          case BKNIGHT: texture = icons[7]; break;
          case BBISHOP: texture = icons[8]; break;
          case BQUEEN:  texture = icons[9]; break;
          case BKING:   texture = icons[10]; break;
          case BPAWN:   texture = icons[11]; break;
          default: break;
        }
        */
        
        texture = icons[board[i][j]];
        original = rec(0, 0, texture.width, texture.height);
        scaled = rec(gridx + 5 + j * (height / 8), 5 + i * (height / 8), height / 8 - 10, height / 8 - 10);
        
        DrawTexturePro(texture, original, scaled, vec2(0, 0), 0.0, WHITE);
      }
    }
    
    // draw box for selected square
    if (selected) {
      DrawRectangleLinesEx(rec(gridx + selected_x * (height / 8), selected_y * (height / 8), height / 8, height / 8), 8, (Color) {0, 255, 0, alpha});
    }
    
    EndDrawing();
  }

  for (int i = 0; i < 12; i += 1) {
    UnloadTexture(icons[i]);
  }

  CloseWindow();

  return 0;
}


