CC	= gcc
CFLAGS  = -Wall
INCLUDE = -I/usr/include/libusb-1.0
SOURCES = usbled.c
LIB	=
LDFLAGS = -lusb-1.0

all:
	$(CC) -o usbled $(CFLAGS) $(INCLUDE) $(SOURCES) $(LIB) $(LDFLAGS)

clean:
	rm usbled
