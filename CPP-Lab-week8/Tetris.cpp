#include "Matrix.h"
#include "Tetris.h"

int *allocArrayScreen(int dy, int dx, int dw)
{
  int **array2d = new int *[dy + dw];
  int y, x;

  for (y = 0; y < dy + dw; y++) // alloc array2d
    array2d[y] = new int[dx + 2 * dw];

  for (y = 0; y < dy + dw; y++)
  {
    for (x = 0; x < dw; x++)
    {
      array2d[y][x] = 1;           // left wall
      array2d[y][dw + dx + x] = 1; // right wall
    }
    if (y < dy)
    {
      for (x = 0; x < dx; x++)
        array2d[y][dw + x] = 0; // empty space
    }
    else
    {
      for (x = 0; x < dx; x++)
        array2d[y][dw + x] = 1; // bottom ground
    }
  }

  int *array1d = new int[(dy + dw) * (dx + 2 * dw)]; // alloc array2d
  for (y = 0; y < dy + dw; y++)
    for (x = 0; x < dx + 2 * dw; x++)
      array1d[(dx + 2 * dw) * y + x] = array2d[y][x]; // copy 2d to 1d

  for (int y = 0; y < dy + dw; y++) // dealloc array2d
    delete[] array2d[y];
  delete[] array2d;

  return array1d;
}

Tetris::Tetris(int cy, int cx)
{
  wall_depth = 4;
  int rows = cy + wall_depth;
  int cols = cx + 2 * wall_depth;

  int *arrayScreen = allocArrayScreen(cy, cx, wall_depth);
  input_screen = new Matrix(arrayScreen, rows, cols);
  output_screen = new Matrix(input_screen);
  current_block = new Matrix(3, 3);
}

int Tetris::get_num_types()
{
  return 7;
}

int Tetris::accept(char key)
{
  return 0;
}

// 벽의 두께를 정의한다.
int Tetris::get_wall_depth()
{
  return wall_depth;
}

Matrix *Tetris::get_output_screen()
{
  return output_screen;
}