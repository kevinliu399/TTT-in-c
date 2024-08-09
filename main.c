#include "raylib.h"
#include <stdbool.h>
#include <string.h>

#define ROW 3
#define COL 3
#define MAX_TILES 9
#define MAX_PLAYERS 2
#define HEADER_HEIGHT 60
#define HEADER_BUTTON_WIDTH 40
#define HEADER_BUTTON_HEIGHT 40

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

/**********************************
 *  Global Variables Declarations *
 **********************************/

static const int screenWidth = 800;
static const int screenHeight = 510;
static const int gameBoardTop = HEADER_HEIGHT;
static const int gameBoardHeight = 450;

static bool gameOver = false;
static int currentPlayer = 0; // blue team starts
static int totalMoves = 0;    // if we reach the total moves, then the game is over

static Vector2 hoveredTile = {-1, -1};

static Player player[MAX_PLAYERS] = {0};
static Tile tile[ROW][COL] = {0};
static Rectangle headerButton = {0};

/***********************
 *  Module Declaration *
 ***********************/

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UpdateDrawFrame(void);

static void InitPlayers(void);
static bool UpdatePlayer(int playerTurn);
static void InitBoard(void);
static bool checkWin(void);

/***********
 * Program *
 ***********/

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

void InitGame(void)
{

    InitBoard();
    InitPlayers();

    // Temporary
    headerButton = (Rectangle){
        screenWidth - HEADER_BUTTON_WIDTH - 10,
        10,
        HEADER_BUTTON_WIDTH,
        HEADER_BUTTON_HEIGHT};
}

void UpdateGame(void)
{
    if (!gameOver)
    {
        if (UpdatePlayer(currentPlayer))
        {

            // game ends if all tiles are filled or if someone wins
            if (totalMoves == MAX_TILES - 1 || checkWin())
            {
                gameOver = true;
            }

            totalMoves++; // one turn
            currentPlayer = (currentPlayer + 1) % MAX_PLAYERS;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
            totalMoves = 0;
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

    // Draw header
    DrawRectangle(0, 0, screenWidth, HEADER_HEIGHT, LIGHTGRAY);
    DrawRectangleRec(headerButton, GRAY);
    DrawRectangleLinesEx(headerButton, 2, BLACK);
    DrawText("AI", headerButton.x + 10, headerButton.y + 10, 20, BLACK);

    float squareSize = 40.0f;
    float margin = 10.0f;
    DrawRectangle(margin, margin, squareSize, squareSize, BLUE);
    DrawRectangle(margin + squareSize + margin, margin, squareSize, squareSize, RED);
    
    if (currentPlayer == 0)
        DrawRectangleLinesEx((Rectangle){margin, margin, squareSize, squareSize}, 3, GOLD);
    else
        DrawRectangleLinesEx((Rectangle){margin + squareSize + margin, margin, squareSize, squareSize}, 3, GOLD);

    // Draw game board
    for (int i = 1; i < 3; i++)
    {
        DrawLineEx(
            (Vector2){screenWidth / 3 * i, gameBoardTop},
            (Vector2){screenWidth / 3 * i, screenHeight},
            5, BLACK);
        DrawLineEx(
            (Vector2){0, gameBoardTop + gameBoardHeight / 3 * i},
            (Vector2){screenWidth, gameBoardTop + gameBoardHeight / 3 * i},
            5, BLACK);
    }

    // Draw tiles
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

    if (!gameOver)
    {
        if (hoveredTile.x != -1 && hoveredTile.y != -1)
        {
            DrawRectangleLinesEx(tile[(int)hoveredTile.x][(int)hoveredTile.y].square, 6, DARKGRAY);
        }
    }
    else
    {
        const char *resultText;
        if (player[0].hasWon) resultText = "Blue team has won!";
        else if (player[1].hasWon) resultText = "Red team has won!";
        else resultText = "Tie game!";

        DrawText(resultText, screenWidth / 2 - MeasureText(resultText, 40) / 2, gameBoardTop + gameBoardHeight / 2 - 50, 40, BLACK);
        DrawText("PRESS [ENTER] TO PLAY AGAIN", screenWidth / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, gameBoardTop + gameBoardHeight / 2, 20, DARKGRAY);
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
        .profile = (Rectangle){margin, margin, squareSize, squareSize}};
    player[1] = (Player){
        .isBlueTeam = false,
        .hasWon = false,
        .profile = (Rectangle){margin + squareSize + 2 * margin, margin, squareSize, squareSize}};
}

static void InitBoard(void)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            tile[i][j] = (Tile){
                .square = (Rectangle){
                    j * (screenWidth / 3.0f),
                    gameBoardTop + i * (gameBoardHeight / 3.0f),
                    screenWidth / 3.0f,
                    gameBoardHeight / 3.0f},
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

    if (mousePosition.y >= gameBoardTop && mousePosition.y <= screenHeight) {
        // Adjust mouse position for game board
        mousePosition.y -= gameBoardTop;

        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                if (CheckCollisionPointRec(mousePosition, (Rectangle){
                                                            tile[i][j].square.x,
                                                            tile[i][j].square.y - gameBoardTop,
                                                            tile[i][j].square.width,
                                                            tile[i][j].square.height}))
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
    }


    return false;
}
