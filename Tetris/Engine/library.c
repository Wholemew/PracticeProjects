#include "library.h"

static GameInfo_t tetris;
static Falling_t piece;
static unsigned rnd = 0;

GameInfo_t Init() {
  tetris.level = 0;
  tetris.speed = 50;  // For 50 ms framerate
  tetris.field = (char **)malloc(HEI * sizeof(char *));
  if (tetris.field == NULL) {
    tetris.pause = 4;
    return tetris;
  } else {
    for (int i = 0; i < HEI; ++i) {
      tetris.field[i] = (char *)calloc(WID + 1, sizeof(char));
      if (tetris.field[i] == NULL) {
        tetris.pause = 4;
        return tetris;
      }
    }
  }
  FILE *sc;
  sc = fopen("Score.dat", "r");
  if (sc == NULL)
    tetris.high_score = 0;
  else
    fscanf(sc, "%u", &tetris.high_score);
  if (sc != NULL) fclose(sc);
  tetris.pause = 2;
  return tetris;
}

void Destroy() {
  if (tetris.field != NULL) {
    for (int i = 0; i < HEI; ++i) {
      if (tetris.field[i] != NULL) {
        free(tetris.field[i]);
      } else
        i = HEI;
    }
    free(tetris.field);
  }
}

GameInfo_t updateCurrentState() { return tetris; }

Falling_t getPiece() { return piece; }

void uploadNewState(GameInfo_t state, Falling_t fig) {
  tetris = state;
  piece = fig;
}

void userInput(UserAction_t action, char hold) {
  if (hold == 0) {
    if (action == Start && (tetris.pause == 2 || tetris.pause == 3))
      StartGame();
    else if (action == Pause && tetris.pause < 2)
      PauseResume();
    else if (action == Terminate && tetris.pause != 3)
      EndGame();
    else if (tetris.pause == 0) {
      if (action == Left)
        ShiftLeft();
      else if (action == Right)
        ShiftRight();
      else if (action == Down)
        Drop();
      else if (action == Action)
        Rotate();
    }
  }
}

void StartGame() {
  if (tetris.pause != 4) {
    for (int i = 0; i < WID; ++i)
      for (int j = 0; j < HEI; ++j) tetris.field[j][i] = EMP;
    GenerateNext();
    Spawn();
    GenerateNext();
    tetris.score = 0;
    tetris.pause = 0;
  }
}

void PauseResume() {
  if (tetris.pause == 0)
    tetris.pause = 1;
  else
    tetris.pause = 0;
}

void EndGame() {
  tetris.pause = 3;
  FILE *sc;
  sc = fopen("Score.dat", "w");
  fprintf(sc, "%u", tetris.high_score);
  fclose(sc);
  tetris.score = 0;
}

void ShiftLeft() {
  if (piece.type == Line)
    LiShiftLeft();
  else if (piece.type == LeftAngle)
    LAShiftLeft();
  else if (piece.type == RightAngle)
    RAShiftLeft();
  else if (piece.type == LeftZ)
    LZShiftLeft();
  else if (piece.type == RightZ)
    RZShiftLeft();
  else if (piece.type == Square)
    SqShiftLeft();
  else
    TShiftLeft();
  PutPiece();
  Connect();
}

void ShiftRight() {
  if (piece.type == Line)
    LiShiftRight();
  else if (piece.type == LeftAngle)
    LAShiftRight();
  else if (piece.type == RightAngle)
    RAShiftRight();
  else if (piece.type == LeftZ)
    LZShiftRight();
  else if (piece.type == RightZ)
    RZShiftRight();
  else if (piece.type == Square)
    SqShiftRight();
  else
    TShiftRight();
  PutPiece();
  Connect();
}

void Rotate() {
  if (piece.type == Line)
    LiRotate();
  else if (piece.type == LeftAngle)
    LARotate();
  else if (piece.type == RightAngle)
    RARotate();
  else if (piece.type == LeftZ)
    LZRotate();
  else if (piece.type == RightZ)
    RZRotate();
  else if (piece.type == T)
    TRotate();
  PutPiece();
  Connect();
}

void PutPiece() {
  if (piece.type == Line)
    PutLine();
  else if (piece.type == LeftAngle)
    PutLA();
  else if (piece.type == RightAngle)
    PutRA();
  else if (piece.type == LeftZ)
    PutLZ();
  else if (piece.type == RightZ)
    PutRZ();
  else if (piece.type == Square)
    PutSquare();
  else
    PutT();
}

void CleanPiece() {
  if (piece.type == Line)
    CleanLine();
  else if (piece.type == LeftAngle)
    CleanLA();
  else if (piece.type == RightAngle)
    CleanRA();
  else if (piece.type == LeftZ)
    CleanLZ();
  else if (piece.type == RightZ)
    CleanRZ();
  else if (piece.type == Square)
    CleanSquare();
  else
    CleanT();
}

void Drop() {
  while (Connect() != 1) MoveDown();
}

void MoveDown() {
  CleanPiece();
  MovePiece(0, -1);
  PutPiece();
}

void GenerateNext() {
  unsigned seed = time(0);
  if (MAXU - seed < rnd) seed = MAXU - seed;
  seed += rnd;
  rnd++;
  tetris.next = rand_r(&seed) % 7;
}

void Spawn() {
  piece.type = tetris.next;
  piece.posx = WID / 2;
  piece.posy = HEI - 1;
  piece.rotation = 0;
  if (CheckEnd() == 0) {
    PutPiece();
    Connect();
  } else
    EndGame();
}

char Connect() {
  char result;
  if (piece.type == Line)
    result = LiConnect();
  else if (piece.type == LeftAngle)
    result = LAConnect();
  else if (piece.type == RightAngle)
    result = RAConnect();
  else if (piece.type == LeftZ)
    result = LZConnect();
  else if (piece.type == RightZ)
    result = RZConnect();
  else if (piece.type == Square)
    result = SqConnect();
  else
    result = TConnect();
  if (result == 1) {
    CheckApplyScore();
    Spawn();
    GenerateNext();
  }
  return result;
}

char CheckEnd() {
  char result;
  if (piece.type == Line)
    result = LiCheckEnd();
  else if (piece.type == LeftAngle)
    result = LACheckEnd();
  else if (piece.type == RightAngle)
    result = RACheckEnd();
  else if (piece.type == LeftZ)
    result = LZCheckEnd();
  else if (piece.type == RightZ)
    result = RZCheckEnd();
  else if (piece.type == Square)
    result = SqCheckEnd();
  else
    result = TCheckEnd();
  return result;
}

void MovePiece(char x, char y) {
  piece.posx += x;
  piece.posy += y;
}

void CheckApplyScore() {
  char filling = 0, scored = 0, top = HEI;
  for (int y = 0; y < top; ++y) {
    for (int x = 0; x < WID; ++x) {
      if (tetris.field[y][x] == FIL) filling++;
    }
    if (filling == 10) {
      RemoveRow(y);
      scored++;
      y--;
      top--;
    }
    filling = 0;
  }
  if (scored == 1) tetris.score += 100;
  if (scored == 2) tetris.score += 300;
  if (scored == 3) tetris.score += 700;
  if (scored == 4) tetris.score += 1500;
  if (tetris.score > tetris.high_score) tetris.high_score = tetris.score;
  updateLevel();
}

void updateLevel() {
  tetris.level = tetris.score / 600;
  if (tetris.level > 9) tetris.level = 9;
  tetris.speed = 50 - 5 * tetris.level;
}

void RemoveRow(char r) {
  for (int y = r; y < HEI - 2; ++y) {
    for (int x = 0; x < WID; ++x) {
      tetris.field[y][x] = tetris.field[y + 1][x];
    }
  }
  for (int x = 0; x < WID; ++x) {
    tetris.field[HEI - 1][x] = EMP;
  }
}

void LiShiftLeft() {
  if (piece.posx != 0)
    if ((tetris.field[piece.posy][piece.posx - 1] == EMP) &&
        ((piece.rotation == 1) ||
         ((tetris.field[piece.posy - 1][piece.posx - 1] == EMP) &&
          (tetris.field[piece.posy - 2][piece.posx - 1] == EMP) &&
          (tetris.field[piece.posy - 3][piece.posx - 1] == EMP)))) {
      CleanLine();
      MovePiece(-1, 0);
    }
}

void LAShiftLeft() {
  if (piece.posx != 0)
    if ((((piece.rotation < 3) &&
          (tetris.field[piece.posy][piece.posx - 1] == EMP)) ||
         ((tetris.field[piece.posy][piece.posx] == EMP))) &&
        (((piece.rotation < 2) &&
          (tetris.field[piece.posy - 1][piece.posx - 1] == EMP)) ||
         (piece.rotation == 2) ||
         ((piece.rotation == 3) &&
          (tetris.field[piece.posy - 1][piece.posx] == EMP))) &&
        ((piece.rotation % 2 == 0) ||
         (tetris.field[piece.posy - 2][piece.posx - 1] == EMP))) {
      CleanLA();
      MovePiece(-1, 0);
    }
}

void RAShiftLeft() {
  if (piece.posx != 0)
    if ((((piece.rotation > 0) &&
          (tetris.field[piece.posy][piece.posx - 1] == EMP)) ||
         ((piece.rotation == 0) &&
          (tetris.field[piece.posy][piece.posx + 1] == EMP))) &&
        (((piece.rotation < 3) &&
          (tetris.field[piece.posy - 1][piece.posx - 1] == EMP)) ||
         (tetris.field[piece.posy - 1][piece.posx] == EMP)) &&
        ((piece.rotation % 2 == 0) ||
         ((piece.rotation == 1) &&
          (tetris.field[piece.posy - 2][piece.posx - 1] == EMP)) ||
         (tetris.field[piece.posy - 2][piece.posx] == EMP))) {
      CleanRA();
      MovePiece(-1, 0);
    }
}

void LZShiftLeft() {
  if (piece.posx != 0)
    if ((((piece.rotation == 0) &&
          (tetris.field[piece.posy][piece.posx - 1] == EMP)) ||
         (tetris.field[piece.posy][piece.posx] == EMP)) &&
        ((piece.rotation == 0) ||
         (tetris.field[piece.posy - 1][piece.posx - 1] == EMP)) &&
        ((piece.rotation == 0) ||
         (tetris.field[piece.posy - 2][piece.posx - 1] == EMP))) {
      CleanLZ();
      MovePiece(-1, 0);
    }
}

void RZShiftLeft() {
  if (piece.posx != 0)
    if ((((piece.rotation == 1) &&
          (tetris.field[piece.posy][piece.posx - 1] == EMP)) ||
         (tetris.field[piece.posy][piece.posx] == EMP)) &&
        (tetris.field[piece.posy - 1][piece.posx - 1] == EMP)) {
      CleanRZ();
      MovePiece(-1, 0);
    }
}

void SqShiftLeft() {
  if (piece.posx != 0)
    if ((tetris.field[piece.posy][piece.posx - 1] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx - 1] == EMP)) {
      CleanSquare();
      MovePiece(-1, 0);
    }
}

void TShiftLeft() {
  if (piece.posx != 0)
    if ((((piece.rotation > 1) &&
          (tetris.field[piece.posy][piece.posx - 1] == EMP)) ||
         (tetris.field[piece.posy][piece.posx] == EMP)) &&
        ((piece.rotation == 2) ||
         (tetris.field[piece.posy - 1][piece.posx - 1] == EMP)) &&
        ((piece.rotation % 2 == 0) ||
         ((piece.rotation == 1) ||
          (tetris.field[piece.posy - 2][piece.posx - 1] == EMP)))) {
      CleanT();
      MovePiece(-1, 0);
    }
}

void LiShiftRight() {
  CleanLine();
  if ((piece.rotation == 0) && (piece.posx != 9)) {
    if ((tetris.field[piece.posy][piece.posx + 1] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 1] == EMP) &&
        (tetris.field[piece.posy - 2][piece.posx + 1] == EMP) &&
        (tetris.field[piece.posy - 3][piece.posx + 1] == EMP))
      MovePiece(1, 0);
  } else if (piece.posx < 6)
    if (tetris.field[piece.posy][piece.posx + 4] == EMP) MovePiece(1, 0);
}

void LAShiftRight() {
  CleanLA();
  if ((piece.rotation % 2 == 0) && (piece.posx < 7)) {
    if ((((piece.rotation == 0) &&
          (tetris.field[piece.posy][piece.posx + 1] == EMP)) ||
         ((piece.rotation == 2) &&
          (tetris.field[piece.posy][piece.posx + 3] == EMP))) &&
        (tetris.field[piece.posy - 1][piece.posx + 3] == EMP))
      MovePiece(1, 0);
  } else if ((piece.rotation % 2 == 1) && (piece.posx < 8))
    if ((tetris.field[piece.posy][piece.posx + 2] == EMP) &&
        (((piece.rotation == 1) &&
          (tetris.field[piece.posy - 2][piece.posx + 1] == EMP)) ||
         ((piece.rotation == 3) &&
          ((tetris.field[piece.posy - 1][piece.posx + 2] == EMP) &&
           (tetris.field[piece.posy - 2][piece.posx + 2] == EMP)))))
      MovePiece(1, 0);
}

void RAShiftRight() {
  CleanRA();
  if ((piece.rotation % 2 == 0) && (piece.posx < 7)) {
    if ((tetris.field[piece.posy][piece.posx + 3] == EMP) &&
        (((piece.rotation == 0) &&
          (tetris.field[piece.posy - 1][piece.posx + 3] == EMP)) ||
         (piece.rotation == 2)))
      MovePiece(1, 0);
  } else if ((piece.rotation % 2 == 1) && (piece.posx < 8))
    if ((((piece.rotation == 1) &&
          ((tetris.field[piece.posy][piece.posx + 1] == EMP) &&
           (tetris.field[piece.posy - 1][piece.posx + 1] == EMP))) ||
         ((piece.rotation == 3) &&
          ((tetris.field[piece.posy][piece.posx + 2] == EMP) &&
           (tetris.field[piece.posy - 1][piece.posx + 2] == EMP)))) &&
        (tetris.field[piece.posy - 2][piece.posx + 2] == EMP))
      MovePiece(1, 0);
}

void LZShiftRight() {
  CleanLZ();
  if ((piece.rotation == 0) && (piece.posx < 7)) {
    if ((tetris.field[piece.posy][piece.posx + 2] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 3] == EMP))
      MovePiece(1, 0);
  } else if ((piece.rotation == 1) && (piece.posx < 8))
    if ((tetris.field[piece.posy][piece.posx + 2] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 2] == EMP))
      MovePiece(1, 0);
}

void RZShiftRight() {
  CleanRZ();
  if ((piece.rotation == 0) && (piece.posx < 7)) {
    if (tetris.field[piece.posy][piece.posx + 3] == EMP) MovePiece(1, 0);
  } else if ((piece.rotation == 1) && (piece.posx < 8))
    if ((tetris.field[piece.posy][piece.posx + 1] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 2] == EMP) &&
        (tetris.field[piece.posy - 2][piece.posx + 2] == EMP))
      MovePiece(1, 0);
}

void SqShiftRight() {
  CleanSquare();
  if (piece.posx < 8)
    if ((tetris.field[piece.posy][piece.posx + 2] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 2] == EMP)) {
      MovePiece(1, 0);
    }
}

void TShiftRight() {
  CleanT();
  if ((piece.rotation % 2 == 0) && (piece.posx < 7)) {
    if ((((piece.rotation == 0) &&
          (tetris.field[piece.posy][piece.posx + 2] == EMP)) ||
         ((piece.rotation == 2) &&
          (tetris.field[piece.posy][piece.posx + 3] == EMP))) &&
        (((piece.rotation == 0) &&
          (tetris.field[piece.posy - 1][piece.posx + 3] == EMP)) ||
         (piece.rotation == 2)))
      MovePiece(1, 0);
  } else if ((piece.rotation % 2 == 1) && (piece.posx < 8))
    if ((((piece.rotation == 1) &&
          (tetris.field[piece.posy][piece.posx + 2] == EMP) &&
          (tetris.field[piece.posy - 2][piece.posx + 2] == EMP)) ||
         ((piece.rotation == 3) &&
          (tetris.field[piece.posy][piece.posx + 1] == EMP))) &&
        (tetris.field[piece.posy - 1][piece.posx + 2] == EMP))
      MovePiece(1, 0);
}

void LiRotate() {
  CleanLine();
  if ((piece.rotation == 0) && (piece.posx <= 6)) {
    if ((tetris.field[piece.posy][piece.posx + 1] == EMP) &&
        (tetris.field[piece.posy][piece.posx + 2] == EMP) &&
        (tetris.field[piece.posy][piece.posx + 3] == EMP))
      piece.rotation = 1;
  } else if ((piece.rotation == 1) && (piece.posy >= 3))
    if ((tetris.field[piece.posy - 1][piece.posx] == EMP) &&
        (tetris.field[piece.posy - 2][piece.posx] == EMP) &&
        (tetris.field[piece.posy - 3][piece.posx] == EMP))
      piece.rotation = 0;
}

void LARotate() {
  CleanLA();
  if ((piece.rotation == 0) &&
      (tetris.field[piece.posy][piece.posx + 1] == EMP)) {
    piece.rotation = 1;
  } else if ((piece.rotation == 1) && (piece.posx <= 7)) {
    if ((tetris.field[piece.posy][piece.posx + 2] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 2] == EMP))
      piece.rotation = 2;
  } else if ((piece.rotation == 2) &&
             (tetris.field[piece.posy - 2][piece.posx] == EMP) &&
             (tetris.field[piece.posy - 2][piece.posx + 1] == EMP)) {
    piece.rotation = 3;
  } else if ((piece.rotation == 3) && (piece.posx <= 7)) {
    if ((tetris.field[piece.posy][piece.posx] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 2] == EMP))
      piece.rotation = 0;
  }
}

void RARotate() {
  CleanRA();
  if ((piece.rotation == 0) && (tetris.field[piece.posy][piece.posx] == EMP)) {
    piece.rotation = 1;
  } else if ((piece.rotation == 1) && (piece.posx <= 7)) {
    if ((tetris.field[piece.posy][piece.posx + 1] == EMP) &&
        (tetris.field[piece.posy][piece.posx + 2] == EMP))
      piece.rotation = 2;
  } else if ((piece.rotation == 2) &&
             (tetris.field[piece.posy - 2][piece.posx + 1] == EMP)) {
    piece.rotation = 3;
  } else if ((piece.rotation == 3) && (piece.posx <= 7)) {
    if ((tetris.field[piece.posy][piece.posx + 2] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 2] == EMP))
      piece.rotation = 0;
  }
}

void LZRotate() {
  CleanLZ();
  if ((piece.rotation == 0) &&
      (tetris.field[piece.posy - 2][piece.posx] == EMP)) {
    piece.rotation = 1;
  } else if ((piece.rotation == 1) && (piece.posx <= 7)) {
    if ((tetris.field[piece.posy][piece.posx] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 2] == EMP))
      piece.rotation = 0;
  }
}

void RZRotate() {
  CleanRZ();
  if ((piece.rotation == 0) && (tetris.field[piece.posy][piece.posx] == EMP)) {
    piece.rotation = 1;
  } else if ((piece.rotation == 1) && (piece.posx <= 7)) {
    if ((tetris.field[piece.posy][piece.posx + 1] == EMP) &&
        (tetris.field[piece.posy][piece.posx + 2] == EMP))
      piece.rotation = 0;
  }
}

void TRotate() {
  CleanT();
  if (piece.rotation == 0) {
    piece.rotation = 1;
  } else if ((piece.rotation == 1) && (piece.posx <= 7)) {
    if ((tetris.field[piece.posy][piece.posx] == EMP) &&
        (tetris.field[piece.posy][piece.posx + 2] == EMP))
      piece.rotation = 2;
  } else if ((piece.rotation == 2) &&
             (tetris.field[piece.posy - 2][piece.posx] == EMP)) {
    piece.rotation = 3;
  } else if ((piece.rotation == 3) && (piece.posx <= 7)) {
    if ((tetris.field[piece.posy][piece.posx + 1] == EMP) &&
        (tetris.field[piece.posy - 1][piece.posx + 2] == EMP))
      piece.rotation = 0;
  }
}

void CleanLine() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy - 2][piece.posx] = EMP;
    tetris.field[piece.posy - 3][piece.posx] = EMP;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy][piece.posx + 2] = EMP;
    tetris.field[piece.posy][piece.posx + 3] = EMP;
  }
}

void CleanLA() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 2] = EMP;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy - 2][piece.posx] = EMP;
  } else if (piece.rotation == 2) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy][piece.posx + 2] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 2] = EMP;
  } else if (piece.rotation == 3) {
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 2][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 2][piece.posx] = EMP;
  }
}

void CleanRA() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 2] = EMP;
    tetris.field[piece.posy][piece.posx + 2] = EMP;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy - 2][piece.posx] = EMP;
    tetris.field[piece.posy - 2][piece.posx + 1] = EMP;
  } else if (piece.rotation == 2) {
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy][piece.posx + 2] = EMP;
  } else if (piece.rotation == 3) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 2][piece.posx + 1] = EMP;
  }
}

void CleanLZ() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 2] = EMP;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy - 2][piece.posx] = EMP;
  }
}

void CleanRZ() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy][piece.posx + 2] = EMP;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 2][piece.posx + 1] = EMP;
  }
}

void CleanT() {
  tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
  if (piece.rotation == 0) {
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 1][piece.posx + 2] = EMP;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy - 2][piece.posx + 1] = EMP;
  } else if (piece.rotation == 2) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy][piece.posx + 1] = EMP;
    tetris.field[piece.posy][piece.posx + 2] = EMP;
  } else if (piece.rotation == 3) {
    tetris.field[piece.posy][piece.posx] = EMP;
    tetris.field[piece.posy - 1][piece.posx] = EMP;
    tetris.field[piece.posy - 2][piece.posx] = EMP;
  }
}

void CleanSquare() {
  tetris.field[piece.posy][piece.posx] = EMP;
  tetris.field[piece.posy - 1][piece.posx] = EMP;
  tetris.field[piece.posy][piece.posx + 1] = EMP;
  tetris.field[piece.posy - 1][piece.posx + 1] = EMP;
}

void PutLine() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy - 2][piece.posx] = FIL;
    tetris.field[piece.posy - 3][piece.posx] = FIL;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy][piece.posx + 2] = FIL;
    tetris.field[piece.posy][piece.posx + 3] = FIL;
  }
}

void PutLA() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 2] = FIL;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy - 2][piece.posx] = FIL;
  } else if (piece.rotation == 2) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy][piece.posx + 2] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 2] = FIL;
  } else if (piece.rotation == 3) {
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 2][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 2][piece.posx] = FIL;
  }
}

void PutRA() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 2] = FIL;
    tetris.field[piece.posy][piece.posx + 2] = FIL;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy - 2][piece.posx] = FIL;
    tetris.field[piece.posy - 2][piece.posx + 1] = FIL;
  } else if (piece.rotation == 2) {
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy][piece.posx + 2] = FIL;
  } else if (piece.rotation == 3) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 2][piece.posx + 1] = FIL;
  }
}

void PutLZ() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 2] = FIL;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy - 2][piece.posx] = FIL;
  }
}

void PutRZ() {
  if (piece.rotation == 0) {
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy][piece.posx + 2] = FIL;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 2][piece.posx + 1] = FIL;
  }
}

void PutT() {
  tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
  if (piece.rotation == 0) {
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 1][piece.posx + 2] = FIL;
  } else if (piece.rotation == 1) {
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy - 2][piece.posx + 1] = FIL;
  } else if (piece.rotation == 2) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy][piece.posx + 1] = FIL;
    tetris.field[piece.posy][piece.posx + 2] = FIL;
  } else if (piece.rotation == 3) {
    tetris.field[piece.posy][piece.posx] = FIL;
    tetris.field[piece.posy - 1][piece.posx] = FIL;
    tetris.field[piece.posy - 2][piece.posx] = FIL;
  }
}

void PutSquare() {
  tetris.field[piece.posy][piece.posx] = FIL;
  tetris.field[piece.posy - 1][piece.posx] = FIL;
  tetris.field[piece.posy][piece.posx + 1] = FIL;
  tetris.field[piece.posy - 1][piece.posx + 1] = FIL;
}

char LiConnect() {
  char result = 0;
  if (((piece.rotation == 0) && (piece.posy == 3)) ||
      ((piece.rotation == 1) && (piece.posy == 0)))
    result = 1;
  else if (piece.rotation == 1) {
    if (((tetris.field[piece.posy - 1][piece.posx] == FIL)) ||
        (tetris.field[piece.posy - 1][piece.posx + 1] == FIL) ||
        (tetris.field[piece.posy - 1][piece.posx + 2] == FIL) ||
        (tetris.field[piece.posy - 1][piece.posx + 3] == FIL))
      result = 1;
  } else if ((piece.rotation == 0) &&
             (tetris.field[piece.posy - 4][piece.posx] == FIL))
    result = 1;
  return result;
}

char LAConnect() {
  char result = 0;
  if (((piece.rotation % 2 == 0) && (piece.posy == 1)) ||
      ((piece.rotation % 2 == 1) && (piece.posy == 2)))
    result = 1;
  else if (piece.rotation == 0) {
    if ((tetris.field[piece.posy - 2][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 1] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 2] == FIL))
      result = 1;
  } else if (piece.rotation == 1) {
    if ((tetris.field[piece.posy - 3][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 1][piece.posx + 1] == FIL))
      result = 1;
  } else if (piece.rotation == 2) {
    if ((tetris.field[piece.posy - 1][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 1][piece.posx + 1] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 2] == FIL))
      result = 1;
  } else if (piece.rotation == 3)
    if ((tetris.field[piece.posy - 3][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 3][piece.posx + 1] == FIL))
      result = 1;
  return result;
}

char RAConnect() {
  char result = 0;
  if (((piece.rotation % 2 == 0) && (piece.posy == 1)) ||
      ((piece.rotation % 2 == 1) && (piece.posy == 2)))
    result = 1;
  else if (piece.rotation == 0) {
    if ((tetris.field[piece.posy - 2][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 1] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 2] == FIL))
      result = 1;
  } else if (piece.rotation == 1) {
    if ((tetris.field[piece.posy - 3][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 3][piece.posx + 1] == FIL))
      result = 1;
  } else if (piece.rotation == 2) {
    if ((tetris.field[piece.posy - 2][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 1][piece.posx + 1] == FIL) ||
        (tetris.field[piece.posy - 1][piece.posx + 2] == FIL))
      result = 1;
  } else if (piece.rotation == 3)
    if ((tetris.field[piece.posy - 1][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 3][piece.posx + 1] == FIL))
      result = 1;

  return result;
}

char LZConnect() {
  char result = 0;
  if (((piece.rotation == 0) && (piece.posy == 1)) ||
      ((piece.rotation == 1) && (piece.posy == 2)))
    result = 1;
  else if (piece.rotation == 0) {
    if ((tetris.field[piece.posy - 1][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 1] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 2] == FIL))
      result = 1;
  } else if (piece.rotation == 1)
    if ((tetris.field[piece.posy - 3][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 1] == FIL))
      result = 1;

  return result;
}

char RZConnect() {
  char result = 0;
  if (((piece.rotation == 0) && (piece.posy == 1)) ||
      ((piece.rotation == 1) && (piece.posy == 2)))
    result = 1;
  else if (piece.rotation == 0) {
    if ((tetris.field[piece.posy - 2][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 1] == FIL) ||
        (tetris.field[piece.posy - 1][piece.posx + 2] == FIL))
      result = 1;
  } else if (piece.rotation == 1)
    if ((tetris.field[piece.posy - 2][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 3][piece.posx + 1] == FIL))
      result = 1;

  return result;
}

char SqConnect() {
  char result = 0;
  if (piece.posy == 1)
    result = 1;
  else if ((tetris.field[piece.posy - 2][piece.posx] == FIL) ||
           (tetris.field[piece.posy - 2][piece.posx + 1] == FIL))
    result = 1;
  return result;
}

char TConnect() {
  char result = 0;
  if (((piece.rotation % 2 == 0) && (piece.posy == 1)) ||
      ((piece.rotation % 2 == 1) && (piece.posy == 2)))
    result = 1;
  else if (piece.rotation == 0) {
    if ((tetris.field[piece.posy - 2][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 1] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 2] == FIL))
      result = 1;
  } else if (piece.rotation == 1) {
    if ((tetris.field[piece.posy - 2][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 3][piece.posx + 1] == FIL))
      result = 1;
  } else if (piece.rotation == 2) {
    if ((tetris.field[piece.posy - 1][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 1] == FIL) ||
        (tetris.field[piece.posy - 1][piece.posx + 2] == FIL))
      result = 1;
  } else if (piece.rotation == 3)
    if ((tetris.field[piece.posy - 3][piece.posx] == FIL) ||
        (tetris.field[piece.posy - 2][piece.posx + 1] == FIL))
      result = 1;

  return result;
}

char LiCheckEnd() {
  char result = 0;
  if (((tetris.field[piece.posy][piece.posx] == FIL)) ||
      (tetris.field[piece.posy - 1][piece.posx] == FIL) ||
      (tetris.field[piece.posy - 2][piece.posx] == FIL) ||
      (tetris.field[piece.posy - 3][piece.posx] == FIL))
    result = 1;
  return result;
}

char LACheckEnd() {
  char result = 0;
  if ((tetris.field[piece.posy][piece.posx] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx + 2] == FIL))
    result = 1;
  return result;
}

char RACheckEnd() {
  char result = 0;
  if ((tetris.field[piece.posy - 1][piece.posx] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx + 2] == FIL) ||
      (tetris.field[piece.posy][piece.posx + 2] == FIL))
    result = 1;
  return result;
}

char LZCheckEnd() {
  char result = 0;
  if (((tetris.field[piece.posy][piece.posx] == FIL)) ||
      (tetris.field[piece.posy][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx + 2] == FIL))
    result = 1;
  return result;
}

char RZCheckEnd() {
  char result = 0;
  if (((tetris.field[piece.posy - 1][piece.posx] == FIL)) ||
      (tetris.field[piece.posy - 1][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy][piece.posx + 2] == FIL))
    result = 1;
  return result;
}

char SqCheckEnd() {
  char result = 0;
  if (((tetris.field[piece.posy][piece.posx] == FIL)) ||
      (tetris.field[piece.posy][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx] == FIL))
    result = 1;
  return result;
}

char TCheckEnd() {
  char result = 0;
  if (((tetris.field[piece.posy - 1][piece.posx] == FIL)) ||
      (tetris.field[piece.posy][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx + 1] == FIL) ||
      (tetris.field[piece.posy - 1][piece.posx + 2] == FIL))
    result = 1;
  return result;
}
