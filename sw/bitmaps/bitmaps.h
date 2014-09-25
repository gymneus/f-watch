#ifndef BITMAPS_H
#define BITMAPS_H

#include <stdint.h>

struct rle_bitmap
{
    uint8_t w;
    uint8_t h;
    const uint8_t *data;
};

extern const struct rle_bitmap comp_circle;
extern const struct rle_bitmap battery;
extern const struct rle_bitmap game_ico;
extern const struct rle_bitmap battery_charging;
extern const struct rle_bitmap gps_searching;
extern const struct rle_bitmap clock_icon;
extern const struct rle_bitmap comp_ico;
extern const struct rle_bitmap gps_disconnected;
extern const struct rle_bitmap date_icon;
extern const struct rle_bitmap comp_arrow;
extern const struct rle_bitmap comp_circle2;
extern const struct rle_bitmap gps_receiving;
extern const struct rle_bitmap settings_icon;
extern const struct rle_bitmap comp_arrow2;
extern const struct rle_bitmap comp_circle3;
extern const struct rle_bitmap example_icon;

#endif /* BITMAPS_H */
