#include "raylib.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

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
static bool checkWin(void);

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
            if (totalMoves == MAX_TILES - 1|| checkWin())
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
    UpdateGame();
    DrawGame();
}

static void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw Tiles
    for (int i = 1; i < 3; i++)
    {
        DrawLineEx((Vector2){screenWidth / 3 * i, 0}, (Vector2){screenWidth / 3 * i, screenHeight}, 5, BLACK);
        DrawLineEx((Vector2){0, screenHeight / 3 * i}, (Vector2){screenWidth, screenHeight / 3 * i}, 5, BLACK);
    }

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (tile[i][j].playerIndex != -1)
            {
                Player playerTile = player[tile[i][j].playerIndex];
                Color tileColor = playerTile.isBlueTeam ? BLUE : RED;
                DrawRectangle(
                    (int)tile[i][j].square.x,
                    (int)tile[i][j].square.y,
                    (int)tile[i][j].square.width,
                    (int)tile[i][j].square.height,
                    tileColor);
            }
        }
    }

    if (hoveredTile.x != -1 && hoveredTile.y != -1)
    {
        DrawRectangleLinesEx(tile[(int)hoveredTile.x][(int)hoveredTile.y].square, 6, DARKGRAY);
    }

    // Player profile
    DrawRectangleRec(player[0].profile, BLUE);
    DrawRectangleRec(player[1].profile, RED);

    DrawRectangleLinesEx(player[0].profile, 2, BLACK);
    DrawRectangleLinesEx(player[1].profile, 2, BLACK);

    if (currentPlayer == 0)
    {
        DrawRectangleLinesEx(player[0].profile, 4, GOLD);
    }
    else
    {
        DrawRectangleLinesEx(player[1].profile, 4, GOLD);
    }

    if (gameOver)
    {
        const char* resultText;
        if (player[0].hasWon)
            resultText = "Blue team has won!";
        else if (player[1].hasWon)
            resultText = "Red team has won!";
        else
            resultText = "Tie game!";

        DrawText(resultText, screenWidth / 2 - MeasureText(resultText, 40) / 2, screenHeight / 2 - 20, 40, BLACK);
    }

    EndDrawing();
}

static void InitPlayers(void)
{
    float squareSize = 50.0f;
    float margin = 10.0f;

    player[0] = (Player){
        .isBlueTeam = true,
        .hasWon = false,
        .profile = (Rectangle){margin, margin, squareSize, squareSize}
    };
    player[1] = (Player){
        .isBlueTeam = false,
        .hasWon = false,
        .profile = (Rectangle){margin + squareSize + 2 * margin, margin, squareSize, squareSize}
    };
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
