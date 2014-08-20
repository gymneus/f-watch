

SRCS=test_convert_files.c
OBJS=$(SRCS:.c=.o)
BIN=test_convert_files

PREREQ_SRC.1=../watch_format/watch_file_format.c
PREREQ_OBJ.1=$(notdir $(PREREQ_SRC.1:.c=.o))

PREREQ_SRC.2=../watch_format/watch_file_gpx.c
PREREQ_OBJ.2=$(notdir $(PREREQ_SRC.2:.c=.o))

PREREQ_SRCS=$(PREREQ_SRC.1) $(PREREQ_SRC.2)
PREREQ_OBJS=$(PREREQ_OBJ.1) $(PREREQ_OBJ.2)

INCLUDES=-I../watch_format
CFLAGS=-g
LIBS=
LDFLAGS=
COMPILER=gcc

ifdef VERBOSE
CFLAGS+= -DVERBOSE=1
endif

.PHONY: all clean

all: prerequis $(BIN)

clean:
	rm -rf $(BINS) $(OBJS)

prerequis:
	$(COMPILER) $(CFLAGS) $(INCLUDES) -o $(PREREQ_OBJ.1) -c $(PREREQ_SRC.1)
	$(COMPILER) $(CFLAGS) $(INCLUDES) -o $(PREREQ_OBJ.2) -c $(PREREQ_SRC.2)

$(BIN): $(OBJS)
	$(COMPILER) $(LDFLAGS) $(LIBS) -o $@ $^ $(PREREQ_OBJS)

%.o: %.c
	$(COMPILER) $(CFLAGS) $(INCLUDES) -o $@ -c $^

printxavier:
	@echo "SRCS = $(SRCS)"
	@echo "OBJS = $(OBJS)"
	@echo "BIN  = $(BIN)"
	@echo "PREREQ_SRCS = $(PREREQ_SRCS)"
	@echo "PREREQ_OBJS = $(PREREQ_OBJS)"
