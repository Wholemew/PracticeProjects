#include "tetris.h"

#include <ncurses.h>

int main() {
  GameInfo_t game = Init();
  unsigned short time = 0;
  char run = 1;
  int input = 0;
  UserAction_t act;
  initscr();
  keypad(stdscr, 1);
  noecho();
  cbreak();
  nodelay(stdscr, 1);

  while (run) {
    input = getch();
    act = parceInput(&input);
    game = updateCurrentState();
    if (game.pause == 2) {
      clear();
      StartingScreen(game);
      if (act == Terminate) run = 0;
      if (act == Start) StartGame();
    } else if (game.pause == 3) {
      clear();
      EndGameScreen(game);
      if (act == Terminate) run = 0;
      if (act == Start) StartGame();
    } else if (game.pause == 4) {
      run = 0;
    } else {
      userInput(act, 0);
      clear();
      game = updateCurrentState();
      UpdateGameScreen(game);
    }
    if (game.pause == 0) time++;
    if (time >= game.speed) {
      MoveDown();
      Connect();
      time = 0;
    }
    refresh();
    napms(50);  // 50 ms delay ~= 20 FPS
  }

  endwin();
  Destroy();

  return 0;
}

UserAction_t parceInput(int *input) {
  UserAction_t res = Up;
  switch (*input) {
    case KEY_LEFT:
      res = Left;
      break;

    case KEY_RIGHT:
      res = Right;
      break;

    case KEY_DOWN:
      res = Down;
      break;

    case 10:
      res = Start;
      break;

    case 'P':
      res = Pause;
      break;

    case 'p':
      res = Pause;
      break;

    case 'E':
      res = Terminate;
      break;

    case 'e':
      res = Terminate;
      break;

    case 32:
      res = Action;
      break;

    default:
      break;
  }
  return res;
}