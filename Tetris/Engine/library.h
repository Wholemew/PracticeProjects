#ifndef TETRIS_LIB
#define TETRIS_LIB

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WID 10  // width of playing field
#define HEI 20  // height of playing field

#define MAXU 4294967295  // Maximum value of unsigned

#define EMP 32  // code of empty space in the field
#define FIL 35  // code of filled space in the field

// Types of user actions that may happen in the game
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

// Types of figures that may be generated in the game
typedef enum { Line, LeftAngle, RightAngle, LeftZ, RightZ, Square, T } Figure_t;

typedef struct {
  Figure_t type;
  unsigned short posx;  // Position of top left corner of draw window
  unsigned short posy;
  char rotation;  // Number of clockwise turns from initial state
} Falling_t;      // Description of falling figure

// General state of the current game
typedef struct {
  Falling_t piece;
  char **field;
  Figure_t next;
  unsigned score;
  unsigned high_score;
  unsigned short speed;
  char level;
  char pause;  // 0 for play, 1 for pause, 2 for starting screen, 3 for game end
               // screen, 4 if an error occured
} GameInfo_t;

GameInfo_t *Init(GameInfo_t *tetris);  // Initiates field and sets up the game
                                       // at starting screen

// Frees memory from static structure in library.c
void Destroy(GameInfo_t *tetris);

void StartGame(GameInfo_t *tetris);  // Starts new game

// Reads user input and deals with consequences
void userInput(UserAction_t action, char hold);

// Returns static game info structure from library.c as is for gui
GameInfo_t *updateCurrentState();

// Pause or unpause in game
void PauseResume(GameInfo_t *tetris);

// Ends game and updates high score
void EndGame(GameInfo_t *tetris);

// Adjusts function to shift the piece to the left
void ShiftLeft(GameInfo_t *tetris);

// Adjusts function to shift the piece to the right
void ShiftRight(GameInfo_t *tetris);

// Adjusts function to rotate the piece
void Rotate(GameInfo_t *tetris);

// Adjusts function to draw the piece into the field
void PutPiece(GameInfo_t *tetris);

// Adjusts function to erase the piece from the field
void CleanPiece(GameInfo_t *tetris);

// Drops the piece down until it connects
void Drop(GameInfo_t *tetris);

// Moves the piece one step down
void MoveDown(GameInfo_t *tetris);

// Generates the type of the next piece
void GenerateNext(GameInfo_t *tetris);

// Spawns next piece in the field and generates new next
void Spawn(GameInfo_t *tetris);

// Adjusts function to check the connection of the piece
char Connect(GameInfo_t *tetris);

// Adjusts function to check if the game has ended
char CheckEnd(GameInfo_t *tetris);

// Moves piece by a given shift in x and y
void MovePiece(char x, char y, GameInfo_t *tetris);

// Checks and removes the filled rows and updates score
void CheckApplyScore(GameInfo_t *tetris);

// Updates level and speed
void updateLevel(GameInfo_t *tetris);

// Removes row on r coordinate and shifts upper rows one position down
void RemoveRow(char r, GameInfo_t *tetris);

// Shifts Line one position Left
void LiShiftLeft(GameInfo_t *tetris);

// Shifts Left Angle one position Left
void LAShiftLeft(GameInfo_t *tetris);

// Shifts Right Angle one position Left
void RAShiftLeft(GameInfo_t *tetris);

// Shifts Left Zigzag one position Left
void LZShiftLeft(GameInfo_t *tetris);

// Shifts Right Zigzag one position Left
void RZShiftLeft(GameInfo_t *tetris);

// Shifts Square one position Left
void SqShiftLeft(GameInfo_t *tetris);

// Shifts T one position Left
void TShiftLeft(GameInfo_t *tetris);

// Shifts Line one position Right
void LiShiftRight(GameInfo_t *tetris);

// Shifts Left Angle one position Right
void LAShiftRight(GameInfo_t *tetris);

// Shifts Right Angle one position Right
void RAShiftRight(GameInfo_t *tetris);

// Shifts Left Zigzag one position Right
void LZShiftRight(GameInfo_t *tetris);

// Shifts Right Zigzag one position Right
void RZShiftRight(GameInfo_t *tetris);

// Shifts Square one position Right
void SqShiftRight(GameInfo_t *tetris);

// Shifts T one position Right
void TShiftRight(GameInfo_t *tetris);

// Rotates Line
void LiRotate(GameInfo_t *tetris);

// Rotates Left Angle
void LARotate(GameInfo_t *tetris);

// Rotates Right Angle
void RARotate(GameInfo_t *tetris);

// Rotates Left Zigzag
void LZRotate(GameInfo_t *tetris);

// Rotates Right Zigzag
void RZRotate(GameInfo_t *tetris);

// Rotates T
void TRotate(GameInfo_t *tetris);

// Erases line on piece position from the field
void CleanLine(GameInfo_t *tetris);

// Erases left angle on piece position from the field
void CleanLA(GameInfo_t *tetris);

// Erases right angle on piece position from the field
void CleanRA(GameInfo_t *tetris);

// Erases left zigzag on piece position from the field
void CleanLZ(GameInfo_t *tetris);

// Erases right zigzag on piece position from the field
void CleanRZ(GameInfo_t *tetris);

// Erases square on piece position from the field
void CleanSquare(GameInfo_t *tetris);

// Erases T on piece position from the field
void CleanT(GameInfo_t *tetris);

// Draws line on piece position in the field
void PutLine(GameInfo_t *tetris);

// Draws left angle on piece position in the field
void PutLA(GameInfo_t *tetris);

// Draws right angle on piece position in the field
void PutRA(GameInfo_t *tetris);

// Draws left zigzag on piece position in the field
void PutLZ(GameInfo_t *tetris);

// Draws right zigzag on piece position in the field
void PutRZ(GameInfo_t *tetris);

// Draws square on piece position in the field
void PutSquare(GameInfo_t *tetris);

// Draws T on piece position in the field
void PutT(GameInfo_t *tetris);

// Checks the connection of the Line
char LiConnect(GameInfo_t *tetris);

// Checks the connection of the Left Angle
char LAConnect(GameInfo_t *tetris);

// Checks the connection of the Right Angle
char RAConnect(GameInfo_t *tetris);

// Checks the connection of the Left Zigzag
char LZConnect(GameInfo_t *tetris);

// Checks the connection of the Right Zigzag
char RZConnect(GameInfo_t *tetris);

// Checks the connection of the Square
char SqConnect(GameInfo_t *tetris);

// Checks the connection of the T
char TConnect(GameInfo_t *tetris);

// Checks if the spawn of the Line ends the game
char LiCheckEnd(GameInfo_t *tetris);

// Checks if the spawn of the Left Angle ends the game
char LACheckEnd(GameInfo_t *tetris);

// Checks if the spawn of the Right Angle ends the game
char RACheckEnd(GameInfo_t *tetris);

// Checks if the spawn of the Left Zigzag ends the game
char LZCheckEnd(GameInfo_t *tetris);

// Checks if the spawn of the Right Zigzag ends the game
char RZCheckEnd(GameInfo_t *tetris);

// Checks if the spawn of the Square ends the game
char SqCheckEnd(GameInfo_t *tetris);

// Checks if the spawn of the T ends the game
char TCheckEnd(GameInfo_t *tetris);

#endif  // TETRIS_LIB