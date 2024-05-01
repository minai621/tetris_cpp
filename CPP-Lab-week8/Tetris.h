#include "Matrix.h"

class Tetris
{
private:
    Matrix *input_screen;
    Matrix *output_screen;
    Matrix *current_block;
    int wall_depth;

public:
    // contructors
    Tetris(int cy, int cx);

    // methods
    int get_num_types();
    int accept(char key);
    int get_wall_depth();
    Matrix *get_output_screen();
};