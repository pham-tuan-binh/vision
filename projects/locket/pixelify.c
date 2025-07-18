/*******************************************************************************
 * Size: 24 px
 * Bpp: 1
 * Opts: --bpp 1 --size 24 --no-compress --font PixelifySans-Regular.ttf --range 32-127 --format lvgl -o pixelify.c
 ******************************************************************************/

#include "lvgl.h"

#ifndef PIXELIFY
#define PIXELIFY 1
#endif

#if PIXELIFY

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xfc, 0x3c,

    /* U+0022 "\"" */
    0xe6, 0x73, 0x4, 0x6c, 0xc0,

    /* U+0023 "#" */
    0x30, 0x61, 0x83, 0xc, 0x18, 0x60, 0xcf, 0xff,
    0xff, 0xfc, 0xc1, 0x86, 0xc, 0x30, 0x61, 0x83,
    0xc, 0x19, 0xff, 0xff, 0xff, 0x98, 0x30, 0xc1,
    0x80,

    /* U+0024 "$" */
    0x6, 0x0, 0xc0, 0x18, 0x1f, 0xc3, 0xf9, 0x80,
    0xf0, 0x1e, 0x0, 0xc0, 0x7, 0xf0, 0xfe, 0x0,
    0x30, 0x6, 0x0, 0xf0, 0x1e, 0x3, 0x3f, 0x87,
    0xf0, 0x18, 0x3, 0x0,

    /* U+0025 "%" */
    0x30, 0x1, 0x8c, 0x0, 0x6f, 0xe0, 0xe3, 0xf8,
    0x30, 0x30, 0x30, 0xc, 0xc, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x3, 0x3, 0x0, 0xc0, 0xc0, 0x40,
    0x30, 0x30, 0x7f, 0xc, 0x1f, 0xdc, 0x0, 0xc6,
    0x0, 0x30,

    /* U+0026 "&" */
    0x3f, 0xc0, 0x7f, 0x83, 0x1, 0x86, 0x3, 0x3,
    0x1c, 0x6, 0x38, 0x3, 0x80, 0x3, 0x0, 0x39,
    0xc0, 0x63, 0x81, 0x1, 0x6, 0x3, 0xc, 0x6,
    0x7, 0xfb, 0xcf, 0xf3, 0x80,

    /* U+0027 "'" */
    0xee, 0x16,

    /* U+0028 "(" */
    0x6, 0xc, 0x18, 0x33, 0x86, 0xc, 0x18, 0x41,
    0x83, 0x6, 0xc, 0x6, 0xc, 0x18, 0x30, 0x1c,
    0x18, 0x30, 0x60, 0xc0,

    /* U+0029 ")" */
    0xc1, 0x83, 0x6, 0x3, 0x6, 0xc, 0x18, 0x8,
    0xc, 0x18, 0x30, 0x67, 0xc, 0x18, 0x31, 0x83,
    0x6, 0xc, 0x18, 0x0,

    /* U+002A "*" */
    0x6f, 0x6f,

    /* U+002B "+" */
    0x6, 0x0, 0xc0, 0x18, 0x3, 0x0, 0x61, 0xff,
    0xff, 0xf8, 0x30, 0x6, 0x0, 0xc0, 0x18, 0x0,

    /* U+002C "," */
    0xee, 0x16,

    /* U+002D "-" */
    0xff, 0xfc,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x0, 0xc0, 0x60, 0x18, 0xc, 0x3, 0x1, 0x80,
    0x60, 0x30, 0xc, 0x6, 0x1, 0x80, 0xc0, 0x30,
    0x18, 0x6, 0x0,

    /* U+0030 "0" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0031 "1" */
    0x30, 0x63, 0x3e, 0x30, 0x60, 0xc1, 0x83, 0x6,
    0xc, 0x18, 0x30, 0x60, 0xc1, 0x80,

    /* U+0032 "2" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x30, 0x6, 0x0,
    0xcf, 0xe1, 0xfc, 0xc0, 0x18, 0x3, 0x0, 0x60,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0033 "3" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x30, 0x6, 0x0,
    0xcf, 0xe1, 0xfc, 0x0, 0x60, 0xc, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0034 "4" */
    0x1, 0xe0, 0x3c, 0x19, 0x83, 0x33, 0x86, 0x60,
    0xf0, 0x1e, 0x3, 0xff, 0xff, 0xfc, 0x1, 0x80,
    0x30, 0x6, 0x0, 0xc0, 0x18,

    /* U+0035 "5" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xcc, 0x1, 0x80, 0xf, 0x81, 0xf0, 0x2, 0x60,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0036 "6" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x1, 0x80,
    0x3f, 0xe7, 0xfc, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x34, 0x8, 0x7f, 0xf, 0xe0,

    /* U+0037 "7" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x30, 0x6, 0x0,
    0xc0, 0x18, 0x3, 0x0, 0x60, 0xc, 0x1, 0x80,
    0x30, 0x6, 0x0, 0xc0, 0x18,

    /* U+0038 "8" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xcf, 0xe1, 0xfc, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x34, 0x8, 0x7f, 0xf, 0xe0,

    /* U+0039 "9" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xcf, 0xf9, 0xff, 0x0, 0x60, 0xc, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+003A ":" */
    0xf0, 0x3c,

    /* U+003B ";" */
    0xee, 0x0, 0xe, 0xe1, 0x60,

    /* U+003C "<" */
    0x0, 0x60, 0xc, 0x2, 0x0, 0xc0, 0x18, 0xc,
    0x1, 0x81, 0xc0, 0x30, 0x18, 0x3, 0x0, 0x18,
    0x3, 0x0, 0x10, 0x1, 0x80, 0x30, 0x1, 0x80,
    0x30, 0x1, 0x80, 0x30,

    /* U+003D "=" */
    0xff, 0xff, 0xc0, 0x0, 0xf, 0xff, 0xfc,

    /* U+003E ">" */
    0xc0, 0x18, 0x1, 0x0, 0x18, 0x3, 0x0, 0x1c,
    0x1, 0x80, 0xc, 0x1, 0x80, 0xc, 0x1, 0x80,
    0xc0, 0x18, 0x4, 0x1, 0x80, 0x30, 0x38, 0x6,
    0x3, 0x0, 0x60, 0x0,

    /* U+003F "?" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xc1, 0xe0, 0x3c, 0x6, 0x0, 0xc0, 0x18, 0x0,
    0x0, 0x0, 0xc, 0x1, 0x80,

    /* U+0040 "@" */
    0x3f, 0xc, 0x33, 0x3, 0x6f, 0xbd, 0x17, 0xa3,
    0xb3, 0xe3, 0x0, 0x20, 0x7, 0xc0,

    /* U+0041 "A" */
    0x6, 0x0, 0xc0, 0xe6, 0x18, 0xcc, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xff, 0xff, 0xff, 0x1, 0xe0,
    0x3c, 0x7, 0x80, 0xf0, 0x18,

    /* U+0042 "B" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf3, 0xe6, 0x7c, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x34, 0x8, 0x7f, 0xf, 0xe0,

    /* U+0043 "C" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x6, 0x0, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x34, 0x8, 0x7f, 0xf, 0xe0,

    /* U+0044 "D" */
    0xfe, 0x1f, 0xc3, 0x6, 0x60, 0xcc, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0xcc, 0x19, 0xfc, 0x3f, 0x80,

    /* U+0045 "E" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x1, 0x80,
    0x3f, 0x87, 0xf0, 0xc0, 0x18, 0x3, 0x0, 0x60,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0046 "F" */
    0xff, 0x9f, 0xf3, 0x1, 0xe0, 0x3c, 0x1, 0x80,
    0x3f, 0x87, 0xf0, 0xc0, 0x18, 0x3, 0x0, 0x60,
    0xc, 0x1, 0x80, 0x30, 0x0,

    /* U+0047 "G" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf1, 0x86, 0x30, 0xc1, 0xf8, 0x3f, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0048 "H" */
    0xc0, 0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc6, 0x3c,
    0x63, 0xff, 0xff, 0xff, 0xc0, 0x3c, 0x3, 0xc0,
    0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x30,

    /* U+0049 "I" */
    0xff, 0xfc, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc1, 0x83, 0x1f, 0xff, 0x80,

    /* U+004A "J" */
    0x3f, 0x87, 0xfc, 0x1, 0x80, 0x30, 0x6, 0x0,
    0xc0, 0x18, 0x3, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x34, 0x8, 0x7f, 0xf, 0xe0,

    /* U+004B "K" */
    0x30, 0x66, 0xf, 0x6, 0x60, 0xcc, 0x61, 0x8c,
    0x3e, 0x7, 0x80, 0xce, 0x18, 0xc3, 0x8, 0x60,
    0xcc, 0x19, 0x80, 0xf0, 0x18,

    /* U+004C "L" */
    0xc0, 0x18, 0x3, 0x0, 0x60, 0xc, 0x1, 0x80,
    0x30, 0x6, 0x0, 0xc0, 0x18, 0x3, 0x0, 0x60,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+004D "M" */
    0x30, 0x18, 0x60, 0x33, 0x39, 0x9e, 0x33, 0x3c,
    0x66, 0x78, 0xcc, 0xf0, 0x61, 0xe0, 0xc3, 0xc1,
    0x87, 0x83, 0xf, 0x6, 0x1e, 0xc, 0x3c, 0x18,
    0x78, 0x0, 0xf0, 0x1, 0x80,

    /* U+004E "N" */
    0xc0, 0x78, 0xf, 0xe1, 0xfc, 0x3c, 0x67, 0x8c,
    0xf1, 0x9e, 0x33, 0xc6, 0x78, 0xcf, 0x19, 0xe0,
    0xfc, 0x1f, 0x80, 0xf0, 0x18,

    /* U+004F "O" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0050 "P" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xff, 0xff, 0xf3, 0x0, 0x60,
    0xc, 0x1, 0x80, 0x30, 0x0,

    /* U+0051 "Q" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0, 0x30, 0x6, 0x0,
    0xc0, 0x6, 0x0, 0xc0,

    /* U+0052 "R" */
    0x3f, 0x9f, 0xff, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xff, 0x9f, 0xf3, 0xc, 0x61,
    0x8c, 0x31, 0x81, 0xb0, 0x30,

    /* U+0053 "S" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x1, 0x80,
    0xf, 0xe1, 0xfc, 0x0, 0x60, 0xc, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0054 "T" */
    0x3f, 0x87, 0xf3, 0x19, 0xe3, 0x30, 0x60, 0xc,
    0x1, 0x80, 0x30, 0x6, 0x0, 0xc0, 0x18, 0x3,
    0x0, 0x60, 0xc, 0x1, 0x80,

    /* U+0055 "U" */
    0xc0, 0x78, 0xf, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0056 "V" */
    0xc0, 0x78, 0xf, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xf1, 0xe6, 0x30, 0xc6, 0x18,
    0xc0, 0xa0, 0xc, 0x1, 0x80,

    /* U+0057 "W" */
    0xc0, 0x7, 0x83, 0xf, 0x6, 0x1e, 0xc, 0x3c,
    0x18, 0x78, 0x30, 0xf0, 0x61, 0xe0, 0xc3, 0xc6,
    0x67, 0x8c, 0xcf, 0x19, 0x9e, 0x33, 0x3c, 0x66,
    0x67, 0x3, 0xc, 0x6, 0x0,

    /* U+0058 "X" */
    0xc0, 0x78, 0xc, 0xc6, 0x18, 0xc3, 0x18, 0x63,
    0x3, 0x80, 0x30, 0x39, 0x86, 0x30, 0xc6, 0x18,
    0xc4, 0x9, 0x80, 0xf0, 0x18,

    /* U+0059 "Y" */
    0xc0, 0x78, 0xf, 0x1, 0xe0, 0x33, 0x18, 0x63,
    0x3, 0x80, 0x30, 0x6, 0x0, 0xc0, 0x18, 0x3,
    0x0, 0x60, 0xc, 0x1, 0x80,

    /* U+005A "Z" */
    0xff, 0x9f, 0xf3, 0x1, 0xe0, 0x30, 0x6, 0x0,
    0xcf, 0xe1, 0xfc, 0xc0, 0x18, 0x3, 0x1, 0xe0,
    0x34, 0x6, 0x7f, 0xcf, 0xf8,

    /* U+005B "[" */
    0x3e, 0x7d, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0x3e, 0x7c,

    /* U+005C "\\" */
    0x60, 0x18, 0x3, 0x0, 0xc0, 0x18, 0x6, 0x0,
    0xc0, 0x30, 0x6, 0x1, 0x80, 0x30, 0xc, 0x1,
    0x80, 0x60, 0xc,

    /* U+005D "]" */
    0xf9, 0xf0, 0x20, 0x30, 0x60, 0xc1, 0x83, 0x6,
    0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0xf9, 0xf0,

    /* U+005E "^" */
    0x6f,

    /* U+005F "_" */
    0xff, 0xfc,

    /* U+0060 "`" */
    0xc6, 0x30,

    /* U+0061 "a" */
    0x3f, 0x81, 0xfc, 0x30, 0x19, 0x80, 0xcc, 0x6,
    0x60, 0x33, 0x1, 0x98, 0xc, 0xc0, 0x61, 0xfc,
    0xcf, 0xe6,

    /* U+0062 "b" */
    0xc0, 0x18, 0x3, 0x0, 0x60, 0xf, 0xf9, 0xff,
    0x30, 0x1e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0063 "c" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x1, 0x80,
    0x30, 0x6, 0x3, 0xc0, 0x67, 0xf0, 0xfe, 0x0,

    /* U+0064 "d" */
    0x0, 0x60, 0xc, 0x1, 0x80, 0x33, 0xfe, 0x7f,
    0xf0, 0x1e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x3c, 0x6, 0x7f, 0xf, 0xe0,

    /* U+0065 "e" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3f, 0xe1, 0xfc,
    0x30, 0x6, 0x3, 0xc0, 0x67, 0xf0, 0xfe, 0x0,

    /* U+0066 "f" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x1, 0x80,
    0x3f, 0x87, 0xf0, 0xc0, 0x18, 0x3, 0x0, 0x60,
    0xc, 0x1, 0x80, 0x30, 0x0,

    /* U+0067 "g" */
    0x3f, 0x87, 0xf1, 0x2, 0x60, 0x3c, 0x7, 0x80,
    0xf0, 0x19, 0xff, 0x3f, 0xe0, 0xc, 0x1, 0x80,
    0x3c, 0x7, 0x80, 0xcf, 0xe1, 0xfc,

    /* U+0068 "h" */
    0xc0, 0x18, 0x3, 0x0, 0x60, 0xc, 0xf9, 0x9f,
    0x3c, 0x1f, 0x83, 0xc0, 0x78, 0xf, 0x1, 0xe0,
    0x3c, 0x7, 0x80, 0xf0, 0x18,

    /* U+0069 "i" */
    0xf0, 0xff, 0xff, 0xfc,

    /* U+006A "j" */
    0x1, 0x80, 0xc0, 0x60, 0x30, 0x18, 0xc, 0x7,
    0x83, 0xc1, 0x9f, 0xf, 0x80,

    /* U+006B "k" */
    0xc0, 0x18, 0x3, 0x0, 0x60, 0xc, 0x7, 0x80,
    0xf3, 0xe6, 0x7c, 0xf0, 0x1e, 0x3, 0x40, 0x67,
    0xcc, 0xf9, 0x80, 0xf0, 0x18,

    /* U+006C "l" */
    0xff, 0xff, 0xff, 0xfc,

    /* U+006D "m" */
    0xfe, 0x7d, 0xfc, 0xfb, 0x6, 0x1e, 0xc, 0x3c,
    0x18, 0x78, 0x30, 0xf0, 0x61, 0xe0, 0xc3, 0xc1,
    0x87, 0x83, 0xf, 0x6, 0x18,

    /* U+006E "n" */
    0xff, 0x9f, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0x80,

    /* U+006F "o" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x67, 0xf0, 0xfe, 0x0,

    /* U+0070 "p" */
    0x3f, 0x87, 0xf1, 0x2, 0x60, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x7f, 0xff, 0xfe, 0x60,
    0xc, 0x1, 0x80, 0x30, 0x6, 0x0,

    /* U+0071 "q" */
    0x3f, 0x87, 0xf1, 0x2, 0x60, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x7f, 0xfc, 0xff, 0x80,
    0x30, 0x6, 0x0, 0xc0, 0x18, 0x3,

    /* U+0072 "r" */
    0x3f, 0x87, 0xf3, 0x1, 0xe0, 0x3c, 0x1, 0x80,
    0x30, 0x6, 0x0, 0xc0, 0x18, 0x3, 0x0, 0x0,

    /* U+0073 "s" */
    0x3f, 0xe7, 0xff, 0x0, 0x60, 0x3, 0xf8, 0x7f,
    0x0, 0x20, 0x3, 0x0, 0x7f, 0xf3, 0xfe, 0x0,

    /* U+0074 "t" */
    0x30, 0x1, 0x80, 0xc, 0x0, 0x60, 0xf, 0xfe,
    0x7f, 0xf0, 0xc0, 0x6, 0x0, 0x30, 0x1, 0x80,
    0xc, 0x0, 0x60, 0x33, 0x1, 0x87, 0xf0, 0x3f,
    0x80,

    /* U+0075 "u" */
    0xc0, 0x78, 0xf, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0xc0, 0x67, 0xf0, 0xfe, 0x0,

    /* U+0076 "v" */
    0xc0, 0x78, 0xf, 0x1, 0xe0, 0x33, 0x18, 0x63,
    0xc, 0x61, 0x8c, 0xa, 0x0, 0xc0, 0x18, 0x0,

    /* U+0077 "w" */
    0xc1, 0x87, 0x83, 0xf, 0x6, 0x1e, 0xc, 0x3c,
    0x18, 0x78, 0x30, 0xf0, 0x61, 0xe0, 0xc3, 0xc1,
    0x87, 0xfc, 0xfb, 0xf9, 0xf0,

    /* U+0078 "x" */
    0xc0, 0x78, 0xc, 0xc6, 0x18, 0xc0, 0xe0, 0xc,
    0x2, 0x81, 0x8c, 0x31, 0x98, 0xf, 0x1, 0x80,

    /* U+0079 "y" */
    0xc0, 0x78, 0xf, 0x1, 0xe0, 0x3c, 0x7, 0x80,
    0xf0, 0x19, 0xff, 0x3f, 0xe0, 0xc, 0x1, 0x80,
    0x3c, 0x7, 0x80, 0xcf, 0xe1, 0xfc,

    /* U+007A "z" */
    0xff, 0xff, 0xfc, 0x6, 0x0, 0xc0, 0x60, 0xc,
    0xe, 0x1, 0x80, 0x30, 0x1f, 0xff, 0xff, 0x80,

    /* U+007B "{" */
    0x6, 0x3, 0x2, 0x86, 0x33, 0x19, 0x80, 0xc0,
    0x60, 0x30, 0x78, 0x30, 0x6, 0x3, 0x1, 0x80,
    0xc0, 0x60, 0x31, 0x98, 0xc3, 0x80, 0xc0,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xfc,

    /* U+007D "}" */
    0x30, 0x18, 0x12, 0x18, 0xcc, 0x60, 0x30, 0x18,
    0xc, 0x6, 0x3, 0xc0, 0x60, 0xc0, 0x60, 0x30,
    0x18, 0xc, 0xc6, 0x63, 0xe, 0x6, 0x0,

    /* U+007E "~" */
    0xef, 0x70
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 77, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 85, .box_w = 2, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 5, .adv_w = 179, .box_w = 9, .box_h = 4, .ofs_x = 1, .ofs_y = 11},
    {.bitmap_index = 10, .adv_w = 260, .box_w = 13, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 35, .adv_w = 225, .box_w = 11, .box_h = 20, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 63, .adv_w = 333, .box_w = 18, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 97, .adv_w = 260, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 126, .adv_w = 103, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 11},
    {.bitmap_index = 128, .adv_w = 144, .box_w = 7, .box_h = 22, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 148, .adv_w = 144, .box_w = 7, .box_h = 22, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 168, .adv_w = 103, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 11},
    {.bitmap_index = 170, .adv_w = 230, .box_w = 11, .box_h = 11, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 186, .adv_w = 103, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 188, .adv_w = 128, .box_w = 7, .box_h = 2, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 190, .adv_w = 85, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 203, .box_w = 10, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 210, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 231, .adv_w = 155, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 245, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 266, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 287, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 329, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 350, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 371, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 392, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 85, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 415, .adv_w = 103, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 420, .adv_w = 230, .box_w = 11, .box_h = 20, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 448, .adv_w = 230, .box_w = 9, .box_h = 6, .ofs_x = 3, .ofs_y = 3},
    {.bitmap_index = 455, .adv_w = 230, .box_w = 11, .box_h = 20, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 483, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 504, .adv_w = 225, .box_w = 11, .box_h = 10, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 518, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 539, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 560, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 581, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 602, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 623, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 644, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 665, .adv_w = 225, .box_w = 12, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 688, .adv_w = 155, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 702, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 723, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 744, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 765, .adv_w = 295, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 794, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 815, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 836, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 857, .adv_w = 225, .box_w = 11, .box_h = 20, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 885, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 906, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 927, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 948, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 969, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 990, .adv_w = 295, .box_w = 15, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1019, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1040, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1061, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1082, .adv_w = 144, .box_w = 7, .box_h = 18, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1098, .adv_w = 203, .box_w = 10, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1117, .adv_w = 144, .box_w = 7, .box_h = 18, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1133, .adv_w = 103, .box_w = 4, .box_h = 2, .ofs_x = 1, .ofs_y = 13},
    {.bitmap_index = 1134, .adv_w = 170, .box_w = 7, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1136, .adv_w = 102, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 13},
    {.bitmap_index = 1138, .adv_w = 260, .box_w = 13, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1156, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1177, .adv_w = 225, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1193, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1214, .adv_w = 225, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1230, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1251, .adv_w = 225, .box_w = 11, .box_h = 16, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 1273, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1294, .adv_w = 85, .box_w = 2, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1298, .adv_w = 190, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1311, .adv_w = 225, .box_w = 11, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1332, .adv_w = 85, .box_w = 2, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1336, .adv_w = 295, .box_w = 15, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1357, .adv_w = 225, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1373, .adv_w = 225, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1389, .adv_w = 225, .box_w = 11, .box_h = 16, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 1411, .adv_w = 225, .box_w = 11, .box_h = 16, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 1433, .adv_w = 225, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1449, .adv_w = 225, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1465, .adv_w = 260, .box_w = 13, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1490, .adv_w = 225, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1506, .adv_w = 221, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1522, .adv_w = 295, .box_w = 15, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1543, .adv_w = 225, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1559, .adv_w = 225, .box_w = 11, .box_h = 16, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 1581, .adv_w = 225, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1597, .adv_w = 178, .box_w = 9, .box_h = 20, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1620, .adv_w = 85, .box_w = 2, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1624, .adv_w = 178, .box_w = 9, .box_h = 20, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1647, .adv_w = 230, .box_w = 6, .box_h = 2, .ofs_x = 4, .ofs_y = 5}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 2,
    0, 3, 4, 0, 5, 6, 0, 7,
    4, 5, 3, 0, 0, 0, 0, 0,
    0, 0, 8, 5, 9, 10, 0, 11,
    0, 6, 12, 3, 13, 14, 4, 6,
    3, 15, 3, 0, 7, 16, 17, 18,
    19, 20, 21, 22, 0, 0, 0, 0,
    2, 0, 23, 24, 25, 26, 0, 27,
    28, 29, 30, 31, 32, 33, 34, 28,
    0, 0, 28, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 0, 6, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 2, 1,
    0, 3, 4, 0, 0, 0, 0, 3,
    0, 0, 5, 0, 0, 0, 0, 0,
    0, 0, 6, 3, 7, 8, 3, 9,
    3, 10, 11, 12, 13, 14, 15, 8,
    3, 16, 3, 13, 5, 4, 17, 18,
    19, 20, 21, 22, 0, 0, 0, 0,
    1, 0, 23, 24, 25, 26, 23, 27,
    28, 29, 30, 31, 32, 33, 34, 34,
    23, 0, 23, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 0, 8, 0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, 0, -12, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -19, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -19, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -12, -8, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -8, -12, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -15,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -8, 0, -15,
    -15, 0, -8, 0, -8, 0, 0, -8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -12, 0, 0, 0, 0, 0,
    0, 0, -12, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -8, 0, -8, 0, -8,
    0, 0, -12, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -8, 0, -15, -12, 0,
    -12, -8, -8, -8, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, -15, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -23, 0, 0, -4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -8, 0,
    -15, -19, 0, -8, 0, -8, 0, 0,
    0, 0, 0, 0, -8, 0, 0, 0,
    0, -8, -8, -8, -8, -8, -8, -8,
    0, 0, 0, -12, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, -12, -12, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -12, 0, 0, 0, 0, 0, -19, 0,
    0, 0, 0, 0, 0, -19, -8, 0,
    0, -8, 0, 0, -8, 0, -19, -23,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, -19,
    0, -12, 0, -12, -8, -8, -35, 0,
    -8, 0, -4, -23, -8, -8, -8, -8,
    -12, -35, -15, -8, -12, -12, -8, -8,
    -8, -15, -12, -8, -38, -12, -38, -38,
    -15, -42, -12, 0, -77, -12, -15, -19,
    -38, -38, 0, -23, -27, -31, -35, -35,
    -35, 0, 0, 0, 0, 0, 0, -8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -19,
    0, -19, 0, 0, 0, 0, 0, 0,
    0, 0, -12, -12, 0, 0, 0, 0,
    -12, 0, 0, 0, 0, 0, -12, -15,
    0, -8, -12, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -8, 0, 0,
    -12, 0, -19, 0, -19, 0, 0, -15,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -12, -15, 0, 0, -12, -12, 0,
    -31, 0, -46, 0, 0, -12, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -19, -46, 0, -12, -8, -8,
    -8, -8, 0, -15, 0, 0, -8, -8,
    0, 0, -8, -38, 0, -38, 0, -15,
    0, 0, -19, 0, 0, -8, 0, -12,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -8, -8, -15, -12, 0,
    -12, -8, -12, -8, -8, 0, -8, 0,
    0, -8, -8, -8, -12, -12, -8, -8,
    -12, -8, 0, -8, -8, -35, -12, -12,
    -23, -8, -15, -12, -12, -12, -12, 0,
    0, -15, 0, -15, -15, -12, -15, 0,
    -23, -15, 0, -38, -8, -38, -38, -12,
    -38, -12, -12, 0, -12, -12, -15, -38,
    -38, 0, -19, 0, -23, 0, -31, -19,
    0, 0, 0, -12, 0, -12, 0, 0,
    0, 0, 0, 0, 0, 0, -8, 0,
    0, 0, 0, 0, -12, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -8, -4,
    -12, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 0, 0, 0, 0, 0,
    0, -15, -8, -12, -12, 0, -12, 0,
    0, 0, -8, 0, -8, -8, -12, 0,
    -8, -4, -8, -8, -8, -8, 0, 0,
    0, -15, 0, -8, 0, 0, 0, 0,
    0, 0, 0, 0, -8, 0, 0, 0,
    0, 0, -8, 0, -12, 0, -8, 0,
    0, -8, 0, 0, 0, 0, 0, 0,
    0, 0, -12, -8, 0, 0, 0, 0,
    -8, 0, 0, -12, -15, -12, -15, -15,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -8, 0, 0, 0, 0, -27,
    0, -23, 0, 0, -12, 0, 0, 0,
    0, 0, -8, -8, -12, 0, 0, 0,
    -8, -8, -8, -8, -19, -19, -8, 0,
    -8, -19, -12, 0, 0, -8, -8, 0,
    -12, -8, -8, -12, -8, -12, -8, -8,
    0, -8, -27, 0, -31, -31, 0, -35,
    0, 0, -61, -15, 0, -19, 0, -38,
    -19, -27, -27, -27, -27, -19, -23, 0,
    0, 0, -8, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -12, 0, 0, 0, 0, -15, 0, -15,
    0, 0, 0, 0, 0, 0, 0, 0,
    -8, -8, -8, -15, -8, -12, -8, -8,
    -8, -8, 0, 0, -27, -50, -12, -15,
    -27, 0, -19, 0, 0, -23, 0, -23,
    -15, -15, -23, -38, -12, -19, -77, 0,
    -19, -19, -19, -19, -19, -19, -12, -12,
    -23, -15, -12, -12, 0, -12, -42, -15,
    -35, -12, -15, -27, -12, 0, 0, 0,
    -46, -8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -12, 0,
    0, -38, 0, -8, 0, -8, 0, 0,
    -8, 0, 0, 0, 0, -8, -4, -8,
    -8, -19, 0, -15, -12, -12, -8, 0,
    0, 0, -12, -46, -8, 0, -12, 0,
    0, 0, 0, -12, 0, 0, 0, 0,
    0, -12, 0, -23, -38, 0, -12, -8,
    -12, -8, -8, -12, -8, 0, 0, -8,
    -8, 0, 0, -8, -19, -8, -12, 0,
    -12, -12, 0, 0, 0, 0, 0, -8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -8,
    0, -8, -8, -8, -8, -8, -8, -8,
    0, -8, -8, -8, -8, -8, -8, -15,
    -8, -12, -8, -8, -8, -8, 0, 0,
    -19, 0, -15, 0, -19, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -15,
    0, 0, -27, 0, -38, -12, -38, -38,
    0, -38, -12, 0, -73, -23, -19, -27,
    -42, -38, -35, -19, -27, -19, -31, -31,
    -23, 0, 0, -8, -46, -8, 0, -8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -12, 0, 0, -38, 0, -12,
    -8, -8, -8, -8, -8, -8, 0, -8,
    -8, -8, 0, 0, -8, -15, -8, -15,
    0, 0, 0, 0, 0, 0, -12, -38,
    -8, 0, -12, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -8, 0, 0,
    -38, 0, -8, 0, -12, -8, 0, -8,
    0, 0, 0, 0, 0, 0, 0, -8,
    -15, -8, -12, -8, -12, -8, 0, 0,
    0, -8, -12, 0, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -8, 0, 0, -12, 0, -8, -4, -8,
    0, 0, -8, 0, 0, -8, 0, 0,
    -8, 0, 0, -12, 0, 0, -8, 0,
    -8, -4, 0, 0, -8, -46, 0, 0,
    -8, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -38, 0,
    0, 0, 0, -8, 0, -8, -12, 0,
    0, 0, 0, 0, -8, 0, -15, 0,
    0, 0, 0, 0, 0, 0, 0, -8,
    0, 0, 0, -8, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -23, -4, -19, -19, -8,
    -4, -8, 0, 0, -8, -8, -8, -8,
    -8, -15, -8, -12, -8, 0, -8, -8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -8, 0,
    -12, 0, 0, -8, 0, 0, 0, -12,
    0, -8, -4, -8, -12, -8, -12, -8,
    -8, -8, 0, 0, 0, -8, -46, -8,
    0, -8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -12, 0, 0, -38,
    0, -4, -8, -4, -8, -8, -8, -8,
    0, -8, -8, -8, 0, 0, -8, -15,
    -8, -15, 0, 0, 0, 0, -58, 0,
    -4, -46, -4, 0, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -12,
    0, 0, -38, 0, -12, -8, -8, -12,
    0, -8, 0, 0, 0, -8, -8, -12,
    -12, -8, 0, -8, -8, -8, -8, -8,
    -8, 0, 0, -8, -46, 0, 0, -8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -8, 0, 0, -35, 0, -8,
    -12, -8, 0, 0, -8, 0, 0, 0,
    0, 0, 0, -8, -8, 0, 0, -8,
    -4, -4, -8, -8, 0, 0, 0, -46,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -8, 0,
    -38, 0, -12, -8, -8, -8, -8, -12,
    -8, 0, -8, -8, -8, -8, 0, -8,
    -27, -8, -27, -12, -12, 0, -8, 0,
    0, -8, -46, 0, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -8, 0, -23, -38, 0, -8, 0, -4,
    -8, 0, -4, -12, 0, 0, 0, 0,
    0, -4, -4, -15, -8, -8, 0, 0,
    0, 0, 0, 0, -8, -27, 0, 0,
    -8, 0, 0, 0, 0, -15, 0, 0,
    0, 0, 0, -8, 0, 0, -38, 0,
    -15, -12, -12, -15, 0, -8, -12, -8,
    -15, -4, -8, -8, 0, -8, -12, -8,
    -8, -8, -8, -8, -8, 0, 0, -8,
    -46, 0, 0, -8, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -8, 0,
    0, -38, 0, -12, -8, -8, -8, -4,
    -4, -8, 0, -8, -4, -4, 0, -8,
    -4, -12, -12, 0, 0, 0, 0, -4,
    0, 0, -8, -46, 0, 0, -8, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -8, 0, 0, -38, 0, -19, -8,
    -19, -8, -8, 0, -12, 0, 0, -8,
    -8, 0, 0, 0, -12, -8, -8, -8,
    0, -8, -8, 0, 0, -8, -46, 0,
    0, -8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -38,
    0, 0, 0, 0, -8, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -8, -46, 0, 0, -8, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -38, 0, -8, 0, -12, -8,
    0, -12, 0, 0, 0, 0, 0, 0,
    0, -12, 0, -12, -8, -8, 0, 0,
    -4
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 43,
    .right_class_cnt     = 43,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t pixelify = {
#else
lv_font_t pixelify = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 23,          /*The maximum line height required by the font*/
    .base_line = 5,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if PIXELIFY*/

