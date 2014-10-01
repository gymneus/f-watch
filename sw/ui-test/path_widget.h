#include "ui.h"
#include "math.h"


extern struct ui_widget path_widget;

typedef struct coord{
        double lat;
        double lon;
        double alt;
}coord;

typedef struct path{
        struct coord *d; // pointer on data array
        uint16_t d_size; // data array size
        uint8_t x_size;  // X plot size (pixel)
        uint8_t y_size;  // Y plot size (pixel)
        char *title;     // pointer on plot title string
        uint8_t frame;   // draw frame around plot
        uint8_t line;    // draw lines between data points
}path;

