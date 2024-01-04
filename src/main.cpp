#include <ncurses.h>
#include "ide.hpp"


/*
  how to prove correctness of the text editor:
    a) write normal text
    b) check the right behaviour of the cursor
    c) check the right behaviour of the backspace
      c1) check the right behaviour of the backspace when the cursor is at the beginning of the row
      c2) check the right behaviour of the backspace when the cursor is in the row
    d) check the right behaviour of the enter
      d1) check the right behaviour of the enter when the cursor is at the beginning of the row
      d2) check the right behaviour of the enter when the cursor is in the row
    e) check the right behaviour of the tab
    f) check the right behaviour of the arrow keys
*/
int main(void) {
  Ide ide;
  ide.open();
  endwin();
}