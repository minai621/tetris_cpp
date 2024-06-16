#include "Tetris.h"

using namespace std;

///**************************************************************
/// static member variables and functions
///**************************************************************

/// Tetris Operation related
int Tetris::nOps = 0;
TetrisOperation *Tetris::operations[MAX_TETRIS_OPERATIONS];

int Tetris::findOpIdxByKey(char key) {
  for (int id = 0; operations[id] != NULL; id++) {
    if (operations[id]->key == key)
      return id;
    }
  return -1;
}

void Tetris::setOperation(char key, TetrisState s0, ActionHandler *h1,
			  TetrisState s1, ActionHandler *h2, TetrisState s2) {
  int idx = findOpIdxByKey(key);
  if (idx >= 0) {
    delete operations[idx];
    operations[idx] = new TetrisOperation(key, s0, h1, s1, h2, s2);
  }
  else {
    if (nOps == MAX_TETRIS_OPERATIONS) {
      cerr << "Tetris::operations[] is full." << endl;
      return;
    }
    operations[nOps] = new TetrisOperation(key, s0, h1, s1, h2, s2);
    nOps++;
  }
}

void Tetris::setDefaultOperations(void) {
  setOperation('a', TetrisState::Running, new OnLeft(), TetrisState::Running, new OnRight(), TetrisState::Running);
  setOperation('d', TetrisState::Running, new OnRight(), TetrisState::Running, new OnLeft(), TetrisState::Running);
  setOperation('s', TetrisState::Running, new OnDown(), TetrisState::Running, new OnUp(), TetrisState::NewBlock);
  setOperation('w', TetrisState::Running, new OnClockWise(), TetrisState::Running, new OnCounterClockWise(), TetrisState::Running);
  setOperation(' ', TetrisState::Running, new OnDrop(), TetrisState::Running, new OnUp(), TetrisState::NewBlock);
  setOperation('0', TetrisState::NewBlock, new OnNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
  setOperation('1', TetrisState::NewBlock, new OnNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
  setOperation('2', TetrisState::NewBlock, new OnNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
  setOperation('3', TetrisState::NewBlock, new OnNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
  setOperation('4', TetrisState::NewBlock, new OnNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
  setOperation('5', TetrisState::NewBlock, new OnNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
  setOperation('6', TetrisState::NewBlock, new OnNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
}

/// Tetris game related
Matrix *** Tetris::setOfBlockObjects = NULL;
int Tetris::numTypes = 0;
int Tetris::numDegrees = 0;
int Tetris::wallDepth = 0;

void Tetris::init(int **setOfBlockArrays, int nTypes, int nDegrees) {
  if (nOps == 0)
    setDefaultOperations();

  if (setOfBlockObjects != NULL) // already allocated?
    deinit();

  numTypes = nTypes;
  numDegrees = nDegrees;

  setOfBlockObjects = new Matrix**[numTypes]; // allocate 1d array of pointers to Matrix pointer arrays

  for (int t = 0; t < numTypes; t++)
    setOfBlockObjects[t] = new Matrix*[numDegrees]; // allocate 1d array of Matrix pointers

  for (int t = 0; t < numTypes; t++) {
    int *array = setOfBlockArrays[numDegrees * t];
    int idx, size;
    for (idx = 0; array[idx] != -1 ; idx++); // find the element of -1 in array[]
    for (size = 0; size*size < idx; size++); // comupte the square root of idx
    wallDepth = (size > wallDepth ? size : wallDepth);
    for (int d = 0; d < numDegrees; d++) { // allocate matrix objects
      int *array2 = new int[size*size+1];
      int k; 
      for (k = 0; k < size*size; k++)
        array2[k] = (setOfBlockArrays[numDegrees * t + d][k] == 0 ? 0 : 1);
      array2[k] = -1; // end of array
      setOfBlockObjects[t][d] = new Matrix(array2, size, size);
      delete[] array2;
      //cout << *setOfBlockObjects[t][d] << endl;
    }
  }
}

void Tetris::deinit(void) {
  for (int t = 0; t < numTypes; t++)
    for (int d = 0; d < numDegrees; d++)
      delete setOfBlockObjects[t][d]; // deallocate matrix objects

  for (int t = 0; t < numTypes; t++)
    delete [] setOfBlockObjects[t]; // deallocate 1d array of Matrix pointers

  delete []  setOfBlockObjects; // deallocate 1d array of pointers to Matrix pointer arrays

  setOfBlockObjects = NULL;
}


///**************************************************************
/// non-member functions
///**************************************************************

Matrix *deleteFullLines(Matrix *screen, Matrix *blk, int top, int dw) {
  Matrix *line, *bline, *zero, *temp;
  int cy, y;
  int nDeleted, nScanned;
  int ws_dy = screen->get_dy() - 2*dw;
  int ws_dx = screen->get_dx() - 2*dw;

  if (top + blk->get_dy() > ws_dy + dw)
    nScanned = ws_dy + dw - top;
  else
    nScanned = blk->get_dy();
  
  zero = new Matrix(1, ws_dx);
  for (y = nScanned - 1, nDeleted = 0; y >= 0; y--) {
    cy = top + y + nDeleted;
    line = screen->clip(cy, dw, cy+1, dw + ws_dx);
    bline = line->int2bool(); // binary version of line
    delete line;
    if (bline->sum() == ws_dx) {
      temp = screen->clip(dw, dw, cy, dw + ws_dx);
      screen->paste(temp, dw+1, dw);
      screen->paste(zero, dw, dw);
      nDeleted++;
      delete temp;
    }
    delete bline; 
  }
  delete zero;
  return screen;
}

bool anyConflict(Matrix *tempBlk) {
  return tempBlk->anyGreaterThan(1);
}

///**************************************************************
/// dynamic member variables and functions
///**************************************************************

/// constructors and destructor

void deallocArrayScreen(int *array1d) {
  delete[] array1d;
}

int *allocArrayScreen(int dy, int dx, int dw) {
  int **array2d = new int*[dy + 2*dw];
  int y, x;

  for (y = 0; y < dy+2*dw; y++) // alloc array2d
    array2d[y] = new int[dx + 2*dw];
  
  for (y = 0; y < dy+2*dw; y++) {
    for (x = 0; x < dw; x++) {
      array2d[y][x] = 1; // left wall
      array2d[y][dw+dx+x] = 1; // right wall
    }
    if (y >= dw && y < dy + dw) {
      for (x = 0; x < dx; x++)
        array2d[y][dw+x] = 0; // empty space
    }
    else {
      for (x = 0; x < dx; x++)
        array2d[y][dw+x] = 1; // bottom ground
    }
  }

  int *array1d = new int[(dy+2*dw)*(dx+2*dw)]; // alloc array2d
  for (y = 0; y < dy+2*dw; y++)
    for (x = 0; x < dx+2*dw; x++)
      array1d[(dx+2*dw)*y + x] = array2d[y][x]; // copy 2d to 1d

  for (int y = 0; y < dy+2*dw; y++) // dealloc array2d
    delete [] array2d[y];
  delete [] array2d;

  return array1d;  
}

Tetris::Tetris(int cy, int cx) {
  rows = cy + 2*wallDepth; 
  cols = cx + 2*wallDepth; 
  type = -1; // unknown as of now
  degree = 0;
  top = wallDepth;
  left = wallDepth + cols/2 - wallDepth/2; // wallDepth equals the size of the largest block.

  int *arrayScreen = allocArrayScreen(cy, cx, wallDepth);
  iScreen = new Matrix(arrayScreen, rows, cols);
  oScreen = new Matrix(iScreen);
  currBlk = NULL;
  state = TetrisState::NewBlock;

  //cout << *iScreen << endl;
  deallocArrayScreen(arrayScreen);
}

Tetris::~Tetris() {
  delete iScreen;
  delete oScreen;
}

/// accessors
Matrix *Tetris::overlap_currBlk(void) {
  Matrix *tBlk1, *tBlk2;
  tBlk1 = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
  tBlk2 = tBlk1->add(currBlk);
  delete tBlk1;
  return tBlk2;
}

/// mutators
void Tetris::update_oScreen(Matrix *tempBlk, int y, int x) {
  oScreen->paste(iScreen, 0, 0);
  oScreen->paste(tempBlk, y, x);
}

TetrisState Tetris::accept(char key) {
  int idx = findOpIdxByKey(key);
  if (idx == -1) {
    cout << "unknown key! (int=" << (int) key << ")" << endl;
    return state;
  }
  TetrisOperation *op = operations[idx];
  if (state != op->preState) {
    cout << "wrong preState for the current key!" << endl;
    return state;
  }
  op->hAction->run(this, key);
  Matrix *tempBlk = overlap_currBlk();
  if (anyConflict(tempBlk) == false) {
    state = op->postAState;
  }
  else {
    op->hCounterAction->run(this, key); 
    delete tempBlk;
    tempBlk = overlap_currBlk();
    state = op->postCState;
  }
  update_oScreen(tempBlk, top, left);
  delete tempBlk;
  return state;
}
