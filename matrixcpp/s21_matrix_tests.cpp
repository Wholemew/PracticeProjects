#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

using namespace std;

void matrixCPY2(S21Matrix& dest, double (*src)[2], int r) {
  for (int i = 0; i < r; ++i)
    for (int j = 0; j < 2; ++j) dest(i, j) = src[i][j];
}
void matrixCPY3(S21Matrix& dest, double (*src)[3], int r) {
  for (int i = 0; i < r; ++i)
    for (int j = 0; j < 3; ++j) dest(i, j) = src[i][j];
}
void matrixCPY4(S21Matrix& dest, double (*src)[4], int r) {
  for (int i = 0; i < r; ++i)
    for (int j = 0; j < 4; ++j) dest(i, j) = src[i][j];
}

namespace {
TEST(Constructors, zero) {
  S21Matrix m;
  EXPECT_EQ(m.getRows(), 1);
  EXPECT_EQ(m.getCols(), 1);
  EXPECT_DOUBLE_EQ(m(0, 0), 0);
}

TEST(Constructors, two) {
  S21Matrix m(1, 1);
  EXPECT_EQ(m.getRows(), 1);
  EXPECT_EQ(m.getCols(), 1);
  EXPECT_DOUBLE_EQ(m(0, 0), 0);
}

TEST(Constructors, copy) {
  S21Matrix m(1, 1);
  m(0, 0) = 5.5;
  S21Matrix& name = m;
  S21Matrix copy(name);
  EXPECT_EQ(copy.getRows(), 1);
  EXPECT_EQ(copy.getCols(), 1);
  EXPECT_DOUBLE_EQ(copy(0, 0), 5.5);
}

TEST(Constructors, move) {
  S21Matrix m(1, 1);
  m(0, 0) = 5.5;
  S21Matrix copy = std::move(m);
  EXPECT_EQ(copy.getRows(), 1);
  EXPECT_EQ(copy.getCols(), 1);
  EXPECT_DOUBLE_EQ(copy(0, 0), 5.5);
}

TEST(Indexation, init) {
  S21Matrix m;
  m(0, 0) = 5.0;
  EXPECT_DOUBLE_EQ(m(0, 0), 5.0);
}

TEST(Accessors, row) {
  S21Matrix m;
  EXPECT_EQ(m.getRows(), 1);
}

TEST(Accessors, col) {
  S21Matrix m;
  EXPECT_EQ(m.getCols(), 1);
}

TEST(Mutators, rowUp) {
  S21Matrix m;
  m.editRows(5);
  EXPECT_EQ(m.getRows(), 5);
  EXPECT_DOUBLE_EQ(m(3, 0), 0.0);
}

TEST(Accessors, colUp) {
  S21Matrix m;
  m.editCols(5);
  EXPECT_EQ(m.getCols(), 5);
  EXPECT_DOUBLE_EQ(m(0, 3), 0.0);
}

TEST(Mutators, rowDown) {
  S21Matrix m(5, 5);
  m.editRows(3);
  EXPECT_EQ(m.getRows(), 3);
}

TEST(Accessors, colDown) {
  S21Matrix m(5, 5);
  m.editCols(3);
  EXPECT_EQ(m.getCols(), 3);
}

TEST(E, equalOp) {
  S21Matrix m1(1, 1), m2(1, 1);
  m1(0, 0) = 7.6543217;
  m2(0, 0) = 7.6543216;
  EXPECT_TRUE(m1 == m2);
}

TEST(E, equalNotOp) {
  S21Matrix m1(1, 1), m2(1, 1);
  m1(0, 0) = 1.0;
  m2(0, 0) = 1.5;
  EXPECT_FALSE(m1 == m2);
}

TEST(E, equal) {
  S21Matrix m1(1, 1), m2(1, 1);
  m1(0, 0) = 7.6543217;
  m2(0, 0) = 7.6543216;
  EXPECT_TRUE(m1.EqMatrix(m2));
}

TEST(E, equalNot) {
  S21Matrix m1(1, 1), m2(1, 1);
  m1(0, 0) = 1.0;
  m2(0, 0) = 1.5;
  EXPECT_FALSE(m1.EqMatrix(m2));
}

TEST(Addition, func) {
  S21Matrix m1(2, 2), m2(2, 2), check(2, 2);
  double ch[2][2] = {{4.0, 2.0}, {2.0, 4.0}};
  double n1[2][2] = {{1.0, 2.0}, {0.0, 3.0}};
  double n2[2][2] = {{3.0, 0.0}, {2.0, 1.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  matrixCPY2(m2, n2, 2);
  m1.SumMatrix(m2);
  EXPECT_TRUE(m1.EqMatrix(check));
}

TEST(Addition, funcOp) {
  S21Matrix m1(2, 2), m2(2, 2), check(2, 2);
  double ch[2][2] = {{4.0, 2.0}, {2.0, 4.0}};
  double n1[2][2] = {{1.0, 2.0}, {0.0, 3.0}};
  double n2[2][2] = {{3.0, 0.0}, {2.0, 1.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  matrixCPY2(m2, n2, 2);
  m1 += m2;
  EXPECT_TRUE(m1.EqMatrix(check));
}

TEST(Addition, funcOp2) {
  S21Matrix m(2, 2), m1(2, 2), m2(2, 2), check(2, 2);
  double ch[2][2] = {{4.0, 2.0}, {2.0, 4.0}};
  double n1[2][2] = {{1.0, 2.0}, {0.0, 3.0}};
  double n2[2][2] = {{3.0, 0.0}, {2.0, 1.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  matrixCPY2(m2, n2, 2);
  m = m1 + m2;
  EXPECT_TRUE(m.EqMatrix(check));
}

TEST(Addition, except) {
  S21Matrix m1(2, 2), m2(1, 2);
  bool caught = false;
  try {
    m1.SumMatrix(m2);
  } catch (const char* msg) {
    caught = true;
  }
  EXPECT_TRUE(caught);
}

TEST(Subtraction, func) {
  S21Matrix m1(2, 2), m2(2, 2), check(2, 2);
  double ch[2][2] = {{-2.0, 2.0}, {-2.0, 2.0}};
  double n1[2][2] = {{1.0, 2.0}, {0.0, 3.0}};
  double n2[2][2] = {{3.0, 0.0}, {2.0, 1.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  matrixCPY2(m2, n2, 2);
  m1.SubMatrix(m2);
  EXPECT_TRUE(m1.EqMatrix(check));
}

TEST(Subtraction, funcOp) {
  S21Matrix m1(2, 2), m2(2, 2), check(2, 2);
  double ch[2][2] = {{-2.0, 2.0}, {-2.0, 2.0}};
  double n1[2][2] = {{1.0, 2.0}, {0.0, 3.0}};
  double n2[2][2] = {{3.0, 0.0}, {2.0, 1.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  matrixCPY2(m2, n2, 2);
  m1 -= m2;
  EXPECT_TRUE(m1.EqMatrix(check));
}

TEST(Subtraction, funcOp2) {
  S21Matrix m(2, 2), m1(2, 2), m2(2, 2), check(2, 2);
  double ch[2][2] = {{-2.0, 2.0}, {-2.0, 2.0}};
  double n1[2][2] = {{1.0, 2.0}, {0.0, 3.0}};
  double n2[2][2] = {{3.0, 0.0}, {2.0, 1.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  matrixCPY2(m2, n2, 2);
  m = m1 - m2;
  EXPECT_TRUE(m.EqMatrix(check));
}

TEST(Subtraction, except) {
  S21Matrix m1(2, 2), m2(1, 2);
  bool caught = false;
  try {
    m1.SubMatrix(m2);
  } catch (const char* msg) {
    caught = true;
  }
  EXPECT_TRUE(caught);
}

TEST(Mulnum, func) {
  S21Matrix m1(2, 2), check(2, 2);
  double ch[2][2] = {{2.0, 4.0}, {6.0, 8.0}};
  double n1[2][2] = {{1.0, 2.0}, {3.0, 4.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  m1.MulNumber(2);
  EXPECT_TRUE(m1.EqMatrix(check));
}

TEST(Mulnum, funcOp) {
  S21Matrix m1(2, 2), check(2, 2);
  double ch[2][2] = {{2.0, 4.0}, {6.0, 8.0}};
  double n1[2][2] = {{1.0, 2.0}, {3.0, 4.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  m1 *= 2;
  EXPECT_TRUE(m1.EqMatrix(check));
}

TEST(Mulnum, funcOp2) {
  S21Matrix m(2, 2), m1(2, 2), check(2, 2);
  double ch[2][2] = {{2.0, 4.0}, {6.0, 8.0}};
  double n1[2][2] = {{1.0, 2.0}, {3.0, 4.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  m = m1 * 2;
  EXPECT_TRUE(m.EqMatrix(check));
}

TEST(Mulnum, funcOp3) {
  S21Matrix m(2, 2), m1(2, 2), check(2, 2);
  double ch[2][2] = {{2.0, 4.0}, {6.0, 8.0}};
  double n1[2][2] = {{1.0, 2.0}, {3.0, 4.0}};
  matrixCPY2(check, ch, 2);
  matrixCPY2(m1, n1, 2);
  m = 2 * m1;
  EXPECT_TRUE(m.EqMatrix(check));
}

TEST(mulmat, func) {
  S21Matrix m1(3, 2), m2(2, 3), check(3, 3);
  double n1[3][2] = {{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}};
  double n2[2][3] = {{1.0, -1.0, 1.0}, {2.0, 3.0, 4.0}};
  double ch[3][3] = {{9.0, 11.0, 17.0}, {12.0, 13.0, 22.0}, {15.0, 15.0, 27.0}};
  matrixCPY3(check, ch, 3);
  matrixCPY2(m1, n1, 3);
  matrixCPY3(m2, n2, 2);
  m1.MulMatrix(m2);
  EXPECT_TRUE(m1.EqMatrix(check));
}

TEST(mulmat, except) {
  S21Matrix m1(3, 2), m2(3, 3);
  bool caught = false;
  try {
    m1.MulMatrix(m2);
  } catch (const char* msg) {
    caught = true;
  }
  EXPECT_TRUE(caught);
}

TEST(mulmat, funcOp) {
  S21Matrix m1(3, 2), m2(2, 3), check(3, 3);
  double n1[3][2] = {{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}};
  double n2[2][3] = {{1.0, -1.0, 1.0}, {2.0, 3.0, 4.0}};
  double ch[3][3] = {{9.0, 11.0, 17.0}, {12.0, 13.0, 22.0}, {15.0, 15.0, 27.0}};
  matrixCPY3(check, ch, 3);
  matrixCPY2(m1, n1, 3);
  matrixCPY3(m2, n2, 2);
  m1 *= m2;
  EXPECT_TRUE(m1.EqMatrix(check));
}

TEST(mulmat, funcOp2) {
  S21Matrix m1(3, 2), m2(2, 3), check(3, 3), m(3, 3);
  double n1[3][2] = {{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}};
  double n2[2][3] = {{1.0, -1.0, 1.0}, {2.0, 3.0, 4.0}};
  double ch[3][3] = {{9.0, 11.0, 17.0}, {12.0, 13.0, 22.0}, {15.0, 15.0, 27.0}};
  matrixCPY3(check, ch, 3);
  matrixCPY2(m1, n1, 3);
  matrixCPY3(m2, n2, 2);
  m = m1 * m2;
  EXPECT_TRUE(m.EqMatrix(check));
}

TEST(trans, func) {
  S21Matrix m1(3, 2), m2(2, 3), check(2, 3);
  double ch[2][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
  double n1[3][2] = {{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}};
  matrixCPY3(check, ch, 2);
  matrixCPY2(m1, n1, 3);
  m2 = m1.Transpose();
  EXPECT_TRUE(m2.EqMatrix(check));
}

TEST(Complement, func) {
  S21Matrix m1(3, 3), m(3, 3), check(3, 3);
  double ch[3][3] = {{0.0, 10.0, -20.0}, {4.0, -14.0, 8.0}, {-8.0, -2.0, 4.0}};
  double n1[3][3] = {{1.0, 2.0, 3.0}, {0.0, 4.0, 2.0}, {5.0, 2.0, 1.0}};
  matrixCPY3(check, ch, 3);
  matrixCPY3(m1, n1, 3);
  m = m1.CalcComplements();
  EXPECT_TRUE(m.EqMatrix(check));
}

TEST(Complement, except) {
  S21Matrix m1(3, 2);
  bool caught = false;
  try {
    m1.CalcComplements();
  } catch (const char* msg) {
    caught = true;
  }
  EXPECT_TRUE(caught);
}

TEST(Determinant, func) {
  S21Matrix m1(3, 3);
  double d;
  double n1[3][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  matrixCPY3(m1, n1, 3);
  d = m1.Determinant();
  EXPECT_DOUBLE_EQ(d, 0.0);
}

TEST(Determinant, diag) {
  S21Matrix m1(4, 4);
  double d;
  double n1[4][4] = {{1.0, 2.0, 3.0, 4.0},
                     {0.0, 2.0, 6.0, 7.0},
                     {0.0, 0.0, 3.0, 4.0},
                     {0.0, 0.0, 0.0, 4.0}};
  matrixCPY4(m1, n1, 4);
  d = m1.Determinant();
  EXPECT_DOUBLE_EQ(d, 24.0);
}

TEST(Determinant, except) {
  S21Matrix m1(3, 2);
  bool caught = false;
  try {
    m1.Determinant();
  } catch (const char* msg) {
    caught = true;
  }
  EXPECT_TRUE(caught);
}

TEST(Inverse, func) {
  S21Matrix m1(3, 3), m(3, 3), check(3, 3);
  double ch[3][3] = {
      {1.0, -1.0, 1.0}, {-38.0, 41.0, -34.0}, {27.0, -29.0, 24.0}};
  double n1[3][3] = {{2.0, 5.0, 7.0}, {6.0, 3.0, 4.0}, {5.0, -2.0, -3.0}};
  matrixCPY3(check, ch, 3);
  matrixCPY3(m1, n1, 3);
  m = m1.InverseMatrix();
  EXPECT_TRUE(m.EqMatrix(check));
}

TEST(Inverse, solo) {
  S21Matrix m1(1, 1), m(1, 1), check(1, 1);
  m1(0, 0) = 2;
  check(0, 0) = 0.5;
  m = m1.InverseMatrix();
  EXPECT_TRUE(m.EqMatrix(check));
}

TEST(Inverse, zeroEx) {
  S21Matrix m1(3, 3);
  double n1[3][3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  matrixCPY3(m1, n1, 3);
  bool caught = false;
  try {
    m1.InverseMatrix();
  } catch (const char* msg) {
    caught = true;
  }
  EXPECT_TRUE(caught);
}

TEST(Inverse, except) {
  S21Matrix m1(1, 2);
  bool caught = false;
  try {
    m1.InverseMatrix();
  } catch (const char* msg) {
    caught = true;
  }
  EXPECT_TRUE(caught);
}
}  // namespace