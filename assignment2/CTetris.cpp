#include "CTetris.h"

using namespace std;

///**************************************************************
/// static member variables and functions
///**************************************************************

Matrix ***CTetris::setOfColorBlockObjects = NULL;

void CTetris::init(int **setOfColorBlockArrays, int nTypes, int nDegrees)
{
  if (setOfColorBlockObjects != NULL) // already allocated?
    deinit();

  Tetris::init(setOfColorBlockArrays, nTypes, nDegrees); // call superclass' function

  // write the rest of this function!!

  // Allocate memory for setOfColorBlockObjects
  setOfColorBlockObjects = new Matrix **[nTypes];
  for (int i = 0; i < nTypes; i++)
  {
    setOfColorBlockObjects[i] = new Matrix *[nDegrees];
  }

  for (int y = 0; y < numTypes; y++)
  {
    setOfColorBlockObjects[y] = new Matrix *[numDegrees];
    for (int x = 0; x < numDegrees; x++)
    {
      setOfColorBlockObjects[y][x] = new Matrix(setOfBlockObjects[y][x]);
      setOfColorBlockObjects[y][x]->mulc(10 * (y + 1));
    }
  }
}

void CTetris::deinit(void)
{
  Tetris::deinit(); // call superclass' function

  for (int t = 0; t < numTypes; t++)
    for (int d = 0; d < numDegrees; d++)
      delete setOfColorBlockObjects[t][d]; // deallocate matrix objects

  for (int t = 0; t < numTypes; t++)
    delete[] setOfColorBlockObjects[t]; // deallocate 1d array of Matrix pointers

  delete[] setOfColorBlockObjects; // deallocate 1d array of pointers to Matrix pointer arrays

  setOfColorBlockObjects = NULL;
}

///**************************************************************
/// dynamic member variables and functions
///**************************************************************

/// constructors and destructor

CTetris::CTetris(int cy, int cx) : Tetris(cy, cx)
{
  iCScreen = new Matrix(iScreen);
  oCScreen = new Matrix(oScreen);
  currCBlk = NULL;
}

CTetris::~CTetris()
{
  delete iCScreen;
  delete oCScreen;
}

/// mutators
TetrisState CTetris::accept(char key)
{
  TetrisState _state = state;
  _state = Tetris::accept(key);

  if (_state == TetrisState::Finished)
  {
    return _state;
  }
  else
  {
    Matrix *tmpCBlk1, *tmpCBlk2;
    currCBlk = setOfColorBlockObjects[type][degree];
    tmpCBlk1 = iCScreen->clip(top, left, top + currCBlk->get_dy(), left + currCBlk->get_dx());
    tmpCBlk2 = tmpCBlk1->add(currCBlk);
    oCScreen->paste(iCScreen, 0, 0);
    oCScreen->paste(tmpCBlk2, top, left);
    delete tmpCBlk1;
    delete tmpCBlk2;
  }
  if (_state == TetrisState::NewBlock)
  {
    oCScreen = deleteFullLines(oCScreen, currCBlk, top, wallDepth);
    iCScreen->paste(oCScreen, 0, 0);
  }

  return _state;
}
