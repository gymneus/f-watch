#include "font.h"

static const uint8_t _gw[] = { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
static const uint16_t _go[] = { 0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144,152,160,168,176,184,192,200,208,216,224,232,240,248,256,264,272,280,288,296,304,312,320,328,336,344,352,360,368,376,384,392,400,408,416,424,432,440,448,456,464,472,480,488,496,504,512,520,528,536,544,552,560,568,576,584,592,600,608,616,624,632,640,648,656,664,672,680,688,696,704,712,720,728,736,744,752};
static const uint8_t _gd[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00,
0x28,0x28,0x28,0x00,0x00,0x00,0x00,0x00,
0x50,0x50,0xf8,0x50,0xf8,0x50,0x50,0x00,
0x20,0x78,0xa0,0x70,0x28,0xf0,0x20,0x00,
0x60,0x68,0x10,0x20,0x58,0x18,0x00,0x00,
0x30,0x40,0x40,0x28,0x50,0x50,0x28,0x00,
0x10,0x10,0x20,0x00,0x00,0x00,0x00,0x00,
0x08,0x10,0x20,0x20,0x20,0x10,0x08,0x00,
0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00,
0x00,0x48,0x30,0x78,0x30,0x48,0x00,0x00,
0x00,0x20,0x20,0xf8,0x20,0x20,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x40,
0x00,0x00,0x00,0xf8,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x00,
0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x40,
0x30,0x48,0x58,0x68,0x48,0x48,0x30,0x00,
0x20,0x60,0x20,0x20,0x20,0x20,0x20,0x00,
0x30,0x48,0x08,0x10,0x20,0x40,0x78,0x00,
0x30,0x48,0x08,0x30,0x08,0x48,0x30,0x00,
0x08,0x18,0x18,0x28,0x28,0x78,0x08,0x00,
0x78,0x40,0x40,0x70,0x08,0x08,0x70,0x00,
0x30,0x40,0x40,0x70,0x48,0x48,0x30,0x00,
0x78,0x08,0x08,0x10,0x10,0x20,0x20,0x00,
0x30,0x48,0x48,0x30,0x48,0x48,0x30,0x00,
0x30,0x48,0x48,0x38,0x08,0x08,0x30,0x00,
0x00,0x20,0x20,0x00,0x00,0x20,0x20,0x00,
0x00,0x20,0x20,0x00,0x00,0x20,0x20,0x40,
0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x00,
0x00,0x00,0xf8,0x00,0xf8,0x00,0x00,0x00,
0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00,
0x30,0x48,0x08,0x10,0x20,0x00,0x20,0x00,
0x00,0x30,0x48,0x58,0x58,0x40,0x30,0x00,
0x30,0x48,0x48,0x78,0x48,0x48,0x48,0x00,
0x70,0x48,0x48,0x70,0x48,0x48,0x70,0x00,
0x30,0x48,0x40,0x40,0x40,0x48,0x30,0x00,
0x70,0x48,0x48,0x48,0x48,0x48,0x70,0x00,
0x78,0x40,0x40,0x70,0x40,0x40,0x78,0x00,
0x78,0x40,0x40,0x70,0x40,0x40,0x40,0x00,
0x30,0x48,0x40,0x58,0x48,0x48,0x38,0x00,
0x48,0x48,0x48,0x78,0x48,0x48,0x48,0x00,
0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00,
0x18,0x08,0x08,0x08,0x48,0x48,0x30,0x00,
0x48,0x48,0x50,0x60,0x50,0x48,0x48,0x00,
0x40,0x40,0x40,0x40,0x40,0x40,0x78,0x00,
0x48,0x78,0x78,0x48,0x48,0x48,0x48,0x00,
0x48,0x68,0x68,0x58,0x58,0x48,0x48,0x00,
0x30,0x48,0x48,0x48,0x48,0x48,0x30,0x00,
0x70,0x48,0x48,0x70,0x40,0x40,0x40,0x00,
0x30,0x48,0x48,0x48,0x48,0x48,0x30,0x18,
0x70,0x48,0x48,0x70,0x50,0x48,0x48,0x00,
0x30,0x48,0x40,0x30,0x08,0x48,0x30,0x00,
0xf8,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
0x48,0x48,0x48,0x48,0x48,0x48,0x30,0x00,
0x48,0x48,0x48,0x48,0x30,0x30,0x30,0x00,
0x48,0x48,0x48,0x48,0x78,0x78,0x48,0x00,
0x48,0x48,0x30,0x30,0x48,0x48,0x48,0x00,
0x88,0x88,0x50,0x20,0x20,0x20,0x20,0x00,
0x78,0x08,0x10,0x20,0x40,0x40,0x78,0x00,
0x38,0x20,0x20,0x20,0x20,0x20,0x38,0x00,
0x40,0x40,0x20,0x20,0x10,0x10,0x08,0x08,
0x70,0x10,0x10,0x10,0x10,0x10,0x70,0x00,
0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,
0x20,0x20,0x10,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x38,0x48,0x48,0x58,0x28,0x00,
0x40,0x40,0x70,0x48,0x48,0x48,0x70,0x00,
0x00,0x00,0x38,0x40,0x40,0x40,0x38,0x00,
0x08,0x08,0x38,0x48,0x48,0x48,0x38,0x00,
0x00,0x00,0x30,0x48,0x78,0x40,0x30,0x00,
0x18,0x20,0x20,0x70,0x20,0x20,0x20,0x00,
0x00,0x00,0x38,0x48,0x48,0x38,0x08,0x30,
0x40,0x40,0x70,0x48,0x48,0x48,0x48,0x00,
0x20,0x00,0x60,0x20,0x20,0x20,0x70,0x00,
0x10,0x00,0x30,0x10,0x10,0x10,0x10,0x60,
0x40,0x40,0x48,0x50,0x60,0x50,0x48,0x00,
0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00,
0x00,0x00,0xd0,0xa8,0xa8,0xa8,0x88,0x00,
0x00,0x00,0x70,0x48,0x48,0x48,0x48,0x00,
0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,
0x00,0x00,0x70,0x48,0x48,0x48,0x70,0x40,
0x00,0x00,0x38,0x48,0x48,0x48,0x38,0x08,
0x00,0x00,0x58,0x60,0x40,0x40,0x40,0x00,
0x00,0x00,0x38,0x40,0x30,0x08,0x70,0x00,
0x00,0x20,0x78,0x20,0x20,0x20,0x18,0x00,
0x00,0x00,0x48,0x48,0x48,0x48,0x38,0x00,
0x00,0x00,0x48,0x48,0x48,0x30,0x30,0x00,
0x00,0x00,0x88,0xa8,0xa8,0xa8,0x50,0x00,
0x00,0x00,0x48,0x48,0x30,0x48,0x48,0x00,
0x00,0x00,0x48,0x48,0x48,0x38,0x08,0x30,
0x00,0x00,0x78,0x10,0x20,0x40,0x78,0x00,
0x08,0x10,0x10,0x20,0x10,0x10,0x08,0x00,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,
0x40,0x20,0x20,0x10,0x20,0x20,0x40,0x00,
0x28,0x50,0x00,0x00,0x00,0x00,0x00,0x00
};
const struct font font_xm5x8 = {
32, 126, 8, _gw, _go, _gd };
