#pragma once

#include <mutex>
#include <ncursesw/curses.h>

using namespace std;

class Window {
 private:
  //mutex mtx; // should be made as a global variable!!
  WINDOW *win;
  int nrows;
  int ncols;
  int curr_row;
  
 public:
  Window(WINDOW *w);
  void addStr(int y, int x, const char *s); // add a plain string
  void addCstr(int y, int x, const char *s, int color); // add a colored string
  void printw(string s);
  void dorefresh();
  void dowrefresh();
  void doclear();
  void dowclear();
};

