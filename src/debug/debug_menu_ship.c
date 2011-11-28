#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"

enum{
	DEBUG_SHIP_INIT = 0,

	DEBUG_SHIP_START = DEBUG_SHIP_INIT,
	DEBUG_SHIP_EXIT,

	DEBUG_SHIP_EXEC,

	DEBUG_SHIP_MAX
};

static SDL_bool debug_ship_isMenu(void);

static SDL_bool initialized = SDL_FALSE;

static MENUdata menu_data[] = {
	{ SDL_FALSE, "*** SHIP MENU ***", NULL, -1, -1, -1, -1 },
	{ SDL_TRUE,  "  TEST START",      NULL, -1, -1, -1, -1 },
	{ SDL_TRUE,  "   TEST EXIT",      NULL, -1, -1, -1, -1 },
};

static MENUdata *menu_p = & menu_data[1];

static int items;
static int items_min;
static int items_max;

static int disp_blink;

void debug_menu_ship_init(void)
{
	int i;

	if(initialized == SDL_FALSE){
		initialized = SDL_TRUE;
		items_min = 0;
		items_max = sizeof(menu_data) / sizeof(MENUdata) - 1;
		for(i = 0; i <= items_max; i++){
			menu_data[i].now = 0;
		}
		printf("ship menu init %d(%d/%d)\n", items, items_min, items_max);
	}
	items = items_min;
	disp_blink = 0;
}

int debug_menu_ship(int mode)
{
	int ret;

	if(debug_ship_isMenu() == SDL_TRUE){
		ret = MENUexec(menu_data, &items, items_max);
		if(ret == 0){
			if(mode != DEBUG_MENU_EXEC) mode = DEBUG_MENU_EXEC;
		}else if(ret == 1){
			switch(items){
				case DEBUG_SHIP_START:	/* TEST START */
					set_star_vertical(256);
					TSKsetTopName(tsk_ship, GROUP_SHIP, NAME_SHIP);
					TSKsetTopName(tsk_info, GROUP_INFO, NAME_INFO);
					items = DEBUG_SHIP_EXEC;
					break;
				case DEBUG_SHIP_EXIT:	/* TEST EXIT */
					mode = DEBUG_MENU_TOP;
					break;

				default:
					break;
			}
		}
	}else{
		if(debug_key_isCancel() == SDL_TRUE){
			TSKclearAll(NULL);
			items = DEBUG_SHIP_START;
		}
	}

	return mode;
}

int debug_menu_ship_draw(int mode)
{
	if(items != DEBUG_SHIP_EXEC){
		MENUdraw(menu_data, items, items_max);
	}else{
		disp_blink++;
		if(disp_blink & 0x10){
			SDL_wheel_DispAscii_Bank(FONT_16x16, "'Q' KEY EXIT.", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
		}
	}

	return mode;
}

SDL_bool debug_ship_isMenu(void)
{
	SDL_bool ret = SDL_TRUE;

	if(items == DEBUG_SHIP_EXEC) ret = SDL_FALSE;

	return ret;
}

