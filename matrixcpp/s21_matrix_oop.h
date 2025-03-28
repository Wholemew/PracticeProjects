#ifndef S21Matrix_OOP
#define S21Matrix_OOP

#include <cstring>
#include <exception>
#include <iostream>

using namespace std;

class S21Matrix {
 private:
  int rows, cols;
  double** matrix;  // Pointer to the memory where the matrix is allocated

 public:
  // Constructors
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other);
  ~S21Matrix();  // Destructor

  // Accessors
  int getRows();
  int getCols();

  // Mutators
  void editRows(int r);
  void editCols(int c);

  // Methods
  bool EqMatrix(const S21Matrix& other);
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix Minor(int r, int c);
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  // Operator overloads
  S21Matrix operator*(S21Matrix x);
  S21Matrix operator+(S21Matrix x);
  S21Matrix operator-(S21Matrix x);
  bool operator==(S21Matrix x);
  S21Matrix operator=(S21Matrix x);
  S21Matrix& operator+=(S21Matrix x);
  S21Matrix& operator-=(S21Matrix x);
  S21Matrix& operator*=(S21Matrix x);
  S21Matrix& operator*=(double x);
  double& operator()(int i, int j);

  friend S21Matrix operator*(double x, S21Matrix m);
  friend S21Matrix operator*(S21Matrix m, double x);
};

S21Matrix operator*(double x, S21Matrix m);
S21Matrix operator*(S21Matrix m, double x);

#endif