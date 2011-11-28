#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"

static void debug_main_init(int mode);

static int debug_start = DEBUG_MENU_TOP;

static DEBUG_FUNC debug_exec;
static DEBUG_FUNC debug_draw;

int debug_main(int mode)
{
	int ret = 0;

	if(DEBUG_MENU == 0) return 0;

	if(SDL_wheel_GetEvent()->type == SDL_QUIT){
		is_exec = SDL_FALSE;
		return 0;
	}

	if(TskEntCnt == 0 && mode == DEBUG_MENU){
		mode = debug_start;
		debug_main_init(mode);
	}

	if(mode != 0){
		ret = debug_exec(mode);
		if(ret == -1){
			debug_start = mode;
			mode = 0;
			is_exec = SDL_TRUE;
			TSKsetTop(tsk_gctrl, GROUP_SYSTEM);
		}else if(ret == -2){
			mode = -1;
			is_exec = SDL_FALSE;
		}else{
			mode = ret;
			if(mode != DEBUG_MENU_EXEC){
				debug_main_init(mode);
			}else{
				debug_draw(mode);
			}
		}
	}

	return mode;
}

void debug_main_init(int mode)
{
	switch(mode){
		case DEBUG_MENU_TOP:
			pause_f = SDL_TRUE;
			debug_menu_top_init();
			debug_exec = debug_menu_top;
			debug_draw = debug_menu_top_draw;
			break;

		case DEBUG_MENU_STAR:
			debug_menu_star_init();
			debug_exec = debug_menu_star;
			debug_draw = debug_menu_star_draw;
			break;

		case DEBUG_MENU_SHIP:
			debug_menu_ship_init();
			debug_exec = debug_menu_ship;
			debug_draw = debug_menu_ship_draw;
			break;

		case DEBUG_MENU_ENEMY:
			debug_menu_enemy_init();
			debug_exec = debug_menu_enemy;
			debug_draw = debug_menu_enemy_draw;
			break;

		case DEBUG_MENU_SURFACE:
			debug_menu_surface_init();
			debug_exec = debug_menu_surface;
			debug_draw = debug_menu_surface_draw;
			break;

		case DEBUG_MENU_SEQUENCE:
			debug_menu_sequence_init();
			debug_exec = debug_menu_sequence;
			debug_draw = debug_menu_sequence_draw;
			break;

		case DEBUG_MENU_SOUND:
			debug_menu_sound_init();
			debug_exec = debug_menu_sound;
			debug_draw = debug_menu_sound_draw;
			break;

		case DEBUG_MENU_EXEC:
			break;

		default:
			break;
	}
}

SDL_bool debug_key_isCancel(void)
{
	return (trgs & BTN_DEBUG_QUIT) ? SDL_TRUE : SDL_FALSE;
}
