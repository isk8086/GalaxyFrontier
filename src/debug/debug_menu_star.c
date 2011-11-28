#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"

enum{
	DEBUG_STAR_INIT = 0,

	DEBUG_STAR_START = DEBUG_STAR_INIT,
	DEBUG_STAR_TYPE,
	DEBUG_STAR_COUNT,
	DEBUG_STAR_RADIAN,
	DEBUG_STAR_ROTATE,
	DEBUG_STAR_EXIT,

	DEBUG_STAR_EXEC,

	DEBUG_STAR_MAX
};

static SDL_bool debug_star_isMenu(void);

static SDL_bool initialized = SDL_FALSE;

static char *star_type[] = {
	"STABLE",
	"VERTICAL",
	"ROTATE",
};

static MENUdata menu_data[] = {
	{ SDL_FALSE, "*** STAR MENU ***",        NULL,   -1,    -1,  -1, -1 },
	{ SDL_TRUE,  "   TEST START",            NULL,   -1,    -1,  -1, -1 },
	{ SDL_TRUE,  "    STAR TYPE",       star_type,    0,     3,   1, -1 },
	{ SDL_TRUE,  "   STAR COUNT",       value_int,    1,   256,   1, -1 },
	{ SDL_TRUE,  "  STAR RADIAN",     value_float, 1000, 50000, 100, -1 },	/* 0.1000f ~ 7.0000f */
	{ SDL_TRUE,  "  STAR ROTATE",     value_float,    0,   700,  10, -1 },	/* 0.0000f ~ 0.0700f */
	{ SDL_TRUE,  "    TEST EXIT",            NULL,   -1,    -1,  -1, -1 },
};

static MENUdata *menu_p = & menu_data[1];

static int items;
static int items_min;
static int items_max;

static int star_cnt;
static float rad_add;
static float rot_add;

static int disp_blink;

void debug_menu_star_init(void)
{
	int i;

	if(initialized == SDL_FALSE){
		initialized = SDL_TRUE;
		items_min = 0;
		items_max = sizeof(menu_data) / sizeof(MENUdata) - 1;
		for(i = 0; i <= items_max; i++){
			menu_data[i].now = 0;
		}
		star_cnt = 128;
		printf("star menu init %d(%d/%d)\n", items, items_min, items_max);
	}
	items = items_min;
	rad_add = 2.0000f;
	rot_add = 0.0350f;
	menu_p[DEBUG_STAR_COUNT].now = star_cnt;
	menu_p[DEBUG_STAR_RADIAN].now = (int)(rad_add * 10000.0f);
	menu_p[DEBUG_STAR_ROTATE].now = (int)(rot_add * 10000.0f);
	disp_blink = 0;
}

int debug_menu_star(int mode)
{
	int ret;

	if(debug_star_isMenu() == SDL_TRUE){
		ret = MENUexec(menu_data, &items, items_max);
		star_cnt = menu_p[DEBUG_STAR_COUNT].now;
		rad_add = (float)menu_p[DEBUG_STAR_RADIAN].now / 10000.0f;
		rot_add = (float)menu_p[DEBUG_STAR_ROTATE].now / 10000.0f;
		if(menu_p[DEBUG_STAR_TYPE].now == 2){
			menu_p[DEBUG_STAR_RADIAN].select = SDL_TRUE;
			menu_p[DEBUG_STAR_ROTATE].select = SDL_TRUE;
		}else{
			menu_p[DEBUG_STAR_RADIAN].select = SDL_FALSE;
			menu_p[DEBUG_STAR_ROTATE].select = SDL_FALSE;
		}
		if(ret == 0){
			if(mode != DEBUG_MENU_EXEC) mode = DEBUG_MENU_EXEC;
		}else if(ret == 1){
			switch(items){
				case DEBUG_STAR_START:	/* TEST START */
					switch(menu_p[DEBUG_STAR_TYPE].now){
						case 0:
							set_star_stable(star_cnt);
							break;
						case 1:
							set_star_vertical(star_cnt);
							break;
						case 2:
							set_star_rolling(star_cnt, rad_add, rot_add);
							break;
						default:
							break;
					}
					items = DEBUG_STAR_EXEC;
					break;
				case DEBUG_STAR_TYPE:	/* STAR TYPE */
					break;
				case DEBUG_STAR_EXIT:	/* TEST EXIT */
					mode = DEBUG_MENU_TOP;
					break;

				default:
					break;
			}
		}
	}else{
		if(debug_key_isCancel() == SDL_TRUE){
			TSKclearAll(NULL);
			items = DEBUG_STAR_START;
		}
	}

	return mode;
}

int debug_menu_star_draw(int mode)
{
	if(items != DEBUG_STAR_EXEC){
		MENUdraw(menu_data, items, items_max);
	}else{
		disp_blink++;
		if(disp_blink & 0x10){
			SDL_wheel_DispAscii_Bank(FONT_16x16, "'Q' KEY EXIT.", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
		}
	}

	return mode;
}

SDL_bool debug_star_isMenu(void)
{
	SDL_bool ret = SDL_TRUE;

	if(items == DEBUG_STAR_EXEC) ret = SDL_FALSE;

	return ret;
}

