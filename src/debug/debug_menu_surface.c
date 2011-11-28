#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"

enum{
	DEBUG_SURFACE_INIT = 0,

	DEBUG_SURFACE_NO = DEBUG_SURFACE_INIT,
	DEBUG_SURFACE_EXIT,

	DEBUG_SURFACE_EXEC,

	DEBUG_SURFACE_MAX
};

static SDL_bool debug_surface_isMenu(void);

static SDL_bool initialized = SDL_FALSE;

static MENUdata menu_data[] = {
	{ SDL_FALSE, "*** SURFACE MENU ***",      NULL,             -1,          -1,  -1, -1 },
	{ SDL_TRUE,  "  SURFACE NO",         value_int, SURFACE_SYSTEM, SURFACE_MAX,   1, -1 },
	{ SDL_TRUE,  "   TEST EXIT",              NULL,             -1,          -1,  -1, -1 },
};

static MENUdata *menu_p = & menu_data[1];

static int items;
static int items_min;
static int items_max;

static int surface_num;
static Sint16 px, py;
static SDL_Surface *draw_surface;

static int disp_blink;

void debug_menu_surface_init(void)
{
	int i;

	if(initialized == SDL_FALSE){
		initialized = SDL_TRUE;
		items_min = 0;
		items_max = sizeof(menu_data) / sizeof(MENUdata) - 1;
		for(i = 0; i <= items_max; i++){
			menu_data[i].now = 0;
		}
		printf("surface menu init %d(%d/%d)\n", items, items_min, items_max);
		draw_surface = NULL;
	}
	px = SCR_HX;
	py = SCR_HY;
	items = items_min;
	menu_p[DEBUG_SURFACE_NO].now = SURFACE_SYSTEM;
	surface_num = -1;
	ASSERT(draw_surface == NULL);
	disp_blink = 0;
	SDL_wheel_SetFillColor(32, 32, 32, 255);
}

int debug_menu_surface(int mode)
{
	int ret;
	Uint32 r_mode;
	Sint16 vx, vy;

	if(debug_surface_isMenu() == SDL_TRUE){
		ret = MENUexec(menu_data, &items, items_max);
		if(surface_num != menu_p[DEBUG_SURFACE_NO].now){
			if(draw_surface != NULL){
				SDL_FreeSurface(draw_surface);
				draw_surface = NULL;
			}
			surface_num = menu_p[DEBUG_SURFACE_NO].now;
			if(surface[surface_num] != NULL){
				draw_surface = SDL_wheel_CreateSurface(SDL_wheel_GetSurfaceW(surface[surface_num]), SDL_wheel_GetSurfaceH(surface[surface_num]), SDL_SWSURFACE);
				if(draw_surface){
					SDL_SetColorKey(draw_surface, 0, 0);
					r_mode = SDL_wheel_SetRanderMode(RENDER_LEFTTOP);
					SDL_wheel_DrawSurface(surface[surface_num], draw_surface, 0, 0, 0, 0, SDL_wheel_GetSurfaceW(surface[surface_num]), SDL_wheel_GetSurfaceH(surface[surface_num]));
					SDL_wheel_SetRanderMode(r_mode);
				}
			}
		}
		if(ret == 0){
			if(mode != DEBUG_MENU_EXEC) mode = DEBUG_MENU_EXEC;
		}else if(ret == 1){
			switch(items){
				case DEBUG_SURFACE_NO:		/* SURFACE NO */
					if(surface[surface_num] != NULL){
						items = DEBUG_SURFACE_EXEC;
					}
					break;
				case DEBUG_SURFACE_EXIT:	/* TEST EXIT */
					if(draw_surface != NULL){
						SDL_FreeSurface(draw_surface);
						draw_surface = NULL;
					}
					game_fillcolor_init();
					mode = DEBUG_MENU_TOP;
					break;

				default:
					break;
			}
		}
	}else{
		vx = 0;
		vy = 0;
		if(pads & PAD_LEFT)  vx = -1;
		if(pads & PAD_RIGHT) vx = +1;
		if(pads & PAD_UP)    vy = -1;
		if(pads & PAD_DOWN)  vy = +1;
		if(pads & PAD_BUTTON1){
			vx *= 8;
			vy *= 8;
		}
		px += vx;
		py += vy;
		if(debug_key_isCancel() == SDL_TRUE){
			items = DEBUG_SURFACE_NO;
			px = SCR_HX;
			py = SCR_HY;
		}
	}

	return mode;
}

int debug_menu_surface_draw(int mode)
{
	char *err = "NOT LOADING IMAGE.";

	if(draw_surface != NULL) SDL_wheel_DispSurface(draw_surface, px, py, 0, 0, SDL_wheel_GetSurfaceW(draw_surface), SDL_wheel_GetSurfaceH(draw_surface));
	else SDL_wheel_DispAscii_Bank(FONT_16x16, err, 320 - (SDL_wheel_GetWidthAscii_Bank(FONT_16x16, err) / 2), 240 - (FONT_H / 2));

	if(items != DEBUG_SURFACE_EXEC){
		MENUdraw(menu_data, items, items_max);
	}else{
		disp_blink++;
		if(disp_blink & 0x40){
			SDL_wheel_DispAscii_Bank(FONT_16x16, "'Q' KEY EXIT.", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
		}else{
			SDL_wheel_DispAscii_Bank(FONT_16x16, "CURSOR : SURFACE MOVE ", 16, SCR_Y - (FONT_H * 2 + FONT_BOTTOM));
			SDL_wheel_DispAscii_Bank(FONT_16x16, " Z KEY : MOVE SPEED UP", 16, SCR_Y - (FONT_H * 1 + FONT_BOTTOM));
		}
	}

	return mode;
}

SDL_bool debug_surface_isMenu(void)
{
	SDL_bool ret = SDL_TRUE;

	if(items == DEBUG_SURFACE_EXEC) ret = SDL_FALSE;

	return ret;
}

