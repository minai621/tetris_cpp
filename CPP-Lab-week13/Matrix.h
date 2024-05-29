#pragma once
#include <iostream>
#include <cstdlib>

using namespace std;

class Matrix {
private:
  static int nAlloc;
  static int nFree;
  int dy;
  int dx;
  int **array;
  void alloc(int cy, int cx);
  void dealloc();
public:
  static int get_nAlloc();
  static int get_nFree();
  int get_dy() const;
  int get_dx() const;
  int** get_array() const;
  Matrix();
  Matrix(int cy, int cx);
  Matrix(const Matrix *obj);
  Matrix(const Matrix &obj);
  Matrix(int *arr, int col, int row);
  ~Matrix();
  Matrix *clip(int top, int left, int bottom, int right);
  Matrix clip_(int top, int left, int bottom, int right);
  void paste(const Matrix *obj, int top, int left);
  void paste(const Matrix &obj, int top, int left);
  Matrix *add(const Matrix *obj);
  friend const Matrix operator+(const Matrix& m1, const Matrix& m2);
  //const Matrix operator+(const Matrix& m2) const;
  int sum();
  void mulc(int coef);
  Matrix *int2bool();
  bool anyGreaterThan(int val);
  void print();
  friend ostream& operator<<(ostream& out, const Matrix& obj);
  Matrix& operator=(const Matrix& obj);
};
