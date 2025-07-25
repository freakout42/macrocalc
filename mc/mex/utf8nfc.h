extern int onlyutf8;
static struct dmap { char basech; int nonsp; unsigned char iso15; int utf16; unsigned char iso152; } diacry[] = {
{ 'A', 0x0300, 0xC0, 0x00C0, 'À' }, /* LATIN CAPITAL LETTER A GRAVE;Lu;0;L;0041 0300;;;;N;;;;00E0; */
{ 'A', 0x0301, 0xC1, 0x00C1, 'Á' }, /* LATIN CAPITAL LETTER A ACUTE;Lu;0;L;0041 0301;;;;N;;;;00E1; */
{ 'A', 0x0302, 0xC2, 0x00C2, 'Â' }, /* LATIN CAPITAL LETTER A CIRCUMFLEX;Lu;0;L;0041 0302;;;;N;;;;00E2; */
{ 'A', 0x0303, 0xC3, 0x00C3, 'Ã' }, /* LATIN CAPITAL LETTER A TILDE;Lu;0;L;0041 0303;;;;N;;;;00E3; */
{ 'A', 0x0308, 0xC4, 0x00C4, 'Ä' }, /* LATIN CAPITAL LETTER A DIAERESIS;Lu;0;L;0041 0308;;;;N;;;;00E4; */
{ 'A', 0x030A, 0xC5, 0x00C5, 'Å' }, /* LATIN CAPITAL LETTER A RING;Lu;0;L;0041 030A;;;;N;;;;00E5; */
{ 'C', 0x0327, 0xC7, 0x00C7, 'Ç' }, /* LATIN CAPITAL LETTER C CEDILLA;Lu;0;L;0043 0327;;;;N;;;;00E7; */
{ 'E', 0x0300, 0xC8, 0x00C8, 'È' }, /* LATIN CAPITAL LETTER E GRAVE;Lu;0;L;0045 0300;;;;N;;;;00E8; */
{ 'E', 0x0301, 0xC9, 0x00C9, 'É' }, /* LATIN CAPITAL LETTER E ACUTE;Lu;0;L;0045 0301;;;;N;;;;00E9; */
{ 'E', 0x0302, 0xCA, 0x00CA, 'Ê' }, /* LATIN CAPITAL LETTER E CIRCUMFLEX;Lu;0;L;0045 0302;;;;N;;;;00EA; */
{ 'E', 0x0308, 0xCB, 0x00CB, 'Ë' }, /* LATIN CAPITAL LETTER E DIAERESIS;Lu;0;L;0045 0308;;;;N;;;;00EB; */
{ 'I', 0x0300, 0xCC, 0x00CC, 'Ì' }, /* LATIN CAPITAL LETTER I GRAVE;Lu;0;L;0049 0300;;;;N;;;;00EC; */
{ 'I', 0x0301, 0xCD, 0x00CD, 'Í' }, /* LATIN CAPITAL LETTER I ACUTE;Lu;0;L;0049 0301;;;;N;;;;00ED; */
{ 'I', 0x0302, 0xCE, 0x00CE, 'Î' }, /* LATIN CAPITAL LETTER I CIRCUMFLEX;Lu;0;L;0049 0302;;;;N;;;;00EE; */
{ 'I', 0x0308, 0xCF, 0x00CF, 'Ï' }, /* LATIN CAPITAL LETTER I DIAERESIS;Lu;0;L;0049 0308;;;;N;;;;00EF; */
{ 'N', 0x0303, 0xD1, 0x00D1, 'Ñ' }, /* LATIN CAPITAL LETTER N TILDE;Lu;0;L;004E 0303;;;;N;;;;00F1; */
{ 'O', 0x0300, 0xD2, 0x00D2, 'Ò' }, /* LATIN CAPITAL LETTER O GRAVE;Lu;0;L;004F 0300;;;;N;;;;00F2; */
{ 'O', 0x0301, 0xD3, 0x00D3, 'Ó' }, /* LATIN CAPITAL LETTER O ACUTE;Lu;0;L;004F 0301;;;;N;;;;00F3; */
{ 'O', 0x0302, 0xD4, 0x00D4, 'Ô' }, /* LATIN CAPITAL LETTER O CIRCUMFLEX;Lu;0;L;004F 0302;;;;N;;;;00F4; */
{ 'O', 0x0303, 0xD5, 0x00D5, 'Õ' }, /* LATIN CAPITAL LETTER O TILDE;Lu;0;L;004F 0303;;;;N;;;;00F5; */
{ 'O', 0x0308, 0xD6, 0x00D6, 'Ö' }, /* LATIN CAPITAL LETTER O DIAERESIS;Lu;0;L;004F 0308;;;;N;;;;00F6; */
{ 'O', 0x0338, 0xD8, 0x00D8, 'Ø' }, /* LATIN CAPITAL LETTER O SLASH;Lu;0;L;004F 0338;;;;N;;;;00F8; */
{ 'S', 0x030C, 0xA6, 0x0160, '¦' }, /* LATIN CAPITAL LETTER S HACEK;Lu;0;L;0053 030C;;;;N;;;;0161; */
{ 'U', 0x0300, 0xD9, 0x00D9, 'Ù' }, /* LATIN CAPITAL LETTER U GRAVE;Lu;0;L;0055 0300;;;;N;;;;00F9; */
{ 'U', 0x0301, 0xDA, 0x00DA, 'Ú' }, /* LATIN CAPITAL LETTER U ACUTE;Lu;0;L;0055 0301;;;;N;;;;00FA; */
{ 'U', 0x0302, 0xDB, 0x00DB, 'Û' }, /* LATIN CAPITAL LETTER U CIRCUMFLEX;Lu;0;L;0055 0302;;;;N;;;;00FB; */
{ 'U', 0x0308, 0xDC, 0x00DC, 'Ü' }, /* LATIN CAPITAL LETTER U DIAERESIS;Lu;0;L;0055 0308;;;;N;;;;00FC; */
{ 'Y', 0x0301, 0xDD, 0x00DD, 'Ý' }, /* LATIN CAPITAL LETTER Y ACUTE;Lu;0;L;0059 0301;;;;N;;;;00FD; */
{ 'Y', 0x0308, 0xBE, 0x0178, '¾' }, /* LATIN CAPITAL LETTER Y DIAERESIS;Lu;0;L;0059 0308;;;;N;;;;00FF; */
{ 'Z', 0x030C, 0xB4, 0x017D, '´' }, /* LATIN CAPITAL LETTER Z HACEK;Lu;0;L;005A 030C;;;;N;;;;017E; */
{ 'a', 0x0300, 0xE0, 0x00E0, 'à' }, /* LATIN SMALL LETTER A GRAVE;Ll;0;L;0061 0300;;;;N;;;00C0;;00C0 */
{ 'a', 0x0301, 0xE1, 0x00E1, 'á' }, /* LATIN SMALL LETTER A ACUTE;Ll;0;L;0061 0301;;;;N;;;00C1;;00C1 */
{ 'a', 0x0302, 0xE2, 0x00E2, 'â' }, /* LATIN SMALL LETTER A CIRCUMFLEX;Ll;0;L;0061 0302;;;;N;;;00C2;;00C2 */
{ 'a', 0x0303, 0xE3, 0x00E3, 'ã' }, /* LATIN SMALL LETTER A TILDE;Ll;0;L;0061 0303;;;;N;;;00C3;;00C3 */
{ 'a', 0x0308, 0xE4, 0x00E4, 'ä' }, /* LATIN SMALL LETTER A DIAERESIS;Ll;0;L;0061 0308;;;;N;;;00C4;;00C4 */
{ 'a', 0x030A, 0xE5, 0x00E5, 'å' }, /* LATIN SMALL LETTER A RING;Ll;0;L;0061 030A;;;;N;;;00C5;;00C5 */
{ 'c', 0x0327, 0xE7, 0x00E7, 'ç' }, /* LATIN SMALL LETTER C CEDILLA;Ll;0;L;0063 0327;;;;N;;;00C7;;00C7 */
{ 'e', 0x0300, 0xE8, 0x00E8, 'è' }, /* LATIN SMALL LETTER E GRAVE;Ll;0;L;0065 0300;;;;N;;;00C8;;00C8 */
{ 'e', 0x0301, 0xE9, 0x00E9, 'é' }, /* LATIN SMALL LETTER E ACUTE;Ll;0;L;0065 0301;;;;N;;;00C9;;00C9 */
{ 'e', 0x0302, 0xEA, 0x00EA, 'ê' }, /* LATIN SMALL LETTER E CIRCUMFLEX;Ll;0;L;0065 0302;;;;N;;;00CA;;00CA */
{ 'e', 0x0308, 0xEB, 0x00EB, 'ë' }, /* LATIN SMALL LETTER E DIAERESIS;Ll;0;L;0065 0308;;;;N;;;00CB;;00CB */
{ 'i', 0x0300, 0xEC, 0x00EC, 'ì' }, /* LATIN SMALL LETTER I GRAVE;Ll;0;L;0069 0300;;;;N;;;00CC;;00CC */
{ 'i', 0x0301, 0xED, 0x00ED, 'í' }, /* LATIN SMALL LETTER I ACUTE;Ll;0;L;0069 0301;;;;N;;;00CD;;00CD */
{ 'i', 0x0302, 0xEE, 0x00EE, 'î' }, /* LATIN SMALL LETTER I CIRCUMFLEX;Ll;0;L;0069 0302;;;;N;;;00CE;;00CE */
{ 'i', 0x0308, 0xEF, 0x00EF, 'ï' }, /* LATIN SMALL LETTER I DIAERESIS;Ll;0;L;0069 0308;;;;N;;;00CF;;00CF */
{ 'n', 0x0303, 0xF1, 0x00F1, 'ñ' }, /* LATIN SMALL LETTER N TILDE;Ll;0;L;006E 0303;;;;N;;;00D1;;00D1 */
{ 'o', 0x0300, 0xF2, 0x00F2, 'ò' }, /* LATIN SMALL LETTER O GRAVE;Ll;0;L;006F 0300;;;;N;;;00D2;;00D2 */
{ 'o', 0x0301, 0xF3, 0x00F3, 'ó' }, /* LATIN SMALL LETTER O ACUTE;Ll;0;L;006F 0301;;;;N;;;00D3;;00D3 */
{ 'o', 0x0302, 0xF4, 0x00F4, 'ô' }, /* LATIN SMALL LETTER O CIRCUMFLEX;Ll;0;L;006F 0302;;;;N;;;00D4;;00D4 */
{ 'o', 0x0303, 0xF5, 0x00F5, 'õ' }, /* LATIN SMALL LETTER O TILDE;Ll;0;L;006F 0303;;;;N;;;00D5;;00D5 */
{ 'o', 0x0308, 0xF6, 0x00F6, 'ö' }, /* LATIN SMALL LETTER O DIAERESIS;Ll;0;L;006F 0308;;;;N;;;00D6;;00D6 */
{ 'o', 0x0337, 0xF8, 0x00F8, 'ø' }, /* LATIN SMALL LETTER O SLASH;Ll;0;L;006F 0338;;;;N;;;00D8;;00D8 */
{ 's', 0x030C, 0xA8, 0x0161, '¨' }, /* LATIN SMALL LETTER S HACEK;Ll;0;L;0073 030C;;;;N;;;0160;;0160 */
{ 'u', 0x0300, 0xF9, 0x00F9, 'ù' }, /* LATIN SMALL LETTER U GRAVE;Ll;0;L;0075 0300;;;;N;;;00D9;;00D9 */
{ 'u', 0x0301, 0xFA, 0x00FA, 'ú' }, /* LATIN SMALL LETTER U ACUTE;Ll;0;L;0075 0301;;;;N;;;00DA;;00DA */
{ 'u', 0x0302, 0xFB, 0x00FB, 'û' }, /* LATIN SMALL LETTER U CIRCUMFLEX;Ll;0;L;0075 0302;;;;N;;;00DB;;00DB */
{ 'u', 0x0308, 0xFC, 0x00FC, 'ü' }, /* LATIN SMALL LETTER U DIAERESIS;Ll;0;L;0075 0308;;;;N;;;00DC;;00DC */
{ 'y', 0x0301, 0xFD, 0x00FD, 'ý' }, /* LATIN SMALL LETTER Y ACUTE;Ll;0;L;0079 0301;;;;N;;;00DD;;00DD */
{ 'y', 0x0308, 0xFF, 0x00FF, 'ÿ' }, /* LATIN SMALL LETTER Y DIAERESIS;Ll;0;L;0079 0308;;;;N;;;0178;;0178 */
{ 'z', 0x030C, 0xB8, 0x017E, '¸' }, /* LATIN SMALL LETTER Z HACEK;Ll;0;L;007A 030C;;;;N;;;017D;;017D */
{ '\0', 0,     0x00, 0x0000, '\0' }
};

#ifdef TEST
#include <stdio.h>
main() {
struct dmap *dcy;
unsigned short s;
for (dcy = diacry; dcy->basech; dcy++) if (dcy->basech) {
  s = dcy->basech;
  fwrite(&s, 2, 1, stdout);
  s = dcy->nonsp;
  fwrite(&s, 2, 1, stdout);
  s = dcy->utf16;
  fwrite(&s, 2, 1, stdout);
  s = dcy->iso15;
  fwrite(&s, 2, 1, stdout);
}
  s = '\n';
  fwrite(&s, 2, 1, stdout);
}
#endif
