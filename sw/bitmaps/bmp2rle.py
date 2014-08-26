#!/usr/bin/python

import sys
import math
import numpy
import os
import os.path
from PIL import Image

def pixel_to_bit(pixel,threshold=128):
    """Convert the pixel value to a bit."""
    if pixel[0] > threshold:
        return 1
    else:
        return 0

def convert(im, name):
    name = name.split('.')[0]
    f = open("bitmaps.c",'a')
    pix = im.load()

    f.write("static const uint8_t %s_data[] = {\n"%name);
    for y in range(0, im.size[1]):
        for x in range(0, im.size[0]):
            if(x==0 and y==0):
                prev = pix[0, 0][0]
                l=0
            else:
                cur = pix[x, y][0]
                if(cur!=prev or l == 127):
                    if(prev):
                        f.write("        0x%02x,\n" % (l))
                    else:
                        f.write("        0x%02x,\n" % (l | 0x80))
                    l = 0
                else:
                    l=l+1
                prev=cur

    if(prev):
        f.write("        0x%02x" % (l))
    else:
        f.write("        0x%02x" % (l | 0x80))

    f.write("};\n");

    f.write("const struct rle_bitmap %s = { %d, %d, %s_data };\n\n" % (name, im.size[0], im.size[1], name));
    f.close()

    f = open("bitmaps.h",'a')
    f.write("extern const struct rle_bitmap %s;\n" % name);
    f.close()

#
# Main script begins here.
#

# Clean the previous output
if os.path.isfile("bitmaps.c"):
    os.remove("bitmaps.h")

if os.path.isfile("bitmaps.c"):
    os.remove("bitmaps.c")

# Write headers
f = open("bitmaps.h", 'a')
f.write("""#ifndef BITMAPS_H
#define BITMAPS_H

#include <stdint.h>

struct rle_bitmap
{
    uint8_t w;
    uint8_t h;
    uint8_t *data;
};

""")
f.close()

f = open("bitmaps.c", 'a')
f.write("#include <bitmaps.h>\n\n")
f.close()

#
# Process .bmp files in the current directory
#
for file in os.listdir("."):
    if file.endswith(".bmp"):
        try:
            print("Processing: " + file)
            im = Image.open(file)
            width,height = im.size
            print("format  : %s" % im.format)
            print("mode    : %s" % im.mode)
            print("palette : %s" % im.palette)
            print("size    : %dx%d" % (width, height))
            convert(im, file)

        except:
            # Eventually this should give more useful information (e.g. file does not
            # exist, or not an image file, or ...
            print("Unable to open " + file)

f = open("bitmaps.h", 'a')
f.write("\n#endif /* BITMAPS_H */\n")
f.close()

