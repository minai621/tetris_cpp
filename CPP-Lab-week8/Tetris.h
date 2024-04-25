#include "Matrix.h"

class Tetris
{
private:
    Matrix *input_screen;
    Matrix *output_screen;
    Matrix *current_block;

public:
    Tetris(int cy, int cx);
    int get_num_types();
    int accept(char key);
    int get_wall_depth();
    Matrix *get_output_screen();
};