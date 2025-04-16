#include "tetrisGUI.h"

void StartingScreen(GameInfo_t game) {
  mvprintw(0, 0, "_______________________");
  mvprintw(1, 0, "|                     |");
  mvprintw(2, 0, "|      Brick Game     |");
  mvprintw(3, 0, "|                     |");
  mvprintw(4, 0, "|        Tetris       |");
  mvprintw(5, 0, "|                     |");
  mvprintw(6, 0, "|     High Score:     |");
  mvprintw(7, 0, "|                     |");
  mvprintw(8, 0, "|       %10d    |", game.high_score);
  mvprintw(9, 0, "|                     |");
  mvprintw(10, 0, "|       Controls:     |");
  mvprintw(11, 0, "|                     |");
  mvprintw(12, 0, "|   Enter to start    |");
  mvprintw(13, 0, "| P to pause/unpause  |");
  mvprintw(14, 0, "|E to end game or exit|");
  mvprintw(15, 0, "| Use arrows to move  |");
  mvprintw(16, 0, "| Down arrow to fall  |");
  mvprintw(17, 0, "|   Space to rotate   |");
  mvprintw(18, 0, "|                     |");
  mvprintw(19, 0, "|                     |");
  mvprintw(20, 0, "|                     |");
  mvprintw(21, 0, "|_____________________|");
}

void UpdateGameScreen(GameInfo_t game) {
  char **next;
  next = (char **)malloc(4 * sizeof(char *));
  for (int i = 0; i < 4; ++i) {
    next[i] = (char *)malloc(4);
    for (int j = 0; j < 3; ++j) next[i][j] = 32;
    next[i][3] = 0;
  }
  DrawNext(game, next);
  mvprintw(0, 0, "_______________________");
  mvprintw(1, 0, "|%10s|          |", game.field[19]);
  mvprintw(2, 0, "|%10s|  Tetris  |", game.field[18]);
  mvprintw(3, 0, "|%10s|__________|", game.field[17]);
  mvprintw(4, 0, "|%10s|   Next   |", game.field[16]);
  mvprintw(5, 0, "|%10s|          |", game.field[15]);
  mvprintw(6, 0, "|%10s|    %3s   |", game.field[14], next[3]);
  mvprintw(7, 0, "|%10s|    %3s   |", game.field[13], next[2]);
  mvprintw(8, 0, "|%10s|    %3s   |", game.field[12], next[1]);
  mvprintw(9, 0, "|%10s|    %3s   |", game.field[11], next[0]);
  mvprintw(10, 0, "|%10s|          |", game.field[10]);
  mvprintw(11, 0, "|%10s|__________|", game.field[9]);
  mvprintw(12, 0, "|%10s|          |", game.field[8]);
  mvprintw(13, 0, "|%10s|   Score  |", game.field[7]);
  mvprintw(14, 0, "|%10s|          |", game.field[6]);
  mvprintw(15, 0, "|%10s|%10d|", game.field[5], game.score);
  mvprintw(16, 0, "|%10s|          |", game.field[4]);
  mvprintw(17, 0, "|%10s|High Score|", game.field[3]);
  mvprintw(18, 0, "|%10s|          |", game.field[2]);
  mvprintw(19, 0, "|%10s|%10d|", game.field[1], game.high_score);
  mvprintw(20, 0, "|%10s|          |", game.field[0]);
  mvprintw(21, 0, "|__________|__________|");
  for (int i = 0; i < 4; ++i) free(next[i]);
  free(next);
}

void EndGameScreen(GameInfo_t game) {
  mvprintw(0, 0, "_______________________");
  mvprintw(1, 0, "|%10s|          |", game.field[19]);
  mvprintw(2, 0, "|%10s|  Tetris  |", game.field[18]);
  mvprintw(3, 0, "|%10s|__________|", game.field[17]);
  mvprintw(4, 0, "|%10s|          |", game.field[16]);
  mvprintw(5, 0, "|%10s| Game Over|", game.field[15]);
  mvprintw(6, 0, "|%10s|          |", game.field[14]);
  mvprintw(7, 0, "|%10s|Restart   |", game.field[13]);
  mvprintw(8, 0, "|%10s|   Enter  |", game.field[12]);
  mvprintw(9, 0, "|%10s|   Quit   |", game.field[11]);
  mvprintw(10, 0, "|%10s|    E     |", game.field[10]);
  mvprintw(11, 0, "|%10s|__________|", game.field[9]);
  mvprintw(12, 0, "|%10s|          |", game.field[8]);
  mvprintw(13, 0, "|%10s|   Score  |", game.field[7]);
  mvprintw(14, 0, "|%10s|          |", game.field[6]);
  mvprintw(15, 0, "|%10s|%10d|", game.field[5], game.score);
  mvprintw(16, 0, "|%10s|          |", game.field[4]);
  mvprintw(17, 0, "|%10s|High Score|", game.field[3]);
  mvprintw(18, 0, "|%10s|          |", game.field[2]);
  mvprintw(19, 0, "|%10s|%10d|", game.field[1], game.high_score);
  mvprintw(20, 0, "|%10s|          |", game.field[0]);
  mvprintw(21, 0, "|__________|__________|");
}

void DrawNext(GameInfo_t game, char **dest) {
  if (game.next == Line)
    DrawLine(dest);
  else if (game.next == LeftAngle)
    DrawLA(dest);
  else if (game.next == RightAngle)
    DrawRA(dest);
  else if (game.next == LeftZ)
    DrawLZ(dest);
  else if (game.next == RightZ)
    DrawRZ(dest);
  else if (game.next == Square)
    DrawSquare(dest);
  else
    DrawT(dest);
}

void DrawLine(char **dest) {
  dest[3][0] = FIL;
  dest[2][0] = FIL;
  dest[1][0] = FIL;
  dest[0][0] = FIL;
}

void DrawLA(char **dest) {
  dest[3][0] = FIL;
  dest[2][0] = FIL;
  dest[2][1] = FIL;
  dest[2][2] = FIL;
}

void DrawRA(char **dest) {
  dest[2][0] = FIL;
  dest[2][1] = FIL;
  dest[2][2] = FIL;
  dest[3][2] = FIL;
}

void DrawLZ(char **dest) {
  dest[3][0] = FIL;
  dest[3][1] = FIL;
  dest[2][1] = FIL;
  dest[2][2] = FIL;
}

void DrawRZ(char **dest) {
  dest[2][0] = FIL;
  dest[2][1] = FIL;
  dest[3][1] = FIL;
  dest[3][2] = FIL;
}

void DrawSquare(char **dest) {
  dest[3][0] = FIL;
  dest[3][1] = FIL;
  dest[2][1] = FIL;
  dest[2][0] = FIL;
}

void DrawT(char **dest) {
  dest[2][0] = FIL;
  dest[2][1] = FIL;
  dest[2][2] = FIL;
  dest[3][1] = FIL;
}
