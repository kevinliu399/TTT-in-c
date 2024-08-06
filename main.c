#include "raylib.h"
#include <stdbool.h>

#define ROW 3
#define COL 3
#define MAX_PLAYERS 2

/************************
 * Types and definition *
 ************************/
typedef struct Tile {
    Rectangle square;
    Color color;
    bool isEmpty;
} Tile;

typedef struct Player {
    bool isBlueTeam; // Blue & Red team
    bool hasPlayed;
    bool hasWon;
} Player;

/*******************
 *  Global Variables Declarations
 *****************/

static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;

// initialize these fields
static Player player[MAX_PLAYERS] = { 0 };
static Tile tile[ROW][COL] = { 0 };

// Module Declaration
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UpdateDrawFrame(void);

static void InitPlayers(void);
static bool UpdatePlayer(int playerTurn);
static void InitBoard(void);

// Program main entry point

int main(void) {

    InitWindow(screenWidth, screenHeight, "Tic-Tac-Toe");
    InitGame();

    SetTargetFPS(60);
    while(!WindowShouldClose()) //  Detect window close button or ESC key
    {
        UpdateDrawFrame(); // Update and Draw
    }

    UnloadGame(); // unload data
    CloseWindow(); // close window and opengl

    return 0;
}

// Module functions definitions

void InitGame(void) {

    InitBoard();
    InitPlayers();
}

void UpdateGame(void) {
    static int currentPlayer = 0; // blue team starts
    if (!gameOver) {
        if (UpdatePlayer(currentPlayer)) {
            // check for wincon

            currentPlayer = (currentPlayer + 1) % MAX_PLAYERS;
        }

    }
}

static void UpdateDrawFrame(void) {
    updateGame();
    DrawGame();
}

static void DrawGame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // TODO: draw game board
    EndDrawing();
}

static void InitPlayer(void) {
    player[0] = (Player){.isBlueTeam = true, .hasPlayed = false , .hasWon = false};
    player[1] = (Player){.isBlueTeam = false, .hasPlayed = false , .hasWon = false};
}

static void InitBoard(void) {
    for (int i = 0; i < ROW; i ++) {
        for (int j = 0; j < COL; j ++) {
            tile[i][j] = (Tile) {
                .square = (Rectangle){ j * (screenWidth / 3.0f), i * (screenHeight / 3.0f), screenWidth / 3.0f, screenHeight / 3.0f },
                .color = LIGHTGRAY,
                .isEmpty = true,
            };
        }
    }
}

static bool UpdatePlayer(int playerTurn) {
    return false;
}

static void UnloadGame(void) {
    // TODO 
}
