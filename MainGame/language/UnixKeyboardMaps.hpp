//
// Copyright (c) 2016-2018 João Baptista de Paula e Silva.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//


#if __linux__


#include <X11/keysymdef.h>

const sf::Uint32 codeMaps0x100[96] =
{
    0x0,	0x0104,	0x02D8,	0x0141,	0x0,	0x013D,	0x015A,	0x0,
    0x0,	0x0160,	0x015E,	0x0164,	0x0179,	0x0,	0x017D,	0x017B,
    0x0,	0x0105,	0x02DB,	0x0142,	0x0,	0x013E,	0x015B,	0x02C7,
    0x0,	0x0161,	0x015F,	0x0165,	0x017A,	0x02DD,	0x017E,	0x017C,
    0x0154,	0x0,	0x0,	0x0102,	0x0,	0x0139,	0x0106,	0x0,
    0x010C,	0x0,	0x0118,	0x0,	0x011A,	0x0,	0x0,	0x010E,
    0x0110,	0x0143,	0x0147,	0x0,	0x0,	0x0150,	0x0,	0x0,
    0x0158,	0x016E,	0x0,	0x0170,	0x0,	0x0,	0x0162,	0x0,
    0x0155,	0x0,	0x0,	0x0103,	0x0,	0x013A,	0x0107,	0x0,
    0x010D,	0x0,	0x0119,	0x0,	0x011B,	0x0,	0x0,	0x010F,
    0x0111,	0x0144,	0x0148,	0x0,	0x0,	0x0151,	0x0,	0x0,
    0x0159,	0x016F,	0x0,	0x0171,	0x0,	0x0,	0x0163,	0x02D9,
};

const sf::Uint32 codeMaps0x200[96] =
{
    0x0,	0x0126,	0x0,	0x0,	0x0,	0x0,	0x0124,	0x0,
    0x0,	0x0130,	0x0,	0x011E,	0x0134,	0x0,	0x0,	0x0,
    0x0,	0x0127,	0x0,	0x0,	0x0,	0x0,	0x0125,	0x0,
    0x0,	0x0131,	0x0,	0x011F,	0x0135,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x010A,	0x0108,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0120,	0x0,	0x0,
    0x011C,	0x0,	0x0,	0x0,	0x0,	0x016C,	0x015C,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x010B,	0x0109,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0121,	0x0,	0x0,
    0x011D,	0x0,	0x0,	0x0,	0x0,	0x016D,	0x015D,	0x0,
};

const sf::Uint32 codeMaps0x300[96] =
{
    0x0,	0x0,	0x0138,	0x0156,	0x0,	0x0128,	0x013B,	0x0,
    0x0,	0x0,	0x0112,	0x0122,	0x0166,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0157,	0x0,	0x0129,	0x013C,	0x0,
    0x0,	0x0,	0x0113,	0x0123,	0x0167,	0x014A,	0x0,	0x014B,
    0x0100,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x012E,
    0x0,	0x0,	0x0,	0x0,	0x0116,	0x0,	0x0,	0x012A,
    0x0,	0x0145,	0x014C,	0x0136,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0172,	0x0,	0x0,	0x0,	0x0168,	0x016A,	0x0,
    0x0101,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x012F,
    0x0,	0x0,	0x0,	0x0,	0x0117,	0x0,	0x0,	0x012B,
    0x0,	0x0146,	0x014D,	0x0137,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0173,	0x0,	0x0,	0x0,	0x0169,	0x016B,	0x0,
};

const sf::Uint32 codeMaps0x400[64] =
{
    0x0,	0x3002,	0x300C,	0x300D,	0x3001,	0x30FB,	0x30F2,	0x30A1,
    0x30A3,	0x30A5,	0x30A7,	0x30A9,	0x30E3,	0x30E5,	0x30E7,	0x30C3,
    0x30FC,	0x30A2,	0x30A4,	0x30A6,	0x30A8,	0x30AA,	0x30AB,	0x30AD,
    0x30AF,	0x30B1,	0x30B3,	0x30B5,	0x30B7,	0x30B9,	0x30BB,	0x30BD,
    0x30BF,	0x30C1,	0x30C4,	0x30C6,	0x30C8,	0x30CA,	0x30CB,	0x30CC,
    0x30CD,	0x30CE,	0x30CF,	0x30D2,	0x30D5,	0x30D8,	0x30DB,	0x30DE,
    0x30DF,	0x30E0,	0x30E1,	0x30E2,	0x30E4,	0x30E6,	0x30E8,	0x30E9,
    0x30EA,	0x30EB,	0x30EC,	0x30ED,	0x30EF,	0x30F3,	0x309B,	0x309C,
};

const sf::Uint32 codeMaps0x600[96] =
{
    0x0,	0x0452,	0x0453,	0x0451,	0x0454,	0x0455,	0x0456,	0x0457,
    0x0458,	0x0459,	0x045A,	0x045B,	0x045C,	0x0491,	0x045E,	0x045F,
    0x2116,	0x0402,	0x0403,	0x0401,	0x0404,	0x0405,	0x0406,	0x0407,
    0x0408,	0x0409,	0x040A,	0x040B,	0x040C,	0x0490,	0x040E,	0x040F,
    0x044E,	0x0430,	0x0431,	0x0446,	0x0434,	0x0435,	0x0444,	0x0433,
    0x445,	0x0438,	0x0439,	0x043A,	0x043B,	0x043C,	0x043D,	0x043E,
    0x043F,	0x044F,	0x0440,	0x0441,	0x0442,	0x0443,	0x0436,	0x0432,
    0x044C,	0x044B,	0x0437,	0x0448,	0x044D,	0x0449,	0x0447,	0x044A,
    0x042E,	0x0410,	0x0411,	0x0426,	0x0414,	0x0415,	0x0424,	0x0413,
    0x0425,	0x0418,	0x0419,	0x041A,	0x041B,	0x041C,	0x041D,	0x041E,
    0x041F,	0x042F,	0x0420,	0x0421,	0x0422,	0x0423,	0x0416,	0x0412,
    0x042C,	0x042B,	0x0417,	0x0428,	0x042D,	0x0429,	0x0427,	0x042A,
};

const sf::Uint32 codeMaps0x700[32] =
{
    0x0,	0x0386,	0x0388,	0x0389,	0x038A,	0x03AA,	0x0,	0x038C,
    0x038E,	0x03AB,	0x0,	0x038F,	0x0,	0x0,	0x0385,	0x2015,
    0x0,	0x03AC,	0x03AD,	0x03AE,	0x03AF,	0x03CA,	0x0390,	0x03CC,
    0x03CD,	0x03CB,	0x03B0,	0x03CE,	0x0,	0x0,	0x0,	0x0,
};

const sf::Uint32 codeMaps0x800[96] =
{
    0x0,	0x23B7,	0x250C,	0x2500,	0x2320,	0x2321,	0x2502,	0x23A1,
    0x23A3,	0x23A4,	0x23A6,	0x239B,	0x239D,	0x239E,	0x23A0,	0x23A8,
    0x23AC,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x2264,	0x2260,	0x2265,	0x222B,
    0x2234,	0x221D,	0x221E,	0x0,	0x0,	0x2207,	0x0,	0x0,
    0x223C,	0x2243,	0x0,	0x0,	0x0,	0x21D4,	0x21D2,	0x2261,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x221A,	0x0,
    0x0,	0x0,	0x2282,	0x2283,	0x2229,	0x222A,	0x2227,	0x2228,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x2202,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0192,	0x0,
    0x0,	0x0,	0x0,	0x2190,	0x2191,	0x2192,	0x2193,	0x0,
};

const sf::Uint32 codeMaps0x900[32] =
{
    0x25C6,	0x2592,	0x2409,	0x240C,	0x240D,	0x240A,	0x0,	0x0,
    0x2424,	0x240B,	0x2518,	0x2510,	0x250C,	0x2514,	0x253C,	0x23BA,
    0x23BB,	0x2500,	0x23BC,	0x23BD,	0x251C,	0x2524,	0x2534,	0x252C,
    0x2502,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
};

const sf::Uint32 codeMaps0xA00[96] =
{
    0x0,	0x2003,	0x2002,	0x2004,	0x2005,	0x2007,	0x2008,	0x2009,
    0x200A,	0x2014,	0x2013,	0x0,	0x2423,	0x0,	0x2026,	0x2025,
    0x2153,	0x2154,	0x2155,	0x2156,	0x2157,	0x2158,	0x2159,	0x215A,
    0x2105,	0x0,	0x0,	0x2012,	0x27E8,	0x002E,	0x27E9,	0x0,
    0x0,	0x0,	0x0,	0x215B,	0x215C,	0x215D,	0x215E,	0x0,
    0x0,	0x2122,	0x2613,	0x0,	0x25C1,	0x25B7,	0x25CB,	0x25AF,
    0x2018,	0x2019,	0x201C,	0x201D,	0x211E,	0x2030,	0x2032,	0x2033,
    0x0,	0x271D,	0x0,	0x25AC,	0x25C0,	0x25B6,	0x25CF,	0x25AE,
    0x25E6,	0x25AB,	0x25AD,	0x25B3,	0x25BD,	0x2606,	0x2022,	0x25AA,
    0x25B2,	0x25BC,	0x261C,	0x261E,	0x2663,	0x2666,	0x2665,	0x0,
    0x2720,	0x2020,	0x2021,	0x2713,	0x2717,	0x266F,	0x266D,	0x2642,
    0x2640,	0x260E,	0x2315,	0x2117,	0x2038,	0x201A,	0x201E,	0x0,
};

const sf::Uint32 codeMaps0xB00[96] =
{
    0x0,	0x0,	0x0,	0x003C,	0x0,	0x0,	0x003E,	0x0,
    0x2228,	0x2227,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x00AF,	0x0,	0x22A4,	0x2229,	0x230A,	0x0,	0x005F,	0x0,
    0x0,	0x0,	0x2218,	0x0,	0x2395,	0x0,	0x22A5,	0x25CB,
    0x0,	0x0,	0x0,	0x2308,	0x0,	0x0,	0x222A,	0x0,
    0x2283,	0x0,	0x2282,	0x0,	0x22A3,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
    0x0,	0x0,	0x0,	0x0,	0x22A2,	0x0,	0x0,	0x0,
};

const sf::Uint32 codeMaps0xC00[32] =
{
    0x05D0,	0x05D1,	0x05D2,	0x05D3,	0x05D4,	0x05D5,	0x05D6,	0x05D7,
    0x05D8,	0x05D9,	0x05DA,	0x05DB,	0x05DC,	0x05DD,	0x05DE,	0x05DF,
    0x05E0,	0x05E1,	0x05E2,	0x05E3,	0x05E4,	0x05E5,	0x05E6,	0x05E7,
    0x05E8,	0x05E9,	0x05EA,	0x0,	0x0,	0x0,	0x0,	0x0,
};

const sf::Uint32 codeMaps0xD00[96] =
{
    0x0,	0x0E01,	0x0E02,	0x0E03,	0x0E04,	0x0E05,	0x0E06,	0x0E07,
    0x0E08,	0x0E09,	0x0E0A,	0x0E0B,	0x0E0C,	0x0E0D,	0x0E0E,	0x0E0F,
    0x0E10,	0x0E11,	0x0E12,	0x0E13,	0x0E14,	0x0E15,	0x0E16,	0x0E17,
    0x0E18,	0x0E19,	0x0E1A,	0x0E1B,	0x0E1C,	0x0E1D,	0x0E1E,	0x0E1F,
    0x0E20,	0x0E21,	0x0E22,	0x0E23,	0x0E24,	0x0E25,	0x0E26,	0x0E27,
    0x0E28,	0x0E29,	0x0E2A,	0x0E2B,	0x0E2C,	0x0E2D,	0x0E2E,	0x0E2F,
    0x0E30,	0x0E31,	0x0E32,	0x0E33,	0x0E34,	0x0E35,	0x0,	0x0E37,
    0x0E38,	0x0E39,	0x0E3A,	0x0,	0x0,	0x0,	0x0,	0x0E3F,
    0x0E40,	0x0E41,	0x0E42,	0x0E43,	0x0E44,	0x0E45,	0x0E46,	0x0E47,
    0x0E48,	0x0E49,	0x0E4A,	0x0E4B,	0x0E4C,	0x0E4D,	0x0,	0x0,
    0x0E50,	0x0E51,	0x0E52,	0x0E53,	0x0E54,	0x0E55,	0x0E56,	0x0E57,
    0x0E58,	0x0E59,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
};

const LangID miscKeys1[] =
{
    "key-name-home",
    "key-name-left",
    "key-name-up",
    "key-name-right",
    "key-name-down",
    "key-name-page-up",
    "key-name-page-down",
    "key-name-end",
    "key-name-begin"
};
constexpr size_t miscKeys1Size = sizeof(miscKeys1)/sizeof(miscKeys1[0]);

const LangID miscKeys2[] =
{
    "key-name-num-multiply",
    "key-name-num-add",
    "key-name-num-separator",
    "key-name-num-subtract",
    "key-name-num-decimal",
    "key-name-num-divide"
};
constexpr size_t miscKeys2Size = sizeof(miscKeys2)/sizeof(miscKeys2[0]);

const LangID miscKeys3[] =
{
    "key-name-select",
    "key-name-print",
    "key-name-execute",
    "key-name-insert",
    "key-name-undo",
    "key-name-redo",
    "key-name-menu",
    "key-name-find",
    "key-name-cancel",
    "key-name-help",
    "key-name-break"
};
constexpr size_t miscKeys3Size = sizeof(miscKeys3)/sizeof(miscKeys3[0]);

const LangID miscKeys4[] =
{
    "key-name-shift-left",
    "key-name-shift-right",
    "key-name-control-left",
    "key-name-control-right",
    "key-name-caps-lock",
    "", "", "",
    "key-name-alt-left",
    "key-name-alt-right",
    "key-name-linux-super-left",
    "key-name-linux-super-right"
};
constexpr size_t miscKeys4Size = sizeof(miscKeys4)/sizeof(miscKeys4[0]);

const struct MiscPair { sf::Uint32 code; LangID id; } scrambledMiscKeys[] =
{
    { 0xFF08, "key-name-backspace" },
    { 0xFF09, "key-name-tab" },
    { 0xFF0D, "key-name-return" },
    { 0xFF13, "key-name-pause" },
    { 0xFF14, "key-name-scroll-lock" },
    { 0xFF1B, "key-name-escape" },
    { 0xFF7F, "key-name-num-lock" },
    { 0xFFFF, "key-name-delete" },
};
constexpr size_t scrambledMiscKeysSize = sizeof(scrambledMiscKeys)/sizeof(scrambledMiscKeys[0]);

#endif