#include <SDL/SDL.h>
#include <sys/time.h>

#include "gfx.h"
#include "ui.h"

static SDL_Surface *scr;

void sys_init()
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTTHREAD);
    scr=SDL_SetVideoMode(128*3,128*3,32,0);
}

static int last_pressed_key=0;
static int key_hold_tics=0;
static int kb_activity_start;

static int64_t last_tics = -1;

 int64_t sys_get_tics()
{
    struct timezone tz = {0,0};
    struct timeval tv;
    
    gettimeofday(&tv, &tz);
    
    return((int64_t)tv.tv_sec*1000000LL+tv.tv_usec) / 1000LL;
}

int sys_update()
{
    SDL_Event ev;
    int cur_key=0;


	/* UI Tick Timer */    
    int64_t tics;
    static int64_t last_tics = -1, last_cycle = -1; 
    static int tick_count = 0, cycle_count = 0;

    if(last_tics < 0)
    	last_tics = sys_get_tics();
    if(last_cycle < 0)
    	last_cycle = sys_get_tics();

    tics = sys_get_tics();

    if(tics - last_tics >= UI_TICK_RATE)
    {
    	ui_post_event ( EVT_TICK, tick_count );
    	tick_count ++;
    	last_tics = tics;
    }

	if(tics - last_cycle >= UI_CYCLE_RATE)
    {
    	ui_post_event ( EVT_NEXT_CYCLE, cycle_count );
    	cycle_count ++;
    	last_cycle = tics;
    }


    while(SDL_PollEvent(&ev)!=0)
    {
		if(ev.type==SDL_KEYDOWN) 
		{
		    switch(ev.key.keysym.sym)
		    {
		    	case SDLK_ESCAPE:
                                exit(0);
                                break;
                        case SDLK_UP:
                                ui_post_event(EVT_UP, 0);
                                break;
                        case SDLK_DOWN:
                                ui_post_event(EVT_DOWN, 0);
                                break;
                        case SDLK_RIGHT:
                                ui_post_event(EVT_RIGHT, 0);
                                break;
                        case SDLK_LEFT:
                                ui_post_event(EVT_LEFT, 0);
                                break;
			#if 0
			case SDLK_LEFT: cur_key=KEY_L; break;	    
			case SDLK_RIGHT: cur_key=KEY_R; break;
			case 'm': cur_key=KEY_M; break;
			case 'v': cur_key=KEY_VOL; break;
			case SDLK_SPACE: cur_key=KEY_PLAY; break;
			#endif
		    }
		}
#if 0
	    if(last_pressed_key!=cur_key)
	    {
		last_pressed_key=cur_key;
		key_hold_tics=sys_get_tics();
    kb_activity_start =sys_get_tics();

		return last_pressed_key|PRESS;
	    } else return last_pressed_key;
	
	} else if (ev.type==SDL_KEYUP)
	{
	    switch(ev.key.keysym.sym)
	    {

	    	case SDLK_ESCAPE: exit(0); break;
		case SDLK_LEFT: cur_key=KEY_L; break;	    
		case SDLK_RIGHT: cur_key=KEY_R; break;
		case 'm': cur_key=KEY_M; break;
		case 'v': cur_key=KEY_VOL; break;
		case SDLK_SPACE: cur_key=KEY_PLAY; break;
		default:return 0;
	    }

	    last_pressed_key=0;
    kb_activity_start =sys_get_tics();

	    return cur_key|RELEASE;

	}
#endif

    }
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void lcd_update(struct surface *surf)
{
	int y, x;
	for( y=0;y<3*surf->height;y++)
		for( x=0;x<3*surf->width;x++)
		{
			uint8_t mask = 1 << ((x/3) & 0x07);                   
    		uint8_t *p = surf->data + ((y/3) * surf->stride) + ((x/3) >> 3);
		
			putpixel(scr, x, y, (*p&mask) ? 0x202020 : 0xb0cdcb);
		}

	SDL_UpdateRect(scr, 0, 0, 0, 0);
}



main()
{
	sys_init();

	ui_init();
	home_screen_create();

	for(;;)
	{
		sys_update();
		ui_update();
	}

}
