
SRCS=test_convert_files.c
BINNAME=test_convert_files

PREREQ_SRC.1=../watch_format/watch_file_format.c
PREREQ_SRC.2=../watch_format/watch_file_gpx.c

BUILDDIR=build
OBJDIR=$(BUILDDIR)/obj
PREREQDIR=$(BUILDDIR)/prereq
BINDIR=$(BUILDDIR)/bin
DIRLIST=$(BUILDDIR) $(OBJDIR) $(PREREQDIR) $(BINDIR)

OBJS=$(addprefix $(OBJDIR)/, $(OBJFILES))
OBJFILES=$(notdir $(SRCS:.c=.o))
BIN=$(BINDIR)/$(BINNAME)

PREREQ_OBJ.1=$(addprefix $(PREREQDIR)/, $(notdir $(PREREQ_SRC.1:.c=.o)))
PREREQ_OBJ.2=$(addprefix $(PREREQDIR)/, $(notdir $(PREREQ_SRC.2:.c=.o)))

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
	rm -rf $(DIRLIST)

prerequis:
	mkdir -p $(DIRLIST)

$(BIN): $(OBJS) $(PREREQ_OBJS)
	$(COMPILER) $(LDFLAGS) $(LIBS) -o $@ $(OBJS) $(PREREQ_OBJS)

$(OBJDIR)/%.o: %.c
	$(COMPILER) $(CFLAGS) $(INCLUDES) -o $@ -c $^

$(PREREQDIR)/%.o:../watch_format/%.c
	$(COMPILER) $(CFLAGS) $(INCLUDES) -o $@ -c $^

printxavier:
	@echo "SRCS = $(SRCS)"
	@echo "OBJS = $(OBJS)"
	@echo "BIN  = $(BIN)"
	@echo "PREREQ_SRCS = $(PREREQ_SRCS)"
	@echo "PREREQ_OBJS = $(PREREQ_OBJS)"
