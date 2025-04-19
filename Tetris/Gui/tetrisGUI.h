#ifndef TETRIS_GUI
#define TETRIS_GUI

#include "../Engine/library.h"

// Draws starting screen
void StartingScreen(GameInfo_t game);

// Draws the in game screen
void UpdateGameScreen(GameInfo_t game);

// Draws endgame screen
void EndGameScreen(GameInfo_t game);

// Draws corresponding piece for "next" field
void DrawNext(GameInfo_t game, char **dest);

// Draws line
void DrawLine(char **dest);

// Draws line
void DrawLA(char **dest);

// Draws line
void DrawRA(char **dest);

// Draws line
void DrawLZ(char **dest);

// Draws line
void DrawRZ(char **dest);

// Draws line
void DrawSquare(char **dest);

// Draws line
void DrawT(char **dest);

#endif  // TETRIS_GUI