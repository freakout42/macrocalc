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

  { COL_DEFAULT,         A_NORMAL,          -1,            -1 },            /* default */
  { COL_BLACK,           A_NORMAL,          COLOR_BLACK,   -1 },            /* black */
  { COL_RED,             A_NORMAL,          COLOR_RED,     -1 },            /* red */
  { COL_GREEN,           A_NORMAL,          COLOR_GREEN,   -1 },            /* green */
  { COL_YELLOW,          A_NORMAL,          COLOR_YELLOW,  -1 },            /* yellow */
  { COL_BLUE,            A_NORMAL,          COLOR_BLUE,    -1 },            /* blue */
  { COL_MAGENTA,         A_NORMAL,          COLOR_MAGENTA, -1 },            /* magenta */
  { COL_CYAN,            A_NORMAL,          COLOR_CYAN,    -1 },            /* cyan */
  { COL_WHITE,           A_NORMAL,          COLOR_WHITE,   -1 },            /* white */

  { TEXTCOLOR,           A_NORMAL,          0,             0 },             /* text cell */
  { EOFCOLOR,            A_NORMAL,          COLOR_GREEN,   -1 },            /* eof cell */
  { ERRORCOLOR,          A_BLINK,           COLOR_RED,     -1 },            /* error cell */
  { VALUECOLOR,          A_NORMAL,          0,             0 },             /* value cell */
  { FORMULACOLOR,        A_NORMAL,          0,             0 },             /* formula cell */
  { STRINGCOLOR,         A_NORMAL,          0,             0 },             /* string cell */
  { BLANKCOLOR,          A_NORMAL,          0,             0 },             /* blank cell */
  { COMMANDCOLOR,        A_UNDERLINE,       COLOR_BLUE,    -1 },            /* command cell */

  { HIGHLIGHTCOLOR,      A_REVERSE,         COLOR_BLUE,    COLOR_WHITE },   /* highlighted current cell */
  { HIGHLIGHTERRORCOLOR, A_REVERSE|A_BLINK, COLOR_RED,     COLOR_WHITE },   /* highlighted current cell with error */
  { HEADERCOLOR,         A_REVERSE,         COLOR_CYAN,    COLOR_BLACK },   /* column and row headers */
  { CURHEADERCOLOR,      A_BOLD,            COLOR_WHITE,   COLOR_BLUE },    /* current col/row header */
  { MARKCOLOR,           A_REVERSE,         COLOR_MAGENTA, COLOR_YELLOW },  /* marked range info */
  { AUTOCALCCOLOR,       A_REVERSE,         0,             0 },             /* autocalc info */
  { FORMDISPLAYCOLOR,    A_REVERSE,         0,             0 },             /* formula display info */
  { MESSAGECOLOR,        A_BOLD|A_BLINK,    0,             0 },             /* messages */
  { PROMPTCOLOR,         A_BOLD,            0,             0 },             /* prompt */
  { INPUTCOLOR,          A_REVERSE,         0,             0 },             /* editor */
  { CELLCONTENTSCOLOR,   A_NORMAL,          0,             0 },             /* cell content info */

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
 if (attrels[pairi].foreg || attrels[pairi].backg) {
  attron(COLOR_PAIR(pairi));
 }
}
}

void uncolor(int pairi) {
if (colors) {
 if (attrels[pairi].foreg || attrels[pairi].backg) {
  attroff(COLOR_PAIR(pairi));
 }
}
}
