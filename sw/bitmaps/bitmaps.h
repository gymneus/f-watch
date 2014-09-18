#ifndef BITMAPS_H
#define BITMAPS_H

#include <stdint.h>

struct rle_bitmap
{
    uint8_t w;
    uint8_t h;
    const uint8_t *data;
};

extern const struct rle_bitmap example_icon;
extern const struct rle_bitmap gps_receiving;
extern const struct rle_bitmap battery;
extern const struct rle_bitmap gps_disconnected;
extern const struct rle_bitmap battery_charging;
extern const struct rle_bitmap gps_searching;

#endif /* BITMAPS_H */
