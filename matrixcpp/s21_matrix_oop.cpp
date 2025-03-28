#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() {
  rows = 1;
  cols = 1;
  matrix = new double*[1];
  matrix[0] = new double[1];
  matrix[0][0] = 0;
}

S21Matrix::S21Matrix(int rows, int cols) : rows(rows), cols(cols) {
  matrix = new double*[rows];
  for (int i = 0; i < rows; ++i) matrix[i] = new double[cols];
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : rows(other.rows), cols(other.cols) {
  matrix = new double*[rows];
  for (int i = 0; i < rows; ++i) {
    matrix[i] = new double[cols];
    memcpy(matrix[i], other.matrix[i], cols * sizeof(double));
  }
}

S21Matrix::S21Matrix(S21Matrix&& other) : rows(other.rows), cols(other.cols) {
  matrix = new double*[rows];
  for (int i = 0; i < rows; ++i) {
    matrix[i] = new double[cols];
    memcpy(matrix[i], other.matrix[i], cols * sizeof(double));
    delete[] other.matrix[i];
  }
  delete[] other.matrix;
  other.matrix = NULL;
  other.rows = 0;
  other.cols = 0;
}

S21Matrix::~S21Matrix() {
  if (matrix != NULL) {
    for (int i = 0; i < rows; ++i) delete[] matrix[i];
    delete[] matrix;
    matrix = NULL;
  }
  rows = 0;
  cols = 0;
}
bool S21Matrix::EqMatrix(const S21Matrix& other) {
  bool res = true;
  if (rows != other.rows || cols != other.cols)
    res = false;
  else
    for (int i = 0; i < rows; ++i)
      for (int j = 0; j < cols; ++j)
        if (matrix[i][j] / other.matrix[i][j] > 1.0000001 ||
            matrix[i][j] / other.matrix[i][j] < 0.9999999)
          res = false;
  return res;
}

int S21Matrix::getCols() { return cols; }

int S21Matrix::getRows() { return rows; }

void S21Matrix::editCols(int c) {
  double buf[c];
  for (int i = 0; i < c; ++i) buf[i] = 0;
  for (int i = 0; i < rows; ++i) {
    memcpy(buf, matrix[i], min(c, cols) * sizeof(double));
    delete[] matrix[i];
    matrix[i] = new double[c];
    memcpy(matrix[i], buf, c * sizeof(double));
  }
  cols = c;
}

void S21Matrix::editRows(int r) {
  S21Matrix sup;
  sup = this->Transpose();
  sup.editCols(r);
  *this = sup.Transpose();
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows != other.rows || cols != other.cols)
    throw "Different matrix dimensions";
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j) matrix[i][j] += other.matrix[i][j];
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows != other.rows || cols != other.cols)
    throw "Different matrix dimensions";
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j) matrix[i][j] -= other.matrix[i][j];
}

void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j) matrix[i][j] *= num;
}
void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols != other.rows)
    throw "The number of columns of the first matrix is not equal to the number of rows of the second matrix";
  double res[rows][other.cols];
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < other.cols; j++) {
      res[i][j] = 0;
      for (int k = 0; k < cols; ++k)
        res[i][j] += matrix[i][k] * other.matrix[k][j];
    }
  cols = other.cols;
  for (int i = 0; i < rows; ++i) {
    delete[] matrix[i];
    matrix[i] = new double[cols];
    memcpy(matrix[i], res[i], cols * sizeof(double));
  }
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix res(cols, rows);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j) res.matrix[j][i] = matrix[i][j];
  return res;
}

double S21Matrix::Determinant() {
  if (rows != cols) throw "The matrix is not square";
  double res = 0;
  if (rows > 1) {
    for (int i = 0; i < cols; ++i)
      res += (1 - 2 * (i % 2)) * matrix[0][i] * this->Minor(0, i).Determinant();
  } else {
    res = matrix[0][0];
  }
  return res;
}

S21Matrix S21Matrix::Minor(int row, int col) {
  if (row > rows || col > cols) throw "Out of range";
  if (rows < 2 || cols < 2) throw "Empty minor";
  int r, c;
  S21Matrix sup(rows - 1, cols - 1);
  for (int i = 0; i < rows; ++i)
    if (i != row)
      for (int j = 0; j < cols; ++j)
        if (j != col) {
          r = i;
          c = j;
          if (r > row) r--;
          if (c > col) c--;
          sup.matrix[r][c] = matrix[i][j];
        }
  return sup;
}

S21Matrix S21Matrix::CalcComplements() {
  if (rows != cols) throw "The matrix is not square";
  S21Matrix res(rows, cols);
  if (rows == 1)
    res.matrix[0][0] = 1;
  else {
    for (int i = 0; i < rows; ++i)
      for (int j = 0; j < cols; ++j)
        res.matrix[i][j] =
            (1 - 2 * ((i + j) % 2)) * this->Minor(i, j).Determinant();
  }
  return res;
}

S21Matrix S21Matrix::InverseMatrix() {
  if (rows != cols) throw "The matrix is not square";
  double det = this->Determinant();
  if (det < 0.0000001 && det > -0.0000001) throw "Matrix determinant is 0";
  det = 1 / det;
  S21Matrix res(*this);
  res = res.CalcComplements();
  res = res.Transpose();
  res.MulNumber(det);
  return res;
}

S21Matrix S21Matrix::operator*(S21Matrix x) {
  S21Matrix res(*this);
  try {
    res.MulMatrix(x);
  } catch (const char* msg) {
    throw msg;
  }
  return res;
}

S21Matrix S21Matrix::operator+(S21Matrix x) {
  S21Matrix res(*this);
  try {
    res.SumMatrix(x);
  } catch (const char* msg) {
    throw msg;
  }
  return res;
}

S21Matrix S21Matrix::operator-(S21Matrix x) {
  S21Matrix res(*this);
  try {
    res.SubMatrix(x);
  } catch (const char* msg) {
    throw msg;
  }
  return res;
}
bool S21Matrix::operator==(S21Matrix x) { return this->EqMatrix(x); }

S21Matrix S21Matrix::operator=(S21Matrix x) {
  this->~S21Matrix();
  rows = x.rows;
  cols = x.cols;
  matrix = new double*[rows];
  for (int i = 0; i < rows; ++i) {
    matrix[i] = new double[cols];
    memcpy(matrix[i], x.matrix[i], cols * sizeof(double));
  }
  return *this;
}
S21Matrix& S21Matrix::operator+=(S21Matrix x) {
  try {
    this->SumMatrix(x);
  } catch (const char* msg) {
    throw msg;
  }
  return *this;
}
S21Matrix& S21Matrix::operator-=(S21Matrix x) {
  try {
    this->SubMatrix(x);
  } catch (const char* msg) {
    throw msg;
  }
  return *this;
}
S21Matrix& S21Matrix::operator*=(S21Matrix x) {
  try {
    this->MulMatrix(x);
  } catch (const char* msg) {
    throw msg;
  }
  return *this;
}
S21Matrix& S21Matrix::operator*=(double x) {
  this->MulNumber(x);
  return *this;
}
double& S21Matrix::operator()(int i, int j) {
  if (i > rows || j > cols) throw "Index is outside the matrix";
  return matrix[i][j];
}

S21Matrix operator*(double x, S21Matrix m) {
  S21Matrix res(m);
  res.MulNumber(x);
  return res;
}
S21Matrix operator*(S21Matrix m, double x) {
  S21Matrix res(m);
  res.MulNumber(x);
  return res;
}
