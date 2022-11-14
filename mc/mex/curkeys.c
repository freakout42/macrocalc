#include <stdio.h>
#include <curses.h>

#define defke3(keyname, keyvalue) printf("#define KEY_%s %s\n", keyname, #keyvalue);
#define defke2(keyname, keyvalue) defke3(keyname, keyvalue)
#define defkey(keyname)           defke2(#keyname, KEY_##keyname)

int main() {
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
defkey(F0)
defkey(F(n))
return 0;
}
