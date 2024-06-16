#include <iostream>
#include <string>
#include "Matrix_e.h"

using namespace std;

int main1(int argc, char *argv[]) {
  Matrix *m[1000] = { new Matrix(10,10), };
  for (int i=1; i<1000; i++)
    m[i] = new Matrix(10, 10);

  for (int i=0; i<999; i++)
    delete m[i];

  cout << "nAlloc=" << m[1000]->get_nAlloc() << endl;
  cout << "nFree=" << m[1000]->get_nFree() << endl;
  return 0;
}

int main2(int argc, char *argv[]) { // why no "throws Exception"???
  int arrayBlk[3][3] = {
    { 0, 1, 0 },
    { 1, 1, 1 },
    { 0, 0, 0 }
  };
  try {
    Matrix *currBlk = new Matrix((int *) arrayBlk, 3, 3);
    Matrix *tempBlk = new Matrix(5,5);
    //Matrix *tempBlk = new Matrix(-1,-1); // falls into the second catch
    Matrix *tempBlk2;
    tempBlk2 = tempBlk->add(currBlk); // falls into the first catch
    delete tempBlk;
    tempBlk2->print();
  } catch(MismatchedMatrixException& e) {
    cout << "at first catch: " << e.getMessage() << endl;
  } catch(MatrixException& e) {
    cout << "at second catch " << e.getMessage() << endl;
  }
  return 0;
}

void drawMatrix(Matrix *m) {
  int dy = m->get_dy();
  int dx = m->get_dx();
  int **array = m->get_array();
  for (int y=0; y < dy; y++) {
    for (int x=0; x < dx; x++) {
      if (array[y][x] == 0) cout << "□ ";
      else if (array[y][x] == 1) cout << "■ ";
      else cout << "X ";
    }
    cout << endl;
  }
}

int main3(int argc, char *argv[]) {
  int arrayScreen[6][12] = {
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  };
  int arrayBlk[3][3] = {
    { 0, 1, 0 },
    { 1, 1, 1 },
    { 0, 0, 0 },
  };
  Matrix *oScreen = new Matrix((int *) arrayScreen, 6, 12);
  cout << "oScreen:" << endl;
  drawMatrix(oScreen); cout << endl;

  Matrix *currBlk = new Matrix((int *) arrayBlk, 3, 3);
  cout << "currBlk:" << endl;
  drawMatrix(currBlk); cout << endl;

  int top = 0;
  int left = 4;
  Matrix *tempBlk = oScreen->clip(top, left, top+currBlk->get_dy(), left+currBlk->get_dx());
  cout << "tempBlk (after clip):" << endl;
  drawMatrix(tempBlk); cout << endl;

  Matrix *tempBlk2 = tempBlk->add(currBlk); delete tempBlk;
  cout << "tempBlk (after add):" << endl;
  drawMatrix(tempBlk2); cout << endl;

  oScreen->paste(tempBlk2, top, left);
  cout << "oScreen (after paste):" << endl;
  drawMatrix(oScreen); cout << endl;

  cout << "currBlk->sum()=" << currBlk->sum() << endl;
  cout << endl;

  tempBlk2->mulc(2);
  cout << "tempBlk (after mulc):" << endl;
  tempBlk2->print(); cout << endl;

  cout << "currBlk->anyGreaterThan(1)=" << currBlk->anyGreaterThan(1) << endl;
  cout << "tempBlk->anyGreaterThan(1)=" << tempBlk->anyGreaterThan(1) << endl;

  return 0;
}

#include <string.h>
int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " [function number]" << endl;
    return 0;
  }
    
  if (strcmp(argv[1], "1") == 0) main1(argc, argv);
  else if (strcmp(argv[1], "2") == 0) main2(argc, argv);
  else if (strcmp(argv[1], "3") == 0) main3(argc, argv);
  else
    cout << "usage: " << argv[0] << " [function number]" << endl;
  return 0;
}


  
