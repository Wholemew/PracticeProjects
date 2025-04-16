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
  char **field;
  Figure_t next;
  unsigned score;
  unsigned high_score;
  unsigned short speed;
  char level;
  char pause;  // 0 for play, 1 for pause, 2 for starting screen, 3 for game end
               // screen, 4 if an error occured
} GameInfo_t;

GameInfo_t Init();  // Initiates field and sets up the game at starting screen

// Frees memory from static structure in library.c
void Destroy();

void StartGame();  // Starts new game

// Reads user input and deals with consequences
void userInput(UserAction_t action, char hold);

// Returns static game info structure from library.c as is for gui
GameInfo_t updateCurrentState();

// Returns static falling structure from library.c as is for testing
Falling_t getPiece();

// Function for testing. Uploads new state into internal static variable tetris
// and new falling figure fig into internal static variable piece.
void uploadNewState(GameInfo_t state, Falling_t fig);

// Pause or unpause in game
void PauseResume();

// Ends game and updates high score
void EndGame();

// Adjusts function to shift the piece to the left
void ShiftLeft();

// Adjusts function to shift the piece to the right
void ShiftRight();

// Adjusts function to rotate the piece
void Rotate();

// Adjusts function to draw the piece into the field
void PutPiece();

// Adjusts function to erase the piece from the field
void CleanPiece();

// Drops the piece down until it connects
void Drop();

// Moves the piece one step down
void MoveDown();

// Generates the type of the next piece
void GenerateNext();

// Spawns next piece in the field and generates new next
void Spawn();

// Adjusts function to check the connection of the piece
char Connect();

// Adjusts function to check if the game has ended
char CheckEnd();

// Moves piece by a given shift in x and y
void MovePiece(char x, char y);

// Checks and removes the filled rows and updates score
void CheckApplyScore();

// Updates level and speed
void updateLevel();

// Removes row on r coordinate and shifts upper rows one position down
void RemoveRow(char r);

// Shifts Line one position Left
void LiShiftLeft();

// Shifts Left Angle one position Left
void LAShiftLeft();

// Shifts Right Angle one position Left
void RAShiftLeft();

// Shifts Left Zigzag one position Left
void LZShiftLeft();

// Shifts Right Zigzag one position Left
void RZShiftLeft();

// Shifts Square one position Left
void SqShiftLeft();

// Shifts T one position Left
void TShiftLeft();

// Shifts Line one position Right
void LiShiftRight();

// Shifts Left Angle one position Right
void LAShiftRight();

// Shifts Right Angle one position Right
void RAShiftRight();

// Shifts Left Zigzag one position Right
void LZShiftRight();

// Shifts Right Zigzag one position Right
void RZShiftRight();

// Shifts Square one position Right
void SqShiftRight();

// Shifts T one position Right
void TShiftRight();

// Rotates Line
void LiRotate();

// Rotates Left Angle
void LARotate();

// Rotates Right Angle
void RARotate();

// Rotates Left Zigzag
void LZRotate();

// Rotates Right Zigzag
void RZRotate();

// Rotates T
void TRotate();

// Erases line on piece position from the field
void CleanLine();

// Erases left angle on piece position from the field
void CleanLA();

// Erases right angle on piece position from the field
void CleanRA();

// Erases left zigzag on piece position from the field
void CleanLZ();

// Erases right zigzag on piece position from the field
void CleanRZ();

// Erases square on piece position from the field
void CleanSquare();

// Erases T on piece position from the field
void CleanT();

// Draws line on piece position in the field
void PutLine();

// Draws left angle on piece position in the field
void PutLA();

// Draws right angle on piece position in the field
void PutRA();

// Draws left zigzag on piece position in the field
void PutLZ();

// Draws right zigzag on piece position in the field
void PutRZ();

// Draws square on piece position in the field
void PutSquare();

// Draws T on piece position in the field
void PutT();

// Checks the connection of the Line
char LiConnect();

// Checks the connection of the Left Angle
char LAConnect();

// Checks the connection of the Right Angle
char RAConnect();

// Checks the connection of the Left Zigzag
char LZConnect();

// Checks the connection of the Right Zigzag
char RZConnect();

// Checks the connection of the Square
char SqConnect();

// Checks the connection of the T
char TConnect();

// Checks if the spawn of the Line ends the game
char LiCheckEnd();

// Checks if the spawn of the Left Angle ends the game
char LACheckEnd();

// Checks if the spawn of the Right Angle ends the game
char RACheckEnd();

// Checks if the spawn of the Left Zigzag ends the game
char LZCheckEnd();

// Checks if the spawn of the Right Zigzag ends the game
char RZCheckEnd();

// Checks if the spawn of the Square ends the game
char SqCheckEnd();

// Checks if the spawn of the T ends the game
char TCheckEnd();

#endif  // TETRIS_LIB