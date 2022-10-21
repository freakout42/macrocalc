static void tr(char *s) {
char * p = s;
int l = strlen(p);

while(isspace(p[l - 1])) p[--l] = 0;
while(* p && isspace(* p)) ++p, --l;
memmove(s, p, l + 1);
}

static void lc(char *p) {
  for ( ; isalnum(*p)||*p=='_'; ++p) *p = tolower(*p);
}

static void uc(char *p) {
  for ( ; isalnum(*p)||*p=='_'; ++p) *p = toupper(*p);
}

static void uu(char *s)
{
char *p = s;

char code[3] = {0};
unsigned long ascii = 0;
char *end = NULL;

while (*s) {
  if (*s == '%') {
    memcpy(code, ++s, 2);
    ascii = strtoul(code, &end, 16);
    *p++ = (char)ascii;
    s += 2;
  } else *p++ = *s++;
}
*p = '\0';
}
