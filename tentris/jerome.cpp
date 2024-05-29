class OnFix : public ActionHandler
{
public:
    void run(Tetris *t, char key) {}
};
Matrix *myDeleteFullLines(Matrix *screen, Matrix *blk, int top, int dw)
{
    Matrix *line, *bline, *temp;
    int cy, y;
    int nDeleted, nScanned;
    int ws_dy = screen->get_dy() - 2 * dw;
    int ws_dx = screen->get_dx() - 2 * dw;
    Matrix *zeroRow = new Matrix(1, ws_dx);
    Matrix *zeroCol = new Matrix(ws_dy, 1);
    vector<int> deletedRows;
    vector<int> deletedCols;
    for (int row = dw; row < ws_dy + dw; row++)
    {
        line = screen->clip(row, dw, row + 1, dw + ws_dx);
        bline = line->int2bool();
        delete line;
        if (bline->sum() == ws_dx)
            deletedRows.push_back(row);
        delete bline;
    }
    for (int col = dw; col < ws_dx + dw; col++)
    {
        line = screen->clip(dw, col, dw + ws_dy, col + 1);
        bline = line->int2bool();
        delete line;
        if (bline->sum() == ws_dy)
            deletedCols.push_back(col);
        delete bline;
    }
    for (int row : deletedRows)
    {
        screen->paste(zeroRow, row, dw);
    }
    for (int col : deletedCols)
    {
        screen->paste(zeroCol, dw, col);
    }
    delete zeroRow;
    delete zeroCol;
    return screen;
}
class OnMyNewBlock : public ActionHandler
{
public:
    void run(Tetris *t, char key)
    {
        if (t->currBlk != NULL) // why test currBlk != NULL?
            t->oScreen = myDeleteFullLines(t->oScreen, t->currBlk, t->top, t->wallDepth);
        t->iScreen->paste(t->oScreen, 0, 0);
        // select a new block
        t->type = key - '0';
        t->degree = 0;
        t->top = t->wallDepth;
        t->left = t->cols / 2 - t->wallDepth / 2;
        t->currBlk = t->setOfBlockObjects[t->type][t->degree];
        return;
    }
};
class OnAction : public ActionHandler
{
private:
    ActionHandler *nextAction;

public:
    OnAction(ActionHandler *nextAction)
    {
        this->nextAction = nextAction;
    }
    ~OnAction()
    {
        delete nextAction;
    }
    void run(Tetris *t, char key)
    {
        Matrix *temp = new Matrix(t->iScreen);
        Matrix *tempBlk = t->overlap_currBlk();
        temp->paste(tempBlk, t->top, t->left);
        delete tempBlk;
        int dw = t->get_wallDepth();
        int ws_dy = temp->get_dy() - 2 * dw;
        int ws_dx = temp->get_dx() - 2 * dw;
        Matrix *zero = new Matrix(ws_dy, ws_dx);
        temp->paste(zero, dw, dw);
        if (anyConflict(temp))
            nextAction->run(t, key);
        delete zero;
        delete temp;
    }
};

/**************************************************************/
/******************** Tetris Main Loop ************************/
/**************************************************************/
int main(int argc, char *argv[])
{
    char key;
    // registerAlarm(); // register one-second timer
    srand((unsigned int)time(NULL)); // init the random number generator

    TetrisState state;
    Tetris::init(setOfBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);

    /////////////////////////////////////////////////////////////////////////
    /// Plug-in architecture for generalized Tetris class
    /////////////////////////////////////////////////////////////////////////
    Tetris::setOperation('a', TetrisState::Running, new OnLeft(), TetrisState::Running, new OnAction(new OnRight()), TetrisState::Running);
    Tetris::setOperation('d', TetrisState::Running, new OnRight(), TetrisState::Running, new OnAction(new OnLeft()), TetrisState::Running);
    Tetris::setOperation('s', TetrisState::Running, new OnDown(), TetrisState::Running, new OnAction(new OnUp()), TetrisState::Running);
    Tetris::setOperation('e', TetrisState::Running, new OnUp(), TetrisState::Running, new OnAction(new OnDown()), TetrisState::Running);
    Tetris::setOperation('w', TetrisState::Running, new OnClockWise(), TetrisState::Running, new OnAction(new OnCounterClockWise()), TetrisState::Running);
    Tetris::setOperation(' ', TetrisState::Running, new OnFix(), TetrisState::NewBlock, new OnFix(), TetrisState::Running);
    Tetris::setOperation('0', TetrisState::NewBlock, new OnMyNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
    Tetris::setOperation('1', TetrisState::NewBlock, new OnMyNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
    Tetris::setOperation('2', TetrisState::NewBlock, new OnMyNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
    Tetris::setOperation('3', TetrisState::NewBlock, new OnMyNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
    Tetris::setOperation('4', TetrisState::NewBlock, new OnMyNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
    Tetris::setOperation('5', TetrisState::NewBlock, new OnMyNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);
    Tetris::setOperation('6', TetrisState::NewBlock, new OnMyNewBlock(), TetrisState::Running, new OnFinished(), TetrisState::Finished);