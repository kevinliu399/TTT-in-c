#include "raylib.h"
#include <stdbool.h>

#define ROW 3
#define COL 3
#define MAX_TILES 9
#define MAX_PLAYERS 2

/************************
 * Types and definition *
 ************************/
typedef struct Tile
{
    Rectangle square;
    Color color;
    int playerIndex; // -1 for empty, 0 for player, 1 for player 2
} Tile;

typedef struct Player
{
    bool isBlueTeam; // Blue & Red team
    bool hasWon;
    Rectangle profile; 
} Player;

/*******************
 *  Global Variables Declarations
 *****************/

static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;
static int currentPlayer = 0; // blue team starts
static int totalMoves = 0;    // if we reach the total moves, then the game is over

static Vector2 hoveredTile = {-1, -1};

// initialize these fields
static Player player[MAX_PLAYERS] = {0};
static Tile tile[ROW][COL] = {0};

// Module Declaration
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UpdateDrawFrame(void);

static void InitPlayers(void);
static bool UpdatePlayer(int playerTurn);
static void InitBoard(void);

// Program main entry point

int main(void)
{

    InitWindow(screenWidth, screenHeight, "Tic-Tac-Toe");
    InitGame();

    SetTargetFPS(60);
    while (!WindowShouldClose()) //  Detect window close button or ESC key
    {
        UpdateDrawFrame(); // Update and Draw
    }

    UnloadGame();  // unload data
    CloseWindow(); // close window and opengl

    return 0;
}

// Module functions definitions

void InitGame(void)
{

    InitBoard();
    InitPlayers();
}

void UpdateGame(void)
{
    if (!gameOver)
    {
        if (UpdatePlayer(currentPlayer))
        {

            // game ends if all tiles are filled or if someone wins
            if (totalMoves == MAX_TILES || checkWin())
            {
                gameOver = true;
            }

            totalMoves++; // one turn
            currentPlayer = (currentPlayer + 1) % MAX_PLAYERS;
        }
    }
}

static void UpdateDrawFrame(void)
{
    updateGame();
    DrawGame();
}

static void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (!gameOver)
    {   
        // Draw Tiles
        if (hoveredTile.x != -1 && hoveredTile.y != -1) {
            DrawRectangleLinesEx(tile[(int)hoveredTile.x][(int)hoveredTile.y].square, 2, BLACK);
        }

        // Player profile
        DrawRectangleRec(player[0].profile, BLUE);
        DrawRectangleRec(player[1].profile, RED);

        DrawRectangleLinesEx(player[0].profile, 2, BLACK);
        DrawRectangleLinesEx(player[1].profile, 2, BLACK);

        if (currentPlayer == 0) {
            DrawRectangleLinesEx(player[0].profile, 4, GOLD);
        } else {
            DrawRectangleLinesEx(player[1].profile, 4, GOLD);
        }


    }
    EndDrawing();
}

static void InitPlayer(void)
{
    float squareSize = 30.0f;
    float margin = 10.0f;

    player[0] = (Player){.isBlueTeam = true, .hasWon = false, .profile = (Rectangle) { margin, margin, squareSize, squareSize }};
    player[1] = (Player){.isBlueTeam = false, .hasWon = false, .profile = (Rectangle) {margin, margin, squareSize, squareSize }};

}

static void InitBoard(void)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            tile[i][j] = (Tile){
                .square = (Rectangle){j * (screenWidth / 3.0f), i * (screenHeight / 3.0f), screenWidth / 3.0f, screenHeight / 3.0f},
                .color = LIGHTGRAY,
                .playerIndex = -1, // -1 means empty
            };
        }
    }
}

static bool checkWin(void)
{
    // check horizontally
    for (int i = 0; i < ROW; i++)
    {
        if (tile[i][0].playerIndex != -1 &&
            tile[i][0].playerIndex == tile[i][1].playerIndex &&
            tile[i][0].playerIndex == tile[i][2].playerIndex)
        {
            player[tile[i][0].playerIndex].hasWon = true;
            return true;
        }
    }

    // check vertically
    for (int i = 0; i < COL; i++)
    {
        if (tile[0][i].playerIndex != -1 &&
            tile[0][i].playerIndex == tile[1][i].playerIndex &&
            tile[0][i].playerIndex == tile[2][i].playerIndex)
        {
            player[tile[0][i].playerIndex].hasWon = true;
            return true;
        }
    }

    // check diagonals
    if (tile[1][1].playerIndex != -1)
    {
        if ((tile[0][0].playerIndex == tile[1][1].playerIndex && tile[1][1].playerIndex == tile[2][2].playerIndex) ||
            (tile[0][2].playerIndex == tile[1][1].playerIndex && tile[1][1].playerIndex == tile[2][0].playerIndex))
        {
            player[tile[1][1].playerIndex].hasWon = true;
            return true;
        }
    }

    return false;
}

static bool UpdatePlayer(int playerTurn)
{
    Vector2 mousePosition = GetMousePosition();
    hoveredTile.x = -1;
    hoveredTile.y = -1;

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (CheckCollisionPointRec(mousePosition, tile[i][j].square))
            {
                hoveredTile.x = i;
                hoveredTile.y = j;
                
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tile[i][j].playerIndex == -1)
                {
                    tile[i][j].playerIndex = playerTurn;
                    tile[i][j].color = player[playerTurn].isBlueTeam ? BLUE : RED;
                    return true;
                }
            }
        }
    }

    return false;
}

static void UnloadGame(void)
{
    // TODO
}
