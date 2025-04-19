#include "library.h"

GameInfo_t *Init(GameInfo_t *tetris) {
  tetris = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  tetris->level = 0;
  tetris->speed = 50;  // For 50 ms framerate
  tetris->field = (char **)malloc(HEI * sizeof(char *));
  if (tetris->field == NULL) {
    tetris->pause = 4;
    return tetris;
  } else {
    for (int i = 0; i < HEI; ++i) {
      tetris->field[i] = (char *)calloc(WID + 1, sizeof(char));
      if (tetris->field[i] == NULL) {
        tetris->pause = 4;
        return tetris;
      }
    }
  }
  FILE *sc;
  sc = fopen("Score.dat", "r");
  if (sc == NULL)
    tetris->high_score = 0;
  else
    fscanf(sc, "%u", &(tetris->high_score));
  if (sc != NULL) fclose(sc);
  tetris->pause = 2;
  return tetris;
}

void Destroy(GameInfo_t *tetris) {
  if (tetris->field != NULL) {
    for (int i = 0; i < HEI; ++i) {
      if (tetris->field[i] != NULL) {
        free(tetris->field[i]);
      } else
        i = HEI;
    }
    free(tetris->field);
  }
  free(tetris);
}

GameInfo_t *updateCurrentState() {
  static GameInfo_t *tetris = NULL;
  if (tetris == NULL) tetris = Init(tetris);
  return tetris;
}

void userInput(UserAction_t action, char hold) {
  GameInfo_t *tetris = updateCurrentState();
  if (hold == 0) {
    if (action == Start && (tetris->pause == 2 || tetris->pause == 3))
      StartGame(tetris);
    else if (action == Pause && tetris->pause < 2)
      PauseResume(tetris);
    else if (action == Terminate && tetris->pause != 3)
      EndGame(tetris);
    else if (tetris->pause == 0) {
      if (action == Left)
        ShiftLeft(tetris);
      else if (action == Right)
        ShiftRight(tetris);
      else if (action == Down)
        Drop(tetris);
      else if (action == Action)
        Rotate(tetris);
    }
  }
}

void StartGame(GameInfo_t *tetris) {
  if (tetris->pause != 4) {
    for (int i = 0; i < WID; ++i)
      for (int j = 0; j < HEI; ++j) tetris->field[j][i] = EMP;
    GenerateNext(tetris);
    Spawn(tetris);
    GenerateNext(tetris);
    tetris->score = 0;
    tetris->pause = 0;
  }
}

void PauseResume(GameInfo_t *tetris) {
  if (tetris->pause == 0)
    tetris->pause = 1;
  else
    tetris->pause = 0;
}

void EndGame(GameInfo_t *tetris) {
  tetris->pause = 3;
  FILE *sc;
  sc = fopen("Score.dat", "w");
  fprintf(sc, "%u", tetris->high_score);
  fclose(sc);
  tetris->score = 0;
}

void ShiftLeft(GameInfo_t *tetris) {
  if (tetris->piece.type == Line)
    LiShiftLeft(tetris);
  else if (tetris->piece.type == LeftAngle)
    LAShiftLeft(tetris);
  else if (tetris->piece.type == RightAngle)
    RAShiftLeft(tetris);
  else if (tetris->piece.type == LeftZ)
    LZShiftLeft(tetris);
  else if (tetris->piece.type == RightZ)
    RZShiftLeft(tetris);
  else if (tetris->piece.type == Square)
    SqShiftLeft(tetris);
  else
    TShiftLeft(tetris);
  PutPiece(tetris);
  Connect(tetris);
}

void ShiftRight(GameInfo_t *tetris) {
  if (tetris->piece.type == Line)
    LiShiftRight(tetris);
  else if (tetris->piece.type == LeftAngle)
    LAShiftRight(tetris);
  else if (tetris->piece.type == RightAngle)
    RAShiftRight(tetris);
  else if (tetris->piece.type == LeftZ)
    LZShiftRight(tetris);
  else if (tetris->piece.type == RightZ)
    RZShiftRight(tetris);
  else if (tetris->piece.type == Square)
    SqShiftRight(tetris);
  else
    TShiftRight(tetris);
  PutPiece(tetris);
  Connect(tetris);
}

void Rotate(GameInfo_t *tetris) {
  if (tetris->piece.type == Line)
    LiRotate(tetris);
  else if (tetris->piece.type == LeftAngle)
    LARotate(tetris);
  else if (tetris->piece.type == RightAngle)
    RARotate(tetris);
  else if (tetris->piece.type == LeftZ)
    LZRotate(tetris);
  else if (tetris->piece.type == RightZ)
    RZRotate(tetris);
  else if (tetris->piece.type == T)
    TRotate(tetris);
  PutPiece(tetris);
  Connect(tetris);
}

void PutPiece(GameInfo_t *tetris) {
  if (tetris->piece.type == Line)
    PutLine(tetris);
  else if (tetris->piece.type == LeftAngle)
    PutLA(tetris);
  else if (tetris->piece.type == RightAngle)
    PutRA(tetris);
  else if (tetris->piece.type == LeftZ)
    PutLZ(tetris);
  else if (tetris->piece.type == RightZ)
    PutRZ(tetris);
  else if (tetris->piece.type == Square)
    PutSquare(tetris);
  else
    PutT(tetris);
}

void CleanPiece(GameInfo_t *tetris) {
  if (tetris->piece.type == Line)
    CleanLine(tetris);
  else if (tetris->piece.type == LeftAngle)
    CleanLA(tetris);
  else if (tetris->piece.type == RightAngle)
    CleanRA(tetris);
  else if (tetris->piece.type == LeftZ)
    CleanLZ(tetris);
  else if (tetris->piece.type == RightZ)
    CleanRZ(tetris);
  else if (tetris->piece.type == Square)
    CleanSquare(tetris);
  else
    CleanT(tetris);
}

void Drop(GameInfo_t *tetris) {
  while (Connect(tetris) != 1) MoveDown(tetris);
}

void MoveDown(GameInfo_t *tetris) {
  CleanPiece(tetris);
  MovePiece(0, -1, tetris);
  PutPiece(tetris);
}

void GenerateNext(GameInfo_t *tetris) {
  unsigned seed = time(0);
  static unsigned rnd = 0;
  if (MAXU - seed < rnd) seed = MAXU - seed;
  seed += rnd;
  rnd++;
  tetris->next = rand_r(&seed) % 7;
}

void Spawn(GameInfo_t *tetris) {
  tetris->piece.type = tetris->next;
  tetris->piece.posx = WID / 2;
  tetris->piece.posy = HEI - 1;
  tetris->piece.rotation = 0;
  if (CheckEnd(tetris) == 0) {
    PutPiece(tetris);
    Connect(tetris);
  } else
    EndGame(tetris);
}

char Connect(GameInfo_t *tetris) {
  char result;
  if (tetris->piece.type == Line)
    result = LiConnect(tetris);
  else if (tetris->piece.type == LeftAngle)
    result = LAConnect(tetris);
  else if (tetris->piece.type == RightAngle)
    result = RAConnect(tetris);
  else if (tetris->piece.type == LeftZ)
    result = LZConnect(tetris);
  else if (tetris->piece.type == RightZ)
    result = RZConnect(tetris);
  else if (tetris->piece.type == Square)
    result = SqConnect(tetris);
  else
    result = TConnect(tetris);
  if (result == 1) {
    CheckApplyScore(tetris);
    Spawn(tetris);
    GenerateNext(tetris);
  }
  return result;
}

char CheckEnd(GameInfo_t *tetris) {
  char result;
  if (tetris->piece.type == Line)
    result = LiCheckEnd(tetris);
  else if (tetris->piece.type == LeftAngle)
    result = LACheckEnd(tetris);
  else if (tetris->piece.type == RightAngle)
    result = RACheckEnd(tetris);
  else if (tetris->piece.type == LeftZ)
    result = LZCheckEnd(tetris);
  else if (tetris->piece.type == RightZ)
    result = RZCheckEnd(tetris);
  else if (tetris->piece.type == Square)
    result = SqCheckEnd(tetris);
  else
    result = TCheckEnd(tetris);
  return result;
}

void MovePiece(char x, char y, GameInfo_t *tetris) {
  tetris->piece.posx += x;
  tetris->piece.posy += y;
}

void CheckApplyScore(GameInfo_t *tetris) {
  char filling = 0, scored = 0, top = HEI;
  for (int y = 0; y < top; ++y) {
    for (int x = 0; x < WID; ++x) {
      if (tetris->field[y][x] == FIL) filling++;
    }
    if (filling == 10) {
      RemoveRow(y, tetris);
      scored++;
      y--;
      top--;
    }
    filling = 0;
  }
  if (scored == 1) tetris->score += 100;
  if (scored == 2) tetris->score += 300;
  if (scored == 3) tetris->score += 700;
  if (scored == 4) tetris->score += 1500;
  if (tetris->score > tetris->high_score) tetris->high_score = tetris->score;
  updateLevel(tetris);
}

void updateLevel(GameInfo_t *tetris) {
  tetris->level = tetris->score / 600;
  if (tetris->level > 9) tetris->level = 9;
  tetris->speed = 50 - 5 * tetris->level;
}

void RemoveRow(char r, GameInfo_t *tetris) {
  for (int y = r; y < HEI - 2; ++y) {
    for (int x = 0; x < WID; ++x) {
      tetris->field[y][x] = tetris->field[y + 1][x];
    }
  }
  for (int x = 0; x < WID; ++x) {
    tetris->field[HEI - 1][x] = EMP;
  }
}

void LiShiftLeft(GameInfo_t *tetris) {
  if (tetris->piece.posx != 0)
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx - 1] == EMP) &&
        ((tetris->piece.rotation == 1) ||
         ((tetris->field[tetris->piece.posy - 1][tetris->piece.posx - 1] ==
           EMP) &&
          (tetris->field[tetris->piece.posy - 2][tetris->piece.posx - 1] ==
           EMP) &&
          (tetris->field[tetris->piece.posy - 3][tetris->piece.posx - 1] ==
           EMP)))) {
      CleanLine(tetris);
      MovePiece(-1, 0, tetris);
    }
}

void LAShiftLeft(GameInfo_t *tetris) {
  if (tetris->piece.posx != 0)
    if ((((tetris->piece.rotation < 3) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx - 1] == EMP)) ||
         ((tetris->field[tetris->piece.posy][tetris->piece.posx] == EMP))) &&
        (((tetris->piece.rotation < 2) &&
          (tetris->field[tetris->piece.posy - 1][tetris->piece.posx - 1] ==
           EMP)) ||
         (tetris->piece.rotation == 2) ||
         ((tetris->piece.rotation == 3) &&
          (tetris->field[tetris->piece.posy - 1][tetris->piece.posx] ==
           EMP))) &&
        ((tetris->piece.rotation % 2 == 0) ||
         (tetris->field[tetris->piece.posy - 2][tetris->piece.posx - 1] ==
          EMP))) {
      CleanLA(tetris);
      MovePiece(-1, 0, tetris);
    }
}

void RAShiftLeft(GameInfo_t *tetris) {
  if (tetris->piece.posx != 0)
    if ((((tetris->piece.rotation > 0) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx - 1] == EMP)) ||
         ((tetris->piece.rotation == 0) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx + 1] ==
           EMP))) &&
        (((tetris->piece.rotation < 3) &&
          (tetris->field[tetris->piece.posy - 1][tetris->piece.posx - 1] ==
           EMP)) ||
         (tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == EMP)) &&
        ((tetris->piece.rotation % 2 == 0) ||
         ((tetris->piece.rotation == 1) &&
          (tetris->field[tetris->piece.posy - 2][tetris->piece.posx - 1] ==
           EMP)) ||
         (tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == EMP))) {
      CleanRA(tetris);
      MovePiece(-1, 0, tetris);
    }
}

void LZShiftLeft(GameInfo_t *tetris) {
  if (tetris->piece.posx != 0)
    if ((((tetris->piece.rotation == 0) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx - 1] == EMP)) ||
         (tetris->field[tetris->piece.posy][tetris->piece.posx] == EMP)) &&
        ((tetris->piece.rotation == 0) ||
         (tetris->field[tetris->piece.posy - 1][tetris->piece.posx - 1] ==
          EMP)) &&
        ((tetris->piece.rotation == 0) ||
         (tetris->field[tetris->piece.posy - 2][tetris->piece.posx - 1] ==
          EMP))) {
      CleanLZ(tetris);
      MovePiece(-1, 0, tetris);
    }
}

void RZShiftLeft(GameInfo_t *tetris) {
  if (tetris->piece.posx != 0)
    if ((((tetris->piece.rotation == 1) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx - 1] == EMP)) ||
         (tetris->field[tetris->piece.posy][tetris->piece.posx] == EMP)) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx - 1] ==
         EMP)) {
      CleanRZ(tetris);
      MovePiece(-1, 0, tetris);
    }
}

void SqShiftLeft(GameInfo_t *tetris) {
  if (tetris->piece.posx != 0)
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx - 1] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx - 1] ==
         EMP)) {
      CleanSquare(tetris);
      MovePiece(-1, 0, tetris);
    }
}

void TShiftLeft(GameInfo_t *tetris) {
  if (tetris->piece.posx != 0)
    if ((((tetris->piece.rotation > 1) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx - 1] == EMP)) ||
         (tetris->field[tetris->piece.posy][tetris->piece.posx] == EMP)) &&
        ((tetris->piece.rotation == 2) ||
         (tetris->field[tetris->piece.posy - 1][tetris->piece.posx - 1] ==
          EMP)) &&
        ((tetris->piece.rotation % 2 == 0) ||
         ((tetris->piece.rotation == 1) ||
          (tetris->field[tetris->piece.posy - 2][tetris->piece.posx - 1] ==
           EMP)))) {
      CleanT(tetris);
      MovePiece(-1, 0, tetris);
    }
}

void LiShiftRight(GameInfo_t *tetris) {
  CleanLine(tetris);
  if ((tetris->piece.rotation == 0) && (tetris->piece.posx != 9)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] ==
         EMP) &&
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
         EMP) &&
        (tetris->field[tetris->piece.posy - 3][tetris->piece.posx + 1] == EMP))
      MovePiece(1, 0, tetris);
  } else if (tetris->piece.posx < 6)
    if (tetris->field[tetris->piece.posy][tetris->piece.posx + 4] == EMP)
      MovePiece(1, 0, tetris);
}

void LAShiftRight(GameInfo_t *tetris) {
  CleanLA(tetris);
  if ((tetris->piece.rotation % 2 == 0) && (tetris->piece.posx < 7)) {
    if ((((tetris->piece.rotation == 0) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == EMP)) ||
         ((tetris->piece.rotation == 2) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx + 3] ==
           EMP))) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 3] == EMP))
      MovePiece(1, 0, tetris);
  } else if ((tetris->piece.rotation % 2 == 1) && (tetris->piece.posx < 8))
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP) &&
        (((tetris->piece.rotation == 1) &&
          (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
           EMP)) ||
         ((tetris->piece.rotation == 3) &&
          ((tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] ==
            EMP) &&
           (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 2] ==
            EMP)))))
      MovePiece(1, 0, tetris);
}

void RAShiftRight(GameInfo_t *tetris) {
  CleanRA(tetris);
  if ((tetris->piece.rotation % 2 == 0) && (tetris->piece.posx < 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 3] == EMP) &&
        (((tetris->piece.rotation == 0) &&
          (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 3] ==
           EMP)) ||
         (tetris->piece.rotation == 2)))
      MovePiece(1, 0, tetris);
  } else if ((tetris->piece.rotation % 2 == 1) && (tetris->piece.posx < 8))
    if ((((tetris->piece.rotation == 1) &&
          ((tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == EMP) &&
           (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] ==
            EMP))) ||
         ((tetris->piece.rotation == 3) &&
          ((tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP) &&
           (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] ==
            EMP)))) &&
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 2] == EMP))
      MovePiece(1, 0, tetris);
}

void LZShiftRight(GameInfo_t *tetris) {
  CleanLZ(tetris);
  if ((tetris->piece.rotation == 0) && (tetris->piece.posx < 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 3] == EMP))
      MovePiece(1, 0, tetris);
  } else if ((tetris->piece.rotation == 1) && (tetris->piece.posx < 8))
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == EMP))
      MovePiece(1, 0, tetris);
}

void RZShiftRight(GameInfo_t *tetris) {
  CleanRZ(tetris);
  if ((tetris->piece.rotation == 0) && (tetris->piece.posx < 7)) {
    if (tetris->field[tetris->piece.posy][tetris->piece.posx + 3] == EMP)
      MovePiece(1, 0, tetris);
  } else if ((tetris->piece.rotation == 1) && (tetris->piece.posx < 8))
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] ==
         EMP) &&
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 2] == EMP))
      MovePiece(1, 0, tetris);
}

void SqShiftRight(GameInfo_t *tetris) {
  CleanSquare(tetris);
  if (tetris->piece.posx < 8)
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] ==
         EMP)) {
      MovePiece(1, 0, tetris);
    }
}

void TShiftRight(GameInfo_t *tetris) {
  CleanT(tetris);
  if ((tetris->piece.rotation % 2 == 0) && (tetris->piece.posx < 7)) {
    if ((((tetris->piece.rotation == 0) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP)) ||
         ((tetris->piece.rotation == 2) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx + 3] ==
           EMP))) &&
        (((tetris->piece.rotation == 0) &&
          (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 3] ==
           EMP)) ||
         (tetris->piece.rotation == 2)))
      MovePiece(1, 0, tetris);
  } else if ((tetris->piece.rotation % 2 == 1) && (tetris->piece.posx < 8))
    if ((((tetris->piece.rotation == 1) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP) &&
          (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 2] ==
           EMP)) ||
         ((tetris->piece.rotation == 3) &&
          (tetris->field[tetris->piece.posy][tetris->piece.posx + 1] ==
           EMP))) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == EMP))
      MovePiece(1, 0, tetris);
}

void LiRotate(GameInfo_t *tetris) {
  CleanLine(tetris);
  if ((tetris->piece.rotation == 0) && (tetris->piece.posx <= 6)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == EMP) &&
        (tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP) &&
        (tetris->field[tetris->piece.posy][tetris->piece.posx + 3] == EMP))
      tetris->piece.rotation = 1;
  } else if ((tetris->piece.rotation == 1) && (tetris->piece.posy >= 3))
    if ((tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == EMP) &&
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == EMP) &&
        (tetris->field[tetris->piece.posy - 3][tetris->piece.posx] == EMP))
      tetris->piece.rotation = 0;
}

void LARotate(GameInfo_t *tetris) {
  CleanLA(tetris);
  if ((tetris->piece.rotation == 0) &&
      (tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == EMP)) {
    tetris->piece.rotation = 1;
  } else if ((tetris->piece.rotation == 1) && (tetris->piece.posx <= 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == EMP))
      tetris->piece.rotation = 2;
  } else if ((tetris->piece.rotation == 2) &&
             (tetris->field[tetris->piece.posy - 2][tetris->piece.posx] ==
              EMP) &&
             (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
              EMP)) {
    tetris->piece.rotation = 3;
  } else if ((tetris->piece.rotation == 3) && (tetris->piece.posx <= 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == EMP))
      tetris->piece.rotation = 0;
  }
}

void RARotate(GameInfo_t *tetris) {
  CleanRA(tetris);
  if ((tetris->piece.rotation == 0) &&
      (tetris->field[tetris->piece.posy][tetris->piece.posx] == EMP)) {
    tetris->piece.rotation = 1;
  } else if ((tetris->piece.rotation == 1) && (tetris->piece.posx <= 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == EMP) &&
        (tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP))
      tetris->piece.rotation = 2;
  } else if ((tetris->piece.rotation == 2) &&
             (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
              EMP)) {
    tetris->piece.rotation = 3;
  } else if ((tetris->piece.rotation == 3) && (tetris->piece.posx <= 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == EMP))
      tetris->piece.rotation = 0;
  }
}

void LZRotate(GameInfo_t *tetris) {
  CleanLZ(tetris);
  if ((tetris->piece.rotation == 0) &&
      (tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == EMP)) {
    tetris->piece.rotation = 1;
  } else if ((tetris->piece.rotation == 1) && (tetris->piece.posx <= 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == EMP))
      tetris->piece.rotation = 0;
  }
}

void RZRotate(GameInfo_t *tetris) {
  CleanRZ(tetris);
  if ((tetris->piece.rotation == 0) &&
      (tetris->field[tetris->piece.posy][tetris->piece.posx] == EMP)) {
    tetris->piece.rotation = 1;
  } else if ((tetris->piece.rotation == 1) && (tetris->piece.posx <= 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == EMP) &&
        (tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP))
      tetris->piece.rotation = 0;
  }
}

void TRotate(GameInfo_t *tetris) {
  CleanT(tetris);
  if (tetris->piece.rotation == 0) {
    tetris->piece.rotation = 1;
  } else if ((tetris->piece.rotation == 1) && (tetris->piece.posx <= 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx] == EMP) &&
        (tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == EMP))
      tetris->piece.rotation = 2;
  } else if ((tetris->piece.rotation == 2) &&
             (tetris->field[tetris->piece.posy - 2][tetris->piece.posx] ==
              EMP)) {
    tetris->piece.rotation = 3;
  } else if ((tetris->piece.rotation == 3) && (tetris->piece.posx <= 7)) {
    if ((tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == EMP) &&
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == EMP))
      tetris->piece.rotation = 0;
  }
}

void CleanLine(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 3][tetris->piece.posx] = EMP;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 3] = EMP;
  }
}

void CleanLA(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = EMP;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = EMP;
  } else if (tetris->piece.rotation == 2) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = EMP;
  } else if (tetris->piece.rotation == 3) {
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = EMP;
  }
}

void CleanRA(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = EMP;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = EMP;
  } else if (tetris->piece.rotation == 2) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = EMP;
  } else if (tetris->piece.rotation == 3) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = EMP;
  }
}

void CleanLZ(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = EMP;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = EMP;
  }
}

void CleanRZ(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = EMP;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = EMP;
  }
}

void CleanT(GameInfo_t *tetris) {
  tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = EMP;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = EMP;
  } else if (tetris->piece.rotation == 2) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = EMP;
  } else if (tetris->piece.rotation == 3) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = EMP;
  }
}

void CleanSquare(GameInfo_t *tetris) {
  tetris->field[tetris->piece.posy][tetris->piece.posx] = EMP;
  tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = EMP;
  tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = EMP;
  tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = EMP;
}

void PutLine(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 3][tetris->piece.posx] = FIL;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 3] = FIL;
  }
}

void PutLA(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = FIL;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = FIL;
  } else if (tetris->piece.rotation == 2) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = FIL;
  } else if (tetris->piece.rotation == 3) {
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = FIL;
  }
}

void PutRA(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = FIL;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = FIL;
  } else if (tetris->piece.rotation == 2) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = FIL;
  } else if (tetris->piece.rotation == 3) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = FIL;
  }
}

void PutLZ(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = FIL;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = FIL;
  }
}

void PutRZ(GameInfo_t *tetris) {
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = FIL;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = FIL;
  }
}

void PutT(GameInfo_t *tetris) {
  tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
  if (tetris->piece.rotation == 0) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] = FIL;
  } else if (tetris->piece.rotation == 1) {
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] = FIL;
  } else if (tetris->piece.rotation == 2) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
    tetris->field[tetris->piece.posy][tetris->piece.posx + 2] = FIL;
  } else if (tetris->piece.rotation == 3) {
    tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
    tetris->field[tetris->piece.posy - 2][tetris->piece.posx] = FIL;
  }
}

void PutSquare(GameInfo_t *tetris) {
  tetris->field[tetris->piece.posy][tetris->piece.posx] = FIL;
  tetris->field[tetris->piece.posy - 1][tetris->piece.posx] = FIL;
  tetris->field[tetris->piece.posy][tetris->piece.posx + 1] = FIL;
  tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] = FIL;
}

char LiConnect(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->piece.rotation == 0) && (tetris->piece.posy == 3)) ||
      ((tetris->piece.rotation == 1) && (tetris->piece.posy == 0)))
    result = 1;
  else if (tetris->piece.rotation == 1) {
    if (((tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL)) ||
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 3] == FIL))
      result = 1;
  } else if ((tetris->piece.rotation == 0) &&
             (tetris->field[tetris->piece.posy - 4][tetris->piece.posx] == FIL))
    result = 1;
  return result;
}

char LAConnect(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->piece.rotation % 2 == 0) && (tetris->piece.posy == 1)) ||
      ((tetris->piece.rotation % 2 == 1) && (tetris->piece.posy == 2)))
    result = 1;
  else if (tetris->piece.rotation == 0) {
    if ((tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 2] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 1) {
    if ((tetris->field[tetris->piece.posy - 3][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 2) {
    if ((tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 2] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 3)
    if ((tetris->field[tetris->piece.posy - 3][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 3][tetris->piece.posx + 1] == FIL))
      result = 1;
  return result;
}

char RAConnect(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->piece.rotation % 2 == 0) && (tetris->piece.posy == 1)) ||
      ((tetris->piece.rotation % 2 == 1) && (tetris->piece.posy == 2)))
    result = 1;
  else if (tetris->piece.rotation == 0) {
    if ((tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 2] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 1) {
    if ((tetris->field[tetris->piece.posy - 3][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 3][tetris->piece.posx + 1] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 2) {
    if ((tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 3)
    if ((tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 3][tetris->piece.posx + 1] == FIL))
      result = 1;

  return result;
}

char LZConnect(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->piece.rotation == 0) && (tetris->piece.posy == 1)) ||
      ((tetris->piece.rotation == 1) && (tetris->piece.posy == 2)))
    result = 1;
  else if (tetris->piece.rotation == 0) {
    if ((tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 2] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 1)
    if ((tetris->field[tetris->piece.posy - 3][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] == FIL))
      result = 1;

  return result;
}

char RZConnect(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->piece.rotation == 0) && (tetris->piece.posy == 1)) ||
      ((tetris->piece.rotation == 1) && (tetris->piece.posy == 2)))
    result = 1;
  else if (tetris->piece.rotation == 0) {
    if ((tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 1)
    if ((tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 3][tetris->piece.posx + 1] == FIL))
      result = 1;

  return result;
}

char SqConnect(GameInfo_t *tetris) {
  char result = 0;
  if (tetris->piece.posy == 1)
    result = 1;
  else if ((tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == FIL) ||
           (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
            FIL))
    result = 1;
  return result;
}

char TConnect(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->piece.rotation % 2 == 0) && (tetris->piece.posy == 1)) ||
      ((tetris->piece.rotation % 2 == 1) && (tetris->piece.posy == 2)))
    result = 1;
  else if (tetris->piece.rotation == 0) {
    if ((tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 2] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 1) {
    if ((tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 3][tetris->piece.posx + 1] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 2) {
    if ((tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] ==
         FIL) ||
        (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == FIL))
      result = 1;
  } else if (tetris->piece.rotation == 3)
    if ((tetris->field[tetris->piece.posy - 3][tetris->piece.posx] == FIL) ||
        (tetris->field[tetris->piece.posy - 2][tetris->piece.posx + 1] == FIL))
      result = 1;

  return result;
}

char LiCheckEnd(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->field[tetris->piece.posy][tetris->piece.posx] == FIL)) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL) ||
      (tetris->field[tetris->piece.posy - 2][tetris->piece.posx] == FIL) ||
      (tetris->field[tetris->piece.posy - 3][tetris->piece.posx] == FIL))
    result = 1;
  return result;
}

char LACheckEnd(GameInfo_t *tetris) {
  char result = 0;
  if ((tetris->field[tetris->piece.posy][tetris->piece.posx] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == FIL))
    result = 1;
  return result;
}

char RACheckEnd(GameInfo_t *tetris) {
  char result = 0;
  if ((tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == FIL) ||
      (tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == FIL))
    result = 1;
  return result;
}

char LZCheckEnd(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->field[tetris->piece.posy][tetris->piece.posx] == FIL)) ||
      (tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == FIL))
    result = 1;
  return result;
}

char RZCheckEnd(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL)) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy][tetris->piece.posx + 2] == FIL))
    result = 1;
  return result;
}

char SqCheckEnd(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->field[tetris->piece.posy][tetris->piece.posx] == FIL)) ||
      (tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL))
    result = 1;
  return result;
}

char TCheckEnd(GameInfo_t *tetris) {
  char result = 0;
  if (((tetris->field[tetris->piece.posy - 1][tetris->piece.posx] == FIL)) ||
      (tetris->field[tetris->piece.posy][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 1] == FIL) ||
      (tetris->field[tetris->piece.posy - 1][tetris->piece.posx + 2] == FIL))
    result = 1;
  return result;
}
