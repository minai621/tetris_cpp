#include "Window.h"

mutex mtx;

Window::Window(WINDOW *w) 
{ 
  win = w; 
  getmaxyx(w, nrows, ncols);
  curr_row = 0; 
}

void Window::dowclear() 
{
  mtx.lock(); 
  wclear(win);
  mtx.unlock(); 
}

void Window::doclear() 
{
  mtx.lock(); 
  clear();
  mtx.unlock(); 
}

void Window::addStr(int y, int x, const char *s) 
{
  mtx.lock(); 
  mvwaddstr(win, y, x, s);
  mtx.unlock(); 
}

void Window::addCstr(int y, int x, const char *s, int color) 
{
  mtx.lock(); 
  wattron(win, COLOR_PAIR(color));
  mvwaddstr(win, y, x, s);
  wattroff(win, COLOR_PAIR(color));
  mtx.unlock(); 
}

void Window::dowrefresh() 
{
  mtx.lock();
  wrefresh(win);
  mtx.unlock();
}

void Window::dorefresh() 
{
  mtx.lock();
  refresh();
  mtx.unlock();
}

void Window::printw(string s) 
{
  mtx.lock(); // curr_row is a shared variable!
  if (curr_row >= nrows) {
    curr_row = 0;
    wclear(win);
  }

  int count = 0;
  for (int i = 0; i < s.size(); i++)
    if (s[i] == '\n') count++;

  curr_row += count; 
  wprintw(win, s.c_str()); 
  wrefresh(win); 
  mtx.unlock();
}

