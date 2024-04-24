#include <iostream>
#include <string>

using namespace std;

int main1(int argc, char *argv[])
{
  string s1 = "Hello, C++!";
  cout << "s1=" << s1 << endl;
  string s0 = "Hello";
  string s2 = s0 + ", " + "C++" + '!';
  cout << "s2=" << s2 << endl;

  int len1 = s1.length();
  int len2 = s2.length();
  cout << "len1=" << len1 << endl;
  cout << "len2=" << len2 << endl;

  int b1 = s1.compare(s1); // compare s1 and s2 (in content)
  int b2 = s1.compare(s2); // compare s1 and s2 (in content)
  bool b3 = (s1 == s2);    // compare s1 and s2 (in content)

  cout << "s1.compare(s1)=" << b1 << endl;
  cout << "s1.compare(s2)=" << b2 << endl;
  cout << "(s1==s2)=" << b3 << endl;
  return 0;
}

//------------------------------------------------------------------------//
// Patch to Cygwin's bug: stoi, stod, to_string are not supported!
#include <cstdlib> // for strtol, strtod
#include <sstream> // for to_string
namespace patch
{
  template <typename T>
  std::string to_string(const T &n)
  {
    std::ostringstream stm;
    stm << n;
    return stm.str();
  }
}
using namespace patch;
//------------------------------------------------------------------------//

int main2(int argc, char *argv[])
{
  string istr = "1234";
  string dstr = "12.34";

  // int ival = stoi(istr); // string -> integer value
  // double dval = stod(dstr); // string -> double value
  int ival = strtol(istr.c_str(), NULL, 10); // string -> integer value
  double dval = strtod(dstr.c_str(), NULL);  // string -> double value
  cout << "before : " << ival << ", " << dval << endl;

  ival = ival + 1111;
  dval = dval + 11.11;
  string istr2 = to_string(ival); // integer value -> string
  string dstr2 = to_string(dval); // double value -> string

  cout << "after  : " << istr2 << ", " << dstr2 << endl;
  return 0;
}

void printArray(int a[], int len)
{
  if (a != NULL)
  {
    for (int i = 0; i < len; i++)
      cout << a[i] << " ";
  }
  cout << endl;
}

#include <vector>
#include <iterator>
int main3(int argc, char *argv[])
{
  int *A0 = NULL; // int A0[] = NULL;
  int A1[] = {1, 2, 3, 4, 5};
  int *A2 = new int[5];                // int[] A2 = new int[5];
  int *A3 = new int[5]{1, 2, 3, 4, 5}; // int A3[] = new int[]{1, 2, 3, 4, 5};

  cout << "A0:";
  printArray(A0, 0);
  cout << "A1:";
  printArray(A1, 5);
  cout << "A2:";
  printArray(A2, 5);
  cout << "A3:";
  printArray(A3, 5);

  vector<int> v1(A1, A1 + 5);
  vector<int> v3(A3, A3 + 5);

  v1.push_back(10);
  v1.push_back(20);

  for (int idx = 0; idx < v1.size(); idx++)
  {
    cout << "v1[" << idx << "] = " << v1[idx] << endl;
  }

  vector<vector<int>> v2d;
  v2d.push_back(v1);
  v2d.push_back(v3);

  for (int idx1 = 0; idx1 < v2d.size(); idx1++)
  {
    for (int idx2 = 0; idx2 < v2d[idx1].size(); idx2++)
    {
      cout << "v2d[" << idx1 << "][" << idx2 << "] = " << v2d[idx1][idx2] << endl;
    }
  }

  cout << "equal(v1, v3)=" << equal(v1.begin(), v1.end(), v3.begin());
  cout << endl;

  return 0;
}

class Nested
{
public:          // private:
  int dy;        // dynamic variable
  static int dx; // static variable
public:
  Nested(int cy, int cx)
  {
    dy = cy;
    dx = cx;
  }
  int get_dy() { return dy; }
  static int get_dx() { return dx; } // can be declared 'dynamic'
  // class InnerD {
  // public:
  //   int get_dy() { return dy; }
  //   int sum() { return dy+dx; }
  //   int get_dx() { return dx; }
  // };
  class InnerS
  { // An inner class inherently is a static class
    // even without 'static' keyword
  public:
    int get_dx() { return dx; }
  };
};

int Nested::dx = 0;

int main4(int argc, char *argv[])
{
  Nested *m1 = new Nested(1, 2);
  Nested *m2 = new Nested(3, 4);
  cout << "m1->get_dy()=" << m1->get_dy() << ", m1->get_dx()=" << m1->get_dx() << endl;
  cout << "m2->get_dy()=" << m2->get_dy() << ", m2->get_dx()=" << m2->get_dx() << endl;
  // Nested::InnerD *d1 = m1::InnerD();
  // Nested::InnerD *d2 = m2::InnerD();
  Nested::InnerS *s = new Nested::InnerS();
  cout << "s->get_dx()=" << s->get_dx() << endl;

  return 0;
}

#include "Matrix.h"
class MyMatrix : public Matrix
{
public:
  MyMatrix() : Matrix() {}
  MyMatrix(int cy, int cx) : Matrix(cy, cx) {}
  MyMatrix(int *arr, int col, int row) : Matrix(arr, col, row) {}
  void print()
  {
    int dy = get_dy();
    int dx = get_dx();
    int **array = get_array();
    for (int y = 0; y < dy; y++)
    {
      for (int x = 0; x < dx; x++)
      {
        if (array[y][x] == 0)
          cout << "□ ";
        else if (array[y][x] == 1)
          cout << "■ ";
        else
          cout << "X ";
      }
      cout << endl;
    }
  }
};

int main5(int argc, char *argv[])
{
  Matrix *m1 = new Matrix(3, 3);
  m1->print();
  cout << endl;
  int A[] = {0, 1, 0, 1, 1, 1, 0, 0, 0}; // int A[][]
  Matrix *m2 = new Matrix(A, 3, 3);      // new Matrix(A)
  m2->print();
  cout << endl;
  MyMatrix *m3 = new MyMatrix(3, 3);
  m3->print();
  cout << endl;
  MyMatrix *m4 = new MyMatrix(A, 3, 3); // new Matrix(A)
  m4->print();
  cout << endl;
  m2 = m4; // polymorphism: Matrix covers MyMatrix!!
  m2->print();
  cout << endl; // static binding!!

  return 0;
}

#include <string.h>
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    cout << "usage: " << argv[0] << " [function number]" << endl;
    return 0;
  }

  if (strcmp(argv[1], "1") == 0)
    main1(argc, argv);
  else if (strcmp(argv[1], "2") == 0)
    main2(argc, argv);
  else if (strcmp(argv[1], "3") == 0)
    main3(argc, argv);
  else if (strcmp(argv[1], "4") == 0)
    main4(argc, argv);
  else if (strcmp(argv[1], "5") == 0)
    main5(argc, argv);
  else
    cout << "usage: " << argv[0] << " [function number]" << endl;
  return 0;
}
