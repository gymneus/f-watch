OBJS=plot_widget.o path_widget.o gfx.o ui.o ui_pc.o font_helv17.o font_luct38.o font_xm16x25b.o digital_watch.o font_helv38b.o font_helv29.o font_helv22b.o font_xm4x6.o  font_xm5x8.o font_helv11.o

CFLAGS=`sdl-config --cflags` -I. -g -O0
LDFLAGS =`sdl-config --libs` -g -O0

all:	$(OBJS)
	gcc -o pc-ui $(OBJS) $(LDFLAGS)

clean:
	rm $(OBJS) pc-ui