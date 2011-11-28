#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"

enum{
	DEBUG_TOP_INIT = 0,

	DEBUG_TOP_START = DEBUG_TOP_INIT,
	DEBUG_TOP_STAR,
	DEBUG_TOP_SHIP,
	DEBUG_TOP_SCROLL,
	DEBUG_TOP_ENEMY,
	DEBUG_TOP_SEQUENCE,
	DEBUG_TOP_EFFECT,
	DEBUG_TOP_SURFACE,
	DEBUG_TOP_SOUND,
	DEBUG_TOP_EXIT,

	DEBUG_TOP_MAX
};

static SDL_bool initialized = SDL_FALSE;

static MENUdata menu_data[] = {
	{ SDL_FALSE, "*** DEBUG MENU ***",  NULL, -1, -1, -1, -1 },
	{ SDL_TRUE,  "      GAME START",    NULL, -1, -1, -1, -1 },
	{ SDL_TRUE,  "      STAR TEST",     NULL, -1, -1, -1, -1 },
	{ SDL_TRUE,  "      SHIP TEST",     NULL, -1, -1, -1, -1 },
	{ SDL_FALSE, "    SCROLL TEST",     NULL, -1, -1, -1, -1 },
	{ SDL_TRUE,  "     ENEMY TEST",     NULL, -1, -1, -1, -1 },
	{ SDL_TRUE,  "  SEQUENCE TEST",     NULL, -1, -1, -1, -1 },
	{ SDL_FALSE, "    EFFECT TEST",     NULL, -1, -1, -1, -1 },
	{ SDL_TRUE,  "   SURFACE TEST",     NULL, -1, -1, -1, -1 },
	{ SDL_TRUE , "     SOUND TEST",     NULL, -1, -1, -1, -1 },
	{ SDL_TRUE,  "      GAME EXIT",     NULL, -1, -1, -1, -1 },
};

static MENUdata *menu_p = & menu_data[1];

static int items;
static int items_min;
static int items_max;

void debug_menu_top_init(void)
{
	int i;

	if(initialized == SDL_FALSE){
		initialized = SDL_TRUE;
		items_min = 0;
		items_max = sizeof(menu_data) / sizeof(MENUdata) - 1;
		items = items_min;
		for(i = 0; i <= items_max; i++){
			menu_data[i].now = 0;
		}
		printf("menu top init %d(%d/%d)\n", items, items_min, items_max);
	}
}

int debug_menu_top(int mode)
{
	int ret;

	ret = MENUexec(menu_data, &items, items_max);
	if(ret == 0){
		if(mode != DEBUG_MENU_EXEC) mode = DEBUG_MENU_EXEC;
	}else if(ret == 1){
		switch(items){
			case DEBUG_TOP_START:		/* GAME START */
				mode = -1;
				break;
			case DEBUG_TOP_STAR:		/* STAR TEST */
				mode = DEBUG_MENU_STAR;
				break;
			case DEBUG_TOP_SHIP:		/* SHIP TEST */
				mode = DEBUG_MENU_SHIP;
				break;
			case DEBUG_TOP_SCROLL:		/* SCROLL TEST */
				break;
			case DEBUG_TOP_ENEMY:		/* ENEMY TEST */
				mode = DEBUG_MENU_ENEMY;
				break;
			case DEBUG_TOP_SEQUENCE:	/* SEQUENCE TEST */
				mode = DEBUG_MENU_SEQUENCE;
				break;
			case DEBUG_TOP_EFFECT:		/* EFFECT TEST */
				break;
			case DEBUG_TOP_SURFACE:		/* SURFACE TEST */
				mode = DEBUG_MENU_SURFACE;
				break;
			case DEBUG_TOP_SOUND:		/* SOUND TEST */
				mode = DEBUG_MENU_SOUND;
				break;
			case DEBUG_TOP_EXIT:		/* GAME EXIT */
				mode = -2;
				break;
	
			default:
				break;
		}
	}

	return mode;
}

int debug_menu_top_draw(int mode)
{
	MENUdraw(menu_data, items, items_max);

	SDL_wheel_DispAscii_Bank(FONT_16x16, APP_NAME, 16, SCR_Y - (FONT_H * 3 + FONT_BOTTOM));
	SDL_wheel_DispAscii_Bank(FONT_16x16, COPYRIGHT_NAME, 16, SCR_Y - (FONT_H * 2 + FONT_BOTTOM));
	SDL_wheel_DispAscii_Bank(FONT_16x16, COOPERATION_NAME, 16, SCR_Y - (FONT_H * 1 + FONT_BOTTOM));

	return mode;
}
