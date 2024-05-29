#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <termios.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "colors.h"
#include "Matrix.h"
#include "Tetris.h"
#include "CTetris.h"
#include "Window.h"

#include <fstream>

using namespace std;


/********************************************************/
/**************** Linux System Functions *********************/
/********************************************************/

char saved_key = 0;
int tty_raw(int fd);	/* put terminal into a raw mode */
int tty_reset(int fd);	/* restore terminal's mode */
  
/* Read 1 character - echo defines echo mode */
char tty_getch() {
  char ch;
  int n;
  while (1) {
    tty_raw(0);
    n = read(0, &ch, 1);
    tty_reset(0);
    if (n > 0)
      break;
    else if (n < 0) {
      if (errno == EINTR) {
        if (saved_key != 0) {
          ch = saved_key;
          saved_key = 0;
          break;
        }
      }
    }
  }
  return ch;
}

void sigint_handler(int signo) {
  // cout << "SIGINT received!" << endl;
  // do nothing;
}

void sigalrm_handler(int signo) {
  alarm(1);
  saved_key = 's';
}

void registerInterrupt() {
  struct sigaction act, oact;
  act.sa_handler = sigint_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGINT, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
}

void registerAlarm() {
  struct sigaction act, oact;
  act.sa_handler = sigalrm_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGALRM, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
  alarm(1);
}

/**************************************************************/
/**************** Tetris Blocks Definitions *******************/
/**************************************************************/
#define MAX_BLK_TYPES 7
#define MAX_BLK_DEGREES 4

int T0D0[] = { 10, 10, 10, 10, -1 };
int T0D1[] = { 10, 10, 10, 10, -1 };
int T0D2[] = { 10, 10, 10, 10, -1 };
int T0D3[] = { 10, 10, 10, 10, -1 };

int T1D0[] = { 0, 20, 0, 20, 20, 20, 0, 0, 0, -1 };
int T1D1[] = { 0, 20, 0, 0, 20, 20, 0, 20, 0, -1 };
int T1D2[] = { 0, 0, 0, 20, 20, 20, 0, 20, 0, -1 };
int T1D3[] = { 0, 20, 0, 20, 20, 0, 0, 20, 0, -1 };

int T2D0[] = { 30, 0, 0, 30, 30, 30, 0, 0, 0, -1 };
int T2D1[] = { 0, 30, 30, 0, 30, 0, 0, 30, 0, -1 };
int T2D2[] = { 0, 0, 0, 30, 30, 30, 0, 0, 30, -1 };
int T2D3[] = { 0, 30, 0, 0, 30, 0, 30, 30, 0, -1 };

int T3D0[] = { 0, 0, 40, 40, 40, 40, 0, 0, 0, -1 };
int T3D1[] = { 0, 40, 0, 0, 40, 0, 0, 40, 40, -1 };
int T3D2[] = { 0, 0, 0, 40, 40, 40, 40, 0, 0, -1 };
int T3D3[] = { 40, 40, 0, 0, 40, 0, 0, 40, 0, -1 };

int T4D0[] = { 0, 50, 0, 50, 50, 0, 50, 0, 0, -1 };
int T4D1[] = { 50, 50, 0, 0, 50, 50, 0, 0, 0, -1 };
int T4D2[] = { 0, 50, 0, 50, 50, 0, 50, 0, 0, -1 };
int T4D3[] = { 50, 50, 0, 0, 50, 50, 0, 0, 0, -1 };

int T5D0[] = { 0, 60, 0, 0, 60, 60, 0, 0, 60, -1 };
int T5D1[] = { 0, 0, 0, 0, 60, 60, 60, 60, 0, -1 };
int T5D2[] = { 0, 60, 0, 0, 60, 60, 0, 0, 60, -1 };
int T5D3[] = { 0, 0, 0, 0, 60, 60, 60, 60, 0, -1 };

int T6D0[] = { 0, 0, 0, 0, 70, 70, 70, 70, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
int T6D1[] = { 0, 70, 0, 0, 0, 70, 0, 0, 0, 70, 0, 0, 0, 70, 0, 0, -1 };
int T6D2[] = { 0, 0, 0, 0, 70, 70, 70, 70, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
int T6D3[] = { 0, 70, 0, 0, 0, 70, 0, 0, 0, 70, 0, 0, 0, 70, 0, 0, -1 };
  
int *setOfColorBlockArrays[] = {
  T0D0, T0D1, T0D2, T0D3,
  T1D0, T1D1, T1D2, T1D3,
  T2D0, T2D1, T2D2, T2D3,
  T3D0, T3D1, T3D2, T3D3,
  T4D0, T4D1, T4D2, T4D3,
  T5D0, T5D1, T5D2, T5D3,
  T6D0, T6D1, T6D2, T6D3,
};

void drawScreen(Matrix *screen, int wall_depth)
{
  int dy = screen->get_dy();
  int dx = screen->get_dx();
  int dw = wall_depth;
  int **array = screen->get_array();

  for (int y = 0; y < dy - dw + 1; y++) {
    for (int x = dw - 1; x < dx - dw + 1; x++) {
      if (array[y][x] == 0)
	      cout << "□ ";
      else if (array[y][x] == 1)
	      cout << "■ ";
      else if (array[y][x] == 10)
	      cout << "◈ ";
      else if (array[y][x] == 20)
	      cout << "★ ";
      else if (array[y][x] == 30)
	      cout << "● ";
      else if (array[y][x] == 40)
	      cout << "◆ ";
      else if (array[y][x] == 50)
	      cout << "▲ ";
      else if (array[y][x] == 60)
	      cout << "♣ ";
      else if (array[y][x] == 70)
	      cout << "♥ ";
      else
	      cout << "XX ";
    }
    cout << endl;
  }
}


/**************************************************************/
/******************** NCurses-related functions *********************/
/**************************************************************/

void init_screen() {
  setlocale(LC_ALL, ""); // for printing a box character
  initscr();         // initialize the curses screen
  start_color(); // start using colors
  // init_pair(index, fg color, bg color);
  init_pair(1, COLOR_WHITE,   COLOR_BLACK);
  init_pair(2, COLOR_RED,     COLOR_BLACK);
  init_pair(3, COLOR_GREEN,   COLOR_BLACK);
  init_pair(4, COLOR_YELLOW,  COLOR_BLACK);
  init_pair(5, COLOR_BLUE,    COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_CYAN,    COLOR_BLACK);
}

void close_screen() {
  endwin();
}

void drawScreen(Matrix *screen, int wall_depth, Window *win) {
  int dy = screen->get_dy();
  int dx = screen->get_dx();
  int dw = wall_depth;
  int **array = screen->get_array();

  win->dowclear();

  for (int y = 0; y < dy - dw + 1; y++) {
    for (int x = dw - 1; x < dx - dw + 1; x++) {
      if (array[y][x] == 0)
	      win->addStr(y, x - dw + 1, "□");
      else if (array[y][x] == 1)
	      win->addStr(y, x - dw + 1, "■");
      else if (array[y][x] >= 10)
	      win->addCstr(y, x - dw + 1, "■", array[y][x]/10);
      else 
	      win->addStr(y, x - dw + 1, "◈");
    }
  }

  win->dowrefresh();
}

/**************************************************************/
/******************** Tetris Main Loop ************************/
/**************************************************************/

static ifstream infStream;
static ofstream outfStream;

char getTetrisKey(TetrisState state, bool fromUser, bool toFile) {
  char key;

  if (fromUser == true) {
    if (state == TetrisState::NewBlock)
      key = (char) ('0' + rand() % MAX_BLK_TYPES);
    else
      key = tty_getch();
  }
  else { // fromUser == false
    if (infStream.is_open() == false) {
      infStream.open("keyseq.txt");
      if (infStream.fail()) {
        cout << "keyseq.txt cannot be opened!" << endl;
        exit(1);
      }
    }
    if (infStream.eof() == true)
      key = 'q';
    else
      infStream.get(key); // why not "infStream >> key" ?

    usleep(100000); // 100 ms
  }

  if (toFile == true) {
    if (outfStream.is_open() == false) {
      outfStream.open("keyseq.txt");
      if (outfStream.fail()) {
        cout << "keyseq.txt cannot be opened!" << endl;
        exit(1);
      }
      // outfStream.close(); // truncate the existing file
      // outfStream.open("keyseq.txt", ios::app);
    }
    outfStream << key;
  }

  return key;
}

int main(int argc, char *argv[]) {

  string mode_normal = "normal";
  string mode_record = "record";
  string mode_replay = "replay";
  bool fromUser = true;
  bool toFile = false;

  if (argc != 2) {
    cout << "usage: " << argv[0] << " [normal/record/replay]" << endl;
    exit(1);
  }

  if (mode_normal.compare(argv[1]) == 0) {
    fromUser = true;
    toFile = false;
    cout << "normal mode on!" << endl;
  }
  else if (mode_record.compare(argv[1]) == 0) {
    fromUser = true;
    toFile = true;
    cout << "record mode on!" << endl;
  }
  else if (mode_replay.compare(argv[1]) == 0) {
    fromUser = false;
    toFile = false;
    cout << "replay mode on!" << endl;
  }
  else {
    cout << "usage: " << argv[0] << " [normal/record/replay]" << endl;
    exit(1);
  }


  char key;
  registerAlarm(); // register one-second timer
  srand((unsigned int)time(NULL)); // init the random number generator

  init_screen();

  // newwin(nlines, ncolumns, begin_y, begin_x)
  Window bttm_win(newwin(3, 60, 12, 0));
  Window left_win(newwin(12, 30, 0, 0));
  Window rght_win(newwin(12, 30, 0, 30));
  bttm_win.printw("123456789012345678901234567890123456789012345678901234567890");
  bttm_win.printw("Program started!\n");

  TetrisState state;
  CTetris::init(setOfColorBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);
  CTetris *board = new CTetris(10, 10);
  key = getTetrisKey(TetrisState::NewBlock, fromUser, toFile);
  state = board->accept(key);
  drawScreen(board->get_oScreen(), board->get_wallDepth(), &left_win); 
  drawScreen(board->get_oCScreen(), board->get_wallDepth(), &rght_win); 

  while ((key = getTetrisKey(state, fromUser, toFile)) != 'q') {
    state = board->accept(key);
    drawScreen(board->get_oScreen(), board->get_wallDepth(), &left_win); 
    drawScreen(board->get_oCScreen(), board->get_wallDepth(), &rght_win); 
    if (state == TetrisState::NewBlock) {
      key = getTetrisKey(state, fromUser, toFile);
      state = board->accept(key);
      drawScreen(board->get_oScreen(), board->get_wallDepth(), &left_win); 
      drawScreen(board->get_oCScreen(), board->get_wallDepth(), &rght_win); 
      if (state == TetrisState::Finished) 
        break;
    }
  }

  if (infStream.is_open() == true) 
    infStream.close();

  if (outfStream.is_open() == true) 
    outfStream.close();

  bttm_win.printw("Program terminated!\n");
  sleep(5);
  close_screen();

  delete board;
  CTetris::deinit();
  cout << "(nAlloc, nFree) = (" << Matrix::get_nAlloc() << ',' << Matrix::get_nFree() << ")" << endl;  
  cout << "Program terminated!" << endl;

  return 0;
}