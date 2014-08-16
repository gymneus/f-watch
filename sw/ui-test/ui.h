#ifndef __UI_H
#define __UI_H

#include <stdint.h>
#include "gfx.h"

#define DBG printf

#define WF_DIRTY     		(1<<0)
#define WF_VISIBLE   		(1<<1)
#define WF_TRANSPARENT   	(1<<2)
#define WF_ACTIVE    		(1<<3)
#define WF_DOUBLE_BUFFER    (1<<4)

#define EVT_PRESS       1
#define EVT_RELEASE     2
#define EVT_TICK        3
#define EVT_FRAME       4
#define EVT_LONG_REEASE 5
#define EVT_TAP         5
#define EVT_ACTIVATE    6
#define EVT_NEXT_CYCLE  7

#define UI_TICK_RATE 30 	/* UI refresh ticks = 30 milliseconds */
#define UI_CYCLE_RATE 1200  /* The Machine Cycle */

#define MAX_CHILD_WIDGETS 8

struct ui_event {
    int type;
    int data;
};

struct ui_widget {
    void (*redraw)(struct ui_widget *w);
    void (*event)(struct ui_widget *w, struct ui_event event);
    struct rect pos;
    uint8_t order;
    int flags;

    struct surface dc;
    struct ui_widget *next;
    struct ui_widget *children[MAX_CHILD_WIDGETS];
    int n_children;
};



extern struct surface screen;

#endif
