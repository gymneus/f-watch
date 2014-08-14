OBJS=gfx.o ui.o ui_pc.o font_helv17.o font_luct38.o font_xm16x25b.o digital_watch.o font_helv38b.o font_helv29.o font_helv22b.o font_xm4x6.o  font_xm5x8.o

CFLAGS=`sdl-config --cflags` -I.
LDFLAGS =`sdl-config --libs`

all:	$(OBJS)
	gcc -o pc-ui $(OBJS) $(LDFLAGS)

clean:
	rm $(OBJS) pc-ui