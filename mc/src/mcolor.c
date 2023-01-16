/* mcolor.c */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <arx_def.h>
#include <cur_def.h>
#include "mc.h"
#include "mcolor.h"

typedef struct attrel {
  int ccode;
  int cattr;
  int foreg;
  int backg;
} attrel;

static attrel attrels[] = {
  { COL_DEFAULT,         A_NORMAL,          -1,            -1 },
  { COL_BLACK,           A_NORMAL,          COLOR_BLACK,   -1 },          /* coloured */
  { COL_RED,             A_NORMAL,          COLOR_RED,     -1 },
  { COL_GREEN,           A_NORMAL,          COLOR_GREEN,   -1 },
  { COL_YELLOW,          A_NORMAL,          COLOR_YELLOW,  -1 },
  { COL_BLUE,            A_NORMAL,          COLOR_BLUE,    -1 },
  { COL_MAGENTA,         A_NORMAL,          COLOR_MAGENTA, -1 },
  { COL_CYAN,            A_NORMAL,          COLOR_CYAN,    -1 },
  { COL_WHITE,           A_NORMAL,          COLOR_WHITE,   -1 },
  { TEXTCOLOR,           A_NORMAL,          0,             0 },           /* cellstr */
  { EOFCOLOR,            A_BOLD,            0,             0 },
  { ERRORCOLOR,          A_BLINK,           COLOR_RED,     COLOR_WHITE },
  { VALUECOLOR,          A_NORMAL,          0,             0 },
  { FORMULACOLOR,        A_NORMAL,          0,             0 },
  { STRINGCOLOR,         A_NORMAL,          0,             0 },
  { BLANKCOLOR,          A_NORMAL,          0,             0 },
  { COMMANDCOLOR,        A_UNDERLINE,       0,             0 },
  { HIGHLIGHTCOLOR,      A_REVERSE,         COLOR_CYAN,    COLOR_BLACK },
  { HIGHLIGHTERRORCOLOR, A_REVERSE|A_BLINK, COLOR_RED,     COLOR_WHITE },
  { HEADERCOLOR,         A_REVERSE,         COLOR_CYAN,    COLOR_BLACK }, /* decoration */
  { CURHEADERCOLOR,      A_BOLD,            COLOR_WHITE,   COLOR_BLUE },
  { MARKCOLOR,           A_REVERSE,         0,             0 },
  { AUTOCALCCOLOR,       A_REVERSE,         0,             0 },
  { FORMDISPLAYCOLOR,    A_REVERSE,         0,             0 },
  { MESSAGECOLOR,        A_BOLD|A_BLINK,    0,             0 },
  { PROMPTCOLOR,         A_BOLD,            0,             0 },
  { INPUTCOLOR,          A_REVERSE,         0,             0 },
  { CELLCONTENTSCOLOR,   A_NORMAL,          0,             0 },
 /* end */
  { COL_UNDEF, 0, 0, 0 }
};

int mcode2att(int colcode) {
return attrels[colcode].cattr;
}

void mcolor0(void) {
int i;

start_color();
use_default_colors();
for (i=0; i<COL_UNDEF; i++) {
  assert(attrels[i].ccode == i);
  init_pair(i, attrels[i].foreg, attrels[i].backg);
}
}

void setcolor(int pairi) {
if (colors) {
 if (attrels[pairi].foreg + attrels[pairi].backg) {
  attron(COLOR_PAIR(pairi));
 }
}
}

void uncolor(int pairi) {
if (colors) {
 if (attrels[pairi].foreg + attrels[pairi].backg) {
  attroff(COLOR_PAIR(pairi));
 }
}
}
