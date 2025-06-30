#include <curses.h>
#ifdef CURKEY2
#include <stdio.h>

#define chtype int

#define defke3(keyname, keyvalue) printf("#define KEY_%s %s\n", keyname, #keyvalue);
#define defke2(keyname, keyvalue) defke3(keyname, keyvalue)
#define defkey(keyname)           defke2(#keyname, KEY_##keyname)

#define defat3(keyname, keyvalue) printf("#define A_%s %s\n", keyname, #keyvalue);
#define defat2(keyname, keyvalue) defat3(keyname, keyvalue)
#define defatt(attname)           defat2(#attname, A_##attname)

int main() {
defkey(ENTER)
defkey(BACKSPACE)
defkey(NEXT)
defkey(UP)
defkey(DOWN)
defkey(RIGHT)
defkey(LEFT)
defkey(PPAGE)
defkey(NPAGE)
defkey(LL)
defkey(END)
defkey(HOME)
defkey(IC)
defkey(DC)
defkey(SUSPEND)
defkey(CANCEL)
defkey(F0)
defkey(F(n))
defatt(NORMAL)
defatt(BOLD)
defatt(DIM)
defatt(UNDERLINE)
return 0;
}
#else
YyY BACKSPACE KEY_BACKSPACE
YyY NEXT KEY_NEXT
YyY UP KEY_UP
YyY DOWN KEY_DOWN
YyY RIGHT KEY_RIGHT
YyY LEFT KEY_LEFT
YyY PPAGE KEY_PPAGE
YyY NPAGE KEY_NPAGE
YyY LL KEY_LL
YyY END KEY_END
YyY HOME KEY_HOME
YyY IC KEY_IC
YyY DC KEY_DC
YyY F0 KEY_F0
YyY F(n) (KEY_F0+(n))
#endif
