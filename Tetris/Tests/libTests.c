#include "libTests.h"

START_TEST(init) {
  GameInfo_t *new = updateCurrentState();
  ck_assert_int_eq(new->level, 0);
  ck_assert_int_eq(new->speed, 50);
  ck_assert_int_eq(new->pause, 2);
  Destroy(new);
}
END_TEST

START_TEST(NG) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->score, 0);
  ck_assert_int_eq(new->pause, 0);
  for (int i = 0; i < HEI - 4; ++i) {
    for (int j = 0; j < WID; ++j) ck_assert_int_eq(new->field[i][j], EMP);
    ck_assert_int_eq(new->field[i][WID], 0);
  }
  Destroy(new);
}
END_TEST

START_TEST(P) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);
  PauseResume(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->pause, 1);
  PauseResume(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->pause, 0);
  Destroy(new);
}
END_TEST

START_TEST(End) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);
  EndGame(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->pause, 3);
  ck_assert_int_eq(new->score, 0);
  Destroy(new);
}
END_TEST

START_TEST(Spawning) {
  GameInfo_t *new = updateCurrentState();
  new->next = T;

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;

  Spawn(new);

  ck_assert_int_eq(new->piece.posx, WID / 2);
  ck_assert_int_eq(new->piece.posy, HEI - 1);
  ck_assert_int_eq(new->piece.type, T);
  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(Move) {
  GameInfo_t *new = updateCurrentState();

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  StartGame(new);
  MovePiece(3, -3, new);

  ck_assert_int_eq(new->piece.posx, WID / 2 + 3);
  ck_assert_int_eq(new->piece.posy, HEI - 4);
  Destroy(new);
}
END_TEST

START_TEST(ScoreLevel) {
  GameInfo_t *new = updateCurrentState();

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  StartGame(new);
  new = updateCurrentState();
  for (int i = 0; i < WID; ++i) new->field[0][i] = FIL;

  CheckApplyScore(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->score, 100);
  ck_assert_int_eq(new->high_score, 100);
  ck_assert_int_eq(new->level, 0);
  for (int i = 0; i < WID; ++i) {
    new->field[1][i] = FIL;
    new->field[2][i] = FIL;
  }

  CheckApplyScore(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->score, 400);
  ck_assert_int_eq(new->high_score, 400);
  ck_assert_int_eq(new->level, 0);
  for (int i = 0; i < WID; ++i) {
    new->field[3][i] = FIL;
    new->field[4][i] = FIL;
    new->field[5][i] = FIL;
  }

  CheckApplyScore(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->score, 1100);
  ck_assert_int_eq(new->high_score, 1100);
  ck_assert_int_eq(new->level, 1);
  for (int i = 0; i < WID; ++i) {
    new->field[6][i] = FIL;
    new->field[7][i] = FIL;
    new->field[8][i] = FIL;
    new->field[9][i] = FIL;
  }

  CheckApplyScore(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->score, 2600);
  ck_assert_int_eq(new->high_score, 2600);
  ck_assert_int_eq(new->level, 4);
  Destroy(new);
}
END_TEST

START_TEST(LineLeft1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  LiShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LineLeft2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  LiShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LineLeft3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][4] = FIL;

  LiShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LineLeft4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[5][4] = FIL;

  LiShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LALeft1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  LAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LALeft2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  LAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LALeft3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  LAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LALeft4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  LAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LALeft5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][4] = FIL;

  LAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LALeft6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[3][4] = FIL;

  LAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LALeft7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[5][4] = FIL;
  new->field[5][5] = FIL;
  new->field[5][6] = FIL;
  new->field[5][7] = FIL;
  new->field[4][7] = FIL;

  LAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LALeft8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[4][5] = FIL;

  LAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RALeft1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  RAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RALeft2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  RAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RALeft3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  RAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RALeft4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  RAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RALeft5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][6] = FIL;

  RAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RALeft6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[4][4] = FIL;
  new->field[5][5] = FIL;
  new->field[4][5] = FIL;
  new->field[3][5] = FIL;
  new->field[3][6] = FIL;

  RAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RALeft7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[4][4] = FIL;
  new->field[5][5] = FIL;
  new->field[4][5] = FIL;
  new->field[5][6] = FIL;
  new->field[5][7] = FIL;

  RAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RALeft8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[3][5] = FIL;

  RAShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LZLeft1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  LZShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LZLeft2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  LZShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LZLeft3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][4] = FIL;
  new->field[5][5] = FIL;
  new->field[5][6] = FIL;
  new->field[4][6] = FIL;
  new->field[4][7] = FIL;

  LZShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LZLeft4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[5][5] = FIL;

  LZShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RZLeft1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  RZShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RZLeft2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  RZShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RZLeft3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][5] = FIL;

  RZShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RZLeft4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[4][4] = FIL;

  RZShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(SqLeft1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  SqShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(SqLeft2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[4][4] = FIL;

  SqShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TLeft1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  TShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TLeft2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  TShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TLeft3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  TShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TLeft4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  TShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 4);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TLeft5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][5] = FIL;

  TShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TLeft6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[4][4] = FIL;

  TShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TLeft7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[5][4] = FIL;
  new->field[5][5] = FIL;
  new->field[5][6] = FIL;
  new->field[5][7] = FIL;
  new->field[4][6] = FIL;

  TShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TLeft8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[3][4] = FIL;
  new->field[5][5] = FIL;
  new->field[4][5] = FIL;
  new->field[3][5] = FIL;
  new->field[4][6] = FIL;

  TShiftLeft(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LineRight1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  LiShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LineRight2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  LiShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LineRight3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][6] = FIL;

  LiShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LineRight4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[5][9] = FIL;

  LiShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LARight1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  LAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LARight2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  LAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LARight3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  LAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LARight4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  LAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LARight5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][6] = FIL;

  LAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LARight6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[3][6] = FIL;

  LAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LARight7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[4][8] = FIL;

  LAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LARight8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[4][7] = FIL;

  LAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RARight1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  RAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RARight2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  RAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RARight3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  RAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RARight4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  RAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RARight5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][8] = FIL;

  RAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RARight6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[5][6] = FIL;
  new->field[5][5] = FIL;
  new->field[4][5] = FIL;
  new->field[3][5] = FIL;
  new->field[3][6] = FIL;

  RAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RARight7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[5][8] = FIL;

  RAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RARight8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[3][7] = FIL;

  RAShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LZRight1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  LZShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LZRight2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  LZShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LZRight3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][7] = FIL;

  LZShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LZRight4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[4][7] = FIL;

  LZShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RZRight1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  RZShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RZRight2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  RZShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RZRight3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][8] = FIL;

  RZShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(RZRight4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[5][6] = FIL;

  RZShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(SqRight1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  SqShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(SqRight2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[4][7] = FIL;

  SqShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TRight1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  TShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TRight2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  TShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TRight3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  TShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TRight4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  TShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 6);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TRight5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][7] = FIL;

  TShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TRight6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[3][7] = FIL;

  TShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TRight7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[5][8] = FIL;

  TShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(TRight8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[5][6] = FIL;

  TShiftRight(new);

  ck_assert_int_eq(new->piece.posx, 5);
  ck_assert_int_eq(new->piece.posy, 5);
  Destroy(new);
}
END_TEST

START_TEST(LinePut1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  PutLine(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[3][5], FIL);
  ck_assert_int_eq(new->field[2][5], FIL);
  Destroy(new);
}
END_TEST

START_TEST(LinePut2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  PutLine(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[5][7], FIL);
  ck_assert_int_eq(new->field[5][8], FIL);
  Destroy(new);
}
END_TEST

START_TEST(LineClean1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanLine(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[3][5], EMP);
  ck_assert_int_eq(new->field[2][5], EMP);
  Destroy(new);
}
END_TEST

START_TEST(LineClean2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanLine(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[5][7], EMP);
  ck_assert_int_eq(new->field[5][8], EMP);
  Destroy(new);
}
END_TEST

START_TEST(LAPut1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  PutLA(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[4][7], FIL);
  Destroy(new);
}
END_TEST

START_TEST(LAPut2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  PutLA(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[3][5], FIL);
  Destroy(new);
}
END_TEST

START_TEST(LAPut3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  PutLA(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[5][7], FIL);
  ck_assert_int_eq(new->field[4][7], FIL);
  Destroy(new);
}
END_TEST

START_TEST(LAPut4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  PutLA(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[3][6], FIL);
  ck_assert_int_eq(new->field[3][5], FIL);
  Destroy(new);
}
END_TEST

START_TEST(LAClean1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanLA(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[4][7], EMP);
  Destroy(new);
}
END_TEST

START_TEST(LAClean2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanLA(new);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[3][5], EMP);
  Destroy(new);
}
END_TEST

START_TEST(LAClean3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanLA(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[5][7], EMP);
  ck_assert_int_eq(new->field[4][7], EMP);
  Destroy(new);
}
END_TEST

START_TEST(LAClean4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanLA(new);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[3][6], EMP);
  ck_assert_int_eq(new->field[3][5], EMP);
  Destroy(new);
}
END_TEST

START_TEST(RAPut1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  PutRA(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[4][7], FIL);
  ck_assert_int_eq(new->field[5][7], FIL);
  Destroy(new);
}
END_TEST

START_TEST(RAPut2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  PutRA(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[3][5], FIL);
  ck_assert_int_eq(new->field[3][6], FIL);
  Destroy(new);
}
END_TEST

START_TEST(RAPut3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  PutRA(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[5][7], FIL);
  Destroy(new);
}
END_TEST

START_TEST(RAPut4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  PutRA(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[3][6], FIL);
  Destroy(new);
}
END_TEST

START_TEST(RAClean1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanRA(new);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[4][7], EMP);
  ck_assert_int_eq(new->field[5][7], EMP);
  Destroy(new);
}
END_TEST

START_TEST(RAClean2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanRA(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[3][5], EMP);
  ck_assert_int_eq(new->field[3][6], EMP);
  Destroy(new);
}
END_TEST

START_TEST(RAClean3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanRA(new);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[5][7], EMP);
  Destroy(new);
}
END_TEST

START_TEST(RAClean4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanRA(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[3][6], EMP);
  Destroy(new);
}
END_TEST

START_TEST(LZPut1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  PutLZ(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[4][7], FIL);
  Destroy(new);
}
END_TEST

START_TEST(LZPut2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  PutLZ(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[3][5], FIL);
  Destroy(new);
}
END_TEST

START_TEST(LZClean1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanLZ(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[4][7], EMP);
  Destroy(new);
}
END_TEST

START_TEST(LZClean2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanLZ(new);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[3][5], EMP);
  Destroy(new);
}
END_TEST

START_TEST(RZPut1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  PutRZ(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[5][7], FIL);
  Destroy(new);
}
END_TEST

START_TEST(RZPut2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  PutRZ(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[3][6], FIL);
  Destroy(new);
}
END_TEST

START_TEST(RZClean1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanRZ(new);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[5][7], EMP);
  Destroy(new);
}
END_TEST

START_TEST(RZClean2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanRZ(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[3][6], EMP);
  Destroy(new);
}
END_TEST

START_TEST(SqPut1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  PutSquare(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[5][6], FIL);
  Destroy(new);
}
END_TEST

START_TEST(SqClean1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanSquare(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[5][6], EMP);
  Destroy(new);
}
END_TEST

START_TEST(TPut1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  PutT(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[4][7], FIL);
  Destroy(new);
}
END_TEST

START_TEST(TPut2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  PutT(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[3][6], FIL);
  Destroy(new);
}
END_TEST

START_TEST(TPut3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  PutT(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[5][6], FIL);
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[5][7], FIL);
  Destroy(new);
}
END_TEST

START_TEST(TPut4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  PutT(new);
  new = updateCurrentState();
  ck_assert_int_eq(new->field[4][6], FIL);
  ck_assert_int_eq(new->field[5][5], FIL);
  ck_assert_int_eq(new->field[4][5], FIL);
  ck_assert_int_eq(new->field[3][5], FIL);
  Destroy(new);
}
END_TEST

START_TEST(TClean1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanT(new);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[4][7], EMP);
  Destroy(new);
}
END_TEST

START_TEST(TClean2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanT(new);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[3][6], EMP);
  Destroy(new);
}
END_TEST

START_TEST(TClean3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanT(new);
  ck_assert_int_eq(new->field[5][6], EMP);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  ck_assert_int_eq(new->field[5][7], EMP);
  Destroy(new);
}
END_TEST

START_TEST(TClean4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  for (int i = 2; i < 6; ++i)
    for (int j = 0; j < WID; ++j) new->field[i][j] = FIL;

  CleanT(new);
  ck_assert_int_eq(new->field[5][5], EMP);
  ck_assert_int_eq(new->field[4][5], EMP);
  ck_assert_int_eq(new->field[3][5], EMP);
  ck_assert_int_eq(new->field[4][6], EMP);
  Destroy(new);
}
END_TEST

START_TEST(LineRotate1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  LiRotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(LineRotate2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  LiRotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(LineRotate3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][7] = FIL;

  LiRotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(LineRotate4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[2][5] = FIL;

  LiRotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(LARotate1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  LARotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(LARotate2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  LARotate(new);

  ck_assert_int_eq(new->piece.rotation, 2);
  Destroy(new);
}
END_TEST

START_TEST(LARotate3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  LARotate(new);

  ck_assert_int_eq(new->piece.rotation, 3);
  Destroy(new);
}
END_TEST

START_TEST(LARotate4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  LARotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(LARotate5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][6] = FIL;

  LARotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(LARotate6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[4][7] = FIL;

  LARotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(LARotate7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[3][6] = FIL;

  LARotate(new);

  ck_assert_int_eq(new->piece.rotation, 2);
  Destroy(new);
}
END_TEST

START_TEST(LARotate8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[4][5] = FIL;

  LARotate(new);

  ck_assert_int_eq(new->piece.rotation, 3);
  Destroy(new);
}
END_TEST

START_TEST(RARotate1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  RARotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(RARotate2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  RARotate(new);

  ck_assert_int_eq(new->piece.rotation, 2);
  Destroy(new);
}
END_TEST

START_TEST(RARotate3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  RARotate(new);

  ck_assert_int_eq(new->piece.rotation, 3);
  Destroy(new);
}
END_TEST

START_TEST(RARotate4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  RARotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(RARotate5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][5] = FIL;

  RARotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(RARotate6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[5][6] = FIL;

  RARotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(RARotate7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[3][6] = FIL;

  RARotate(new);

  ck_assert_int_eq(new->piece.rotation, 2);
  Destroy(new);
}
END_TEST

START_TEST(RARotate8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[5][7] = FIL;

  RARotate(new);

  ck_assert_int_eq(new->piece.rotation, 3);
  Destroy(new);
}
END_TEST

START_TEST(LZRotate1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  LZRotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(LZRotate2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  LZRotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(LZRotate3) {
  GameInfo_t *new = updateCurrentState();

  StartGame(new);
  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[3][5] = FIL;

  LZRotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(LZRotate4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[5][5] = FIL;

  LZRotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(RZRotate1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  RZRotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(RZRotate2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  RZRotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(RZRotate3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[5][5] = FIL;

  RZRotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(RZRotate4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[5][6] = FIL;

  RZRotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(TRotate1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  TRotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(TRotate2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  TRotate(new);

  ck_assert_int_eq(new->piece.rotation, 2);
  Destroy(new);
}
END_TEST

START_TEST(TRotate3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  TRotate(new);

  ck_assert_int_eq(new->piece.rotation, 3);
  Destroy(new);
}
END_TEST

START_TEST(TRotate4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  TRotate(new);

  ck_assert_int_eq(new->piece.rotation, 0);
  Destroy(new);
}
END_TEST

START_TEST(TRotate6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[5][5] = FIL;

  TRotate(new);

  ck_assert_int_eq(new->piece.rotation, 1);
  Destroy(new);
}
END_TEST

START_TEST(TRotate7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[3][5] = FIL;

  TRotate(new);

  ck_assert_int_eq(new->piece.rotation, 2);
  Destroy(new);
}
END_TEST

START_TEST(TRotate8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[5][6] = FIL;

  TRotate(new);

  ck_assert_int_eq(new->piece.rotation, 3);
  Destroy(new);
}
END_TEST

START_TEST(LineConnect1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(LiConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LineConnect2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  ck_assert_int_eq(LiConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LineConnect3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[1][5] = FIL;

  ck_assert_int_eq(LiConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LineConnect4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[4][8] = FIL;

  ck_assert_int_eq(LiConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LAConnect1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(LAConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LAConnect2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  ck_assert_int_eq(LAConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LAConnect3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  ck_assert_int_eq(LAConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LAConnect4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  ck_assert_int_eq(LAConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LAConnect5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[3][6] = FIL;

  ck_assert_int_eq(LAConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LAConnect6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[4][6] = FIL;

  ck_assert_int_eq(LAConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LAConnect7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[4][6] = FIL;

  ck_assert_int_eq(LAConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LAConnect8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[2][5] = FIL;

  ck_assert_int_eq(LAConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(RAConnect1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(RAConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(RAConnect2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  ck_assert_int_eq(RAConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(RAConnect3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  ck_assert_int_eq(RAConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(RAConnect4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  ck_assert_int_eq(RAConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(RAConnect5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[3][6] = FIL;

  ck_assert_int_eq(RAConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(RAConnect6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[2][5] = FIL;

  ck_assert_int_eq(RAConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(RAConnect7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[4][6] = FIL;

  ck_assert_int_eq(RAConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(RAConnect8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[4][5] = FIL;

  ck_assert_int_eq(RAConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LZConnect1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(LZConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LZConnect2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  ck_assert_int_eq(LZConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LZConnect3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[3][6] = FIL;

  ck_assert_int_eq(LZConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LZConnect4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[2][5] = FIL;

  ck_assert_int_eq(LZConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(RZConnect1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(RZConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(RZConnect2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  ck_assert_int_eq(RZConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(RZConnect3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][7] = FIL;

  ck_assert_int_eq(RZConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(RZConnect4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[2][6] = FIL;

  ck_assert_int_eq(RZConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(SqConnect1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(SqConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(SqConnect2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[3][6] = FIL;

  ck_assert_int_eq(SqConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(TConnect1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(TConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(TConnect2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();

  ck_assert_int_eq(TConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(TConnect3) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();

  ck_assert_int_eq(TConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(TConnect4) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();

  ck_assert_int_eq(TConnect(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(TConnect5) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[3][6] = FIL;

  ck_assert_int_eq(TConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(TConnect6) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 1;
  new = updateCurrentState();
  new->field[3][5] = FIL;

  ck_assert_int_eq(TConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(TConnect7) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 2;
  new = updateCurrentState();
  new->field[4][7] = FIL;

  ck_assert_int_eq(TConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(TConnect8) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 3;
  new = updateCurrentState();
  new->field[3][6] = FIL;

  ck_assert_int_eq(TConnect(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LineCheck1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(LiCheckEnd(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LineCheck2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Line;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][5] = FIL;

  ck_assert_int_eq(LiCheckEnd(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LACheck1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(LACheckEnd(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LACheck2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][5] = FIL;

  ck_assert_int_eq(LACheckEnd(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(RACheck1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(RACheckEnd(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(RACheck2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftAngle;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][5] = FIL;

  ck_assert_int_eq(RACheckEnd(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(LZCheck1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(LZCheckEnd(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(LZCheck2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = LeftZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][6] = FIL;

  ck_assert_int_eq(LZCheckEnd(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(RZCheck1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(RZCheckEnd(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(RZCheck2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = RightZ;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][5] = FIL;

  ck_assert_int_eq(RZCheckEnd(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(SqCheck1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(SqCheckEnd(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(SqCheck2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = Square;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][5] = FIL;

  ck_assert_int_eq(SqCheckEnd(new), 1);
  Destroy(new);
}
END_TEST

START_TEST(TCheck1) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();

  ck_assert_int_eq(TCheckEnd(new), 0);
  Destroy(new);
}
END_TEST

START_TEST(TCheck2) {
  GameInfo_t *new = updateCurrentState();
  StartGame(new);

  new->piece.type = T;
  new->piece.posx = 5;
  new->piece.posy = 5;
  new->piece.rotation = 0;
  new = updateCurrentState();
  new->field[4][5] = FIL;

  ck_assert_int_eq(TCheckEnd(new), 1);
  Destroy(new);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1 = tcase_create("General");
  TCase *tc2 = tcase_create("Shift");
  TCase *tc3 = tcase_create("PutClean");
  TCase *tc4 = tcase_create("Rotate");
  TCase *tc5 = tcase_create("Connect");
  TCase *tc6 = tcase_create("Check");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1);
  suite_add_tcase(s1, tc2);
  suite_add_tcase(s1, tc3);
  suite_add_tcase(s1, tc4);
  suite_add_tcase(s1, tc5);
  suite_add_tcase(s1, tc6);

  tcase_add_test(tc1, init);
  tcase_add_test(tc1, NG);
  tcase_add_test(tc1, P);
  tcase_add_test(tc1, End);
  tcase_add_test(tc1, Spawning);
  tcase_add_test(tc1, Move);
  tcase_add_test(tc1, ScoreLevel);
  tcase_add_test(tc2, LineLeft1);
  tcase_add_test(tc2, LineLeft2);
  tcase_add_test(tc2, LineLeft3);
  tcase_add_test(tc2, LineLeft4);
  tcase_add_test(tc2, LALeft1);
  tcase_add_test(tc2, LALeft2);
  tcase_add_test(tc2, LALeft3);
  tcase_add_test(tc2, LALeft4);
  tcase_add_test(tc2, LALeft5);
  tcase_add_test(tc2, LALeft6);
  tcase_add_test(tc2, LALeft7);
  tcase_add_test(tc2, LALeft8);
  tcase_add_test(tc2, RALeft1);
  tcase_add_test(tc2, RALeft2);
  tcase_add_test(tc2, RALeft3);
  tcase_add_test(tc2, RALeft4);
  tcase_add_test(tc2, RALeft5);
  tcase_add_test(tc2, RALeft6);
  tcase_add_test(tc2, RALeft7);
  tcase_add_test(tc2, RALeft8);
  tcase_add_test(tc2, LZLeft1);
  tcase_add_test(tc2, LZLeft2);
  tcase_add_test(tc2, LZLeft3);
  tcase_add_test(tc2, LZLeft4);
  tcase_add_test(tc2, RZLeft1);
  tcase_add_test(tc2, RZLeft2);
  tcase_add_test(tc2, RZLeft3);
  tcase_add_test(tc2, RZLeft4);
  tcase_add_test(tc2, SqLeft1);
  tcase_add_test(tc2, SqLeft2);
  tcase_add_test(tc2, TLeft1);
  tcase_add_test(tc2, TLeft2);
  tcase_add_test(tc2, TLeft3);
  tcase_add_test(tc2, TLeft4);
  tcase_add_test(tc2, TLeft5);
  tcase_add_test(tc2, TLeft6);
  tcase_add_test(tc2, TLeft7);
  tcase_add_test(tc2, TLeft8);
  tcase_add_test(tc2, LineRight1);
  tcase_add_test(tc2, LineRight2);
  tcase_add_test(tc2, LineRight3);
  tcase_add_test(tc2, LineRight4);
  tcase_add_test(tc2, LARight1);
  tcase_add_test(tc2, LARight2);
  tcase_add_test(tc2, LARight3);
  tcase_add_test(tc2, LARight4);
  tcase_add_test(tc2, LARight5);
  tcase_add_test(tc2, LARight6);
  tcase_add_test(tc2, LARight7);
  tcase_add_test(tc2, LARight8);
  tcase_add_test(tc2, RARight1);
  tcase_add_test(tc2, RARight2);
  tcase_add_test(tc2, RARight3);
  tcase_add_test(tc2, RARight4);
  tcase_add_test(tc2, RARight5);
  tcase_add_test(tc2, RARight6);
  tcase_add_test(tc2, RARight7);
  tcase_add_test(tc2, RARight8);
  tcase_add_test(tc2, LZRight1);
  tcase_add_test(tc2, LZRight2);
  tcase_add_test(tc2, LZRight3);
  tcase_add_test(tc2, LZRight4);
  tcase_add_test(tc2, RZRight1);
  tcase_add_test(tc2, RZRight2);
  tcase_add_test(tc2, RZRight3);
  tcase_add_test(tc2, RZRight4);
  tcase_add_test(tc2, SqRight1);
  tcase_add_test(tc2, SqRight2);
  tcase_add_test(tc2, TRight1);
  tcase_add_test(tc2, TRight2);
  tcase_add_test(tc2, TRight3);
  tcase_add_test(tc2, TRight4);
  tcase_add_test(tc2, TRight5);
  tcase_add_test(tc2, TRight6);
  tcase_add_test(tc2, TRight7);
  tcase_add_test(tc2, TRight8);
  tcase_add_test(tc3, LinePut1);
  tcase_add_test(tc3, LinePut2);
  tcase_add_test(tc3, LineClean1);
  tcase_add_test(tc3, LineClean2);
  tcase_add_test(tc3, LAPut1);
  tcase_add_test(tc3, LAPut2);
  tcase_add_test(tc3, LAPut3);
  tcase_add_test(tc3, LAPut4);
  tcase_add_test(tc3, LAClean1);
  tcase_add_test(tc3, LAClean2);
  tcase_add_test(tc3, LAClean3);
  tcase_add_test(tc3, LAClean4);
  tcase_add_test(tc3, RAPut1);
  tcase_add_test(tc3, RAPut2);
  tcase_add_test(tc3, RAPut3);
  tcase_add_test(tc3, RAPut4);
  tcase_add_test(tc3, RAClean1);
  tcase_add_test(tc3, RAClean2);
  tcase_add_test(tc3, RAClean3);
  tcase_add_test(tc3, RAClean4);
  tcase_add_test(tc3, LZPut1);
  tcase_add_test(tc3, LZPut2);
  tcase_add_test(tc3, LZClean1);
  tcase_add_test(tc3, LZClean2);
  tcase_add_test(tc3, RZPut1);
  tcase_add_test(tc3, RZPut2);
  tcase_add_test(tc3, RZClean1);
  tcase_add_test(tc3, RZClean2);
  tcase_add_test(tc3, SqPut1);
  tcase_add_test(tc3, SqClean1);
  tcase_add_test(tc3, TPut1);
  tcase_add_test(tc3, TPut2);
  tcase_add_test(tc3, TPut3);
  tcase_add_test(tc3, TPut4);
  tcase_add_test(tc3, TClean1);
  tcase_add_test(tc3, TClean2);
  tcase_add_test(tc3, TClean3);
  tcase_add_test(tc3, TClean4);
  tcase_add_test(tc4, LineRotate1);
  tcase_add_test(tc4, LineRotate2);
  tcase_add_test(tc4, LineRotate3);
  tcase_add_test(tc4, LineRotate4);
  tcase_add_test(tc4, LARotate1);
  tcase_add_test(tc4, LARotate2);
  tcase_add_test(tc4, LARotate3);
  tcase_add_test(tc4, LARotate4);
  tcase_add_test(tc4, LARotate5);
  tcase_add_test(tc4, LARotate6);
  tcase_add_test(tc4, LARotate7);
  tcase_add_test(tc4, LARotate8);
  tcase_add_test(tc4, RARotate1);
  tcase_add_test(tc4, RARotate2);
  tcase_add_test(tc4, RARotate3);
  tcase_add_test(tc4, RARotate4);
  tcase_add_test(tc4, RARotate5);
  tcase_add_test(tc4, RARotate6);
  tcase_add_test(tc4, RARotate7);
  tcase_add_test(tc4, RARotate8);
  tcase_add_test(tc4, LZRotate1);
  tcase_add_test(tc4, LZRotate2);
  tcase_add_test(tc4, LZRotate3);
  tcase_add_test(tc4, LZRotate4);
  tcase_add_test(tc4, RZRotate1);
  tcase_add_test(tc4, RZRotate2);
  tcase_add_test(tc4, RZRotate3);
  tcase_add_test(tc4, RZRotate4);
  tcase_add_test(tc4, TRotate1);
  tcase_add_test(tc4, TRotate2);
  tcase_add_test(tc4, TRotate3);
  tcase_add_test(tc4, TRotate4);
  tcase_add_test(tc4, TRotate6);
  tcase_add_test(tc4, TRotate7);
  tcase_add_test(tc4, TRotate8);
  tcase_add_test(tc5, LineConnect1);
  tcase_add_test(tc5, LineConnect2);
  tcase_add_test(tc5, LineConnect3);
  tcase_add_test(tc5, LineConnect4);
  tcase_add_test(tc5, LAConnect1);
  tcase_add_test(tc5, LAConnect2);
  tcase_add_test(tc5, LAConnect3);
  tcase_add_test(tc5, LAConnect4);
  tcase_add_test(tc5, LAConnect5);
  tcase_add_test(tc5, LAConnect6);
  tcase_add_test(tc5, LAConnect7);
  tcase_add_test(tc5, LAConnect8);
  tcase_add_test(tc5, RAConnect1);
  tcase_add_test(tc5, RAConnect2);
  tcase_add_test(tc5, RAConnect3);
  tcase_add_test(tc5, RAConnect4);
  tcase_add_test(tc5, RAConnect5);
  tcase_add_test(tc5, RAConnect6);
  tcase_add_test(tc5, RAConnect7);
  tcase_add_test(tc5, RAConnect8);
  tcase_add_test(tc5, LZConnect1);
  tcase_add_test(tc5, LZConnect2);
  tcase_add_test(tc5, LZConnect3);
  tcase_add_test(tc5, LZConnect4);
  tcase_add_test(tc5, RZConnect1);
  tcase_add_test(tc5, RZConnect2);
  tcase_add_test(tc5, RZConnect3);
  tcase_add_test(tc5, RZConnect4);
  tcase_add_test(tc5, SqConnect1);
  tcase_add_test(tc5, SqConnect2);
  tcase_add_test(tc5, TConnect1);
  tcase_add_test(tc5, TConnect2);
  tcase_add_test(tc5, TConnect3);
  tcase_add_test(tc5, TConnect4);
  tcase_add_test(tc5, TConnect5);
  tcase_add_test(tc5, TConnect6);
  tcase_add_test(tc5, TConnect7);
  tcase_add_test(tc5, TConnect8);
  tcase_add_test(tc6, LineCheck1);
  tcase_add_test(tc6, LineCheck2);
  tcase_add_test(tc6, LACheck1);
  tcase_add_test(tc6, LACheck2);
  tcase_add_test(tc6, RACheck1);
  tcase_add_test(tc6, RACheck2);
  tcase_add_test(tc6, LZCheck1);
  tcase_add_test(tc6, LZCheck2);
  tcase_add_test(tc6, RZCheck1);
  tcase_add_test(tc6, RZCheck2);
  tcase_add_test(tc6, SqCheck1);
  tcase_add_test(tc6, SqCheck2);
  tcase_add_test(tc6, TCheck1);
  tcase_add_test(tc6, TCheck2);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
