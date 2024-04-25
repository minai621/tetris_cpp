#include "Matrix.h"
#include "Tetris.h"

Tetris::Tetris(int cy, int cx)
{
  input_screen = new Matrix();
  output_screen = new Matrix();
  current_block = new Matrix();
}

int Tetris::get_num_types()
{
  return 7;
}

int Tetris::accept(char key)
{
  return 0;
}

int Tetris::get_wall_depth()
{
  return 0;
}

Matrix *Tetris::get_output_screen()
{
  return output_screen;
}