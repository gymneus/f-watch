CC = gcc
CFLAGS  = -Wall $(INCLUDE)
INCLUDE = -I../nmealib/include
OBJS = main.o
LIB = -lnmea
LIBDIR = -L../nmealib/lib

all: $(OBJS)
	$(CC) $(CFLAGS) -o nmeatest $^ $(LIBDIR) $(LIB)

clean:
	rm nmeatest $(OBJS)

