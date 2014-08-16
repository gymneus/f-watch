#!/usr/bin/python

import sys
import math
import numpy
from PIL import Image

def pixel_to_bit(pixel,threshold=128):
    """Convert the pixel value to a bit."""
    if pixel[0] > threshold:
        return 1
    else:
        return 0

def convert(im, name):

    name = name.split('.')[0]

    f = open(name+".h",'w')

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
    f.write("static const struct rle_bitmap %s = { %d, %d, %s_data };\n" % (name, im.size[0], im.size[1], name));
    f.close()

    print "RLE encoded image written to: %s.h\n"%name


def use():
    print "Use: bmp2array.py  imagefile"


#
# Main script begins here.
#
# First check for keyword arguments on the command line.
#

if len(sys.argv) > 2:
    print "Too many arghuments."
    use()
    exit(-1)


#
# Open the image file and get the basic information about it.
#
try:
    im = Image.open(sys.argv[1])
except:
    # Eventually this should give more useful information (e.g. file does not
    # exist, or not an image file, or ...
    print "Unable to open %s" % sys.argv[1]
    exit(-1)

width,height = im.size
print "\nfile    : %s\nformat  : %s\nmode    : %s\npalette : %s\nsize    : %dx%d\n" % (sys.argv[1], im.format, im.mode, im.palette, width, height)

#
# Do it!
#
convert(im, sys.argv[1])
