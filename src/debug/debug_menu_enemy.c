#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"

enum{
	DEBUG_ENEMY_INIT = 0,

	DEBUG_ENEMY_TYPE = DEBUG_ENEMY_INIT,
	DEBUG_ENEMY_CMAX,
	DEBUG_ENEMY_EXIT,

	DEBUG_ENEMY_EXEC,

	DEBUG_ENEMY_MAX
};

static SDL_bool debug_enemy_isMenu(void);

static SDL_bool initialized = SDL_FALSE;

static char *enemy_type[] = {
	"ZAKO_01",
	"ZAKO_02",
	"ZAKO_03",
	"ZAKO_04",
	"ZAKO_05",
	"ZAKO_06",
	"ZAKO_07",
	"ZAKO_08",

	"MIDDLE_01",
	"MIDDLE_02",

	"BOSS_01",
};

static MENUdata menu_data[] = {
	{ SDL_FALSE, "*** ENEMY MENU ***",       NULL,      -1,        -1,  -1, -1 },
	{ SDL_TRUE,  "   ENEMY TYPE",      enemy_type, ZAKO_01, ENEMY_MAX,   1, -1 },
	{ SDL_TRUE,  "    ENEMY MAX",       value_int,       1,        17,   1, -1 },
	{ SDL_TRUE,  "    TEST EXIT",            NULL,      -1,        -1,  -1, -1 },
};

static MENUdata *menu_p = & menu_data[1];

static int items;
static int items_min;
static int items_max;

static int disp_blink;

static int enemy_num;
static int enemy_now;
static int enemy_max;
static int enemy_wait;
static int enemy_serial;
static int enemy_stable;

static COLLISION dummy_collision[] = {
	{ {   +0.0f,   +0.0f }, {   +2.0f,   +2.0f } },
};

void debug_menu_enemy_init(void)
{
	int i;

	if(initialized == SDL_FALSE){
		initialized = SDL_TRUE;
		items_min = 0;
		items_max = sizeof(menu_data) / sizeof(MENUdata) - 1;
		for(i = 0; i <= items_max; i++){
			menu_data[i].now = 0;
		}
		printf("enemy menu init %d(%d/%d)\n", items, items_min, items_max);
	}
	items = items_min;
	disp_blink = 0;
	enemy_num = -1;
	enemy_now = 0;
	enemy_max = 4;
	enemy_wait = 0;
	enemy_serial = 0;
	enemy_stable = 0;
	game_level = 0;
	menu_p[DEBUG_ENEMY_TYPE].now = ZAKO_01;
	menu_p[DEBUG_ENEMY_CMAX].now = enemy_max;
	enemy_reset();
}

int debug_menu_enemy(int mode)
{
	int ret;
	TSK *tp;

	if(debug_enemy_isMenu() == SDL_TRUE){
		ret = MENUexec(menu_data, &items, items_max);
		enemy_max = menu_p[DEBUG_ENEMY_CMAX].now;
		if(enemy_num != menu_p[DEBUG_ENEMY_TYPE].now){
			enemy_num = menu_p[DEBUG_ENEMY_TYPE].now;
			TSKclearGroup(NULL, GROUP_ENEMY);
			TSKclearGroup(NULL, GROUP_ESHOT);
			TSKclearGroup(NULL, GROUP_INFO);
			enemy_reset();
			enemy_serial = 0;
			enemy_wait = 0;
		}
		if(ret == 0){
			if(mode != DEBUG_MENU_EXEC) mode = DEBUG_MENU_EXEC;
		}else if(ret == 1){
			switch(items){
				case DEBUG_ENEMY_TYPE:	/* ENEMY TYPE */
					if(enemy_adr[enemy_num] != NULL){
						tp = TSKsetTopName(tsk_ship, GROUP_SHIP, NAME_SHIP);
						if(tp != NULL){
							TSKexecDirect(tp);
							tp->tskid |= TSKID_NODMG;
						}
						TSKclearGroup(NULL, GROUP_ENEMY);
						TSKclearGroup(NULL, GROUP_ESHOT);
						TSKclearGroup(NULL, GROUP_INFO);
						TSKsetTopName(tsk_info, GROUP_INFO, NAME_INFO);
						enemy_reset();
						enemy_serial = 0;
						enemy_wait = 0;
						enemy_stable = 1;
						items = DEBUG_ENEMY_EXEC;
					}
					break;
				case DEBUG_ENEMY_EXIT:	/* TEST EXIT */
					TSKclearGroup(NULL, GROUP_ENEMY);
					TSKclearGroup(NULL, GROUP_ESHOT);
					mode = DEBUG_MENU_TOP;
					break;

				default:
					break;
			}
		}
	}else{
		if(debug_key_isCancel() == SDL_TRUE){
			TSKclearGroup(NULL, GROUP_SHIP);
			TSKclearGroup(NULL, GROUP_INFO);
			TSKclearGroup(NULL, GROUP_ENEMY);
			TSKclearGroup(NULL, GROUP_EFFECT);
			TSKclearGroup(NULL, GROUP_ESHOT);
			TSKclearGroup(NULL, GROUP_SSHOT);
			items = DEBUG_ENEMY_TYPE;
		}
	}

	if(mode != DEBUG_MENU_TOP){
		enemy_now  = TSKgetGroupNameCnt(GROUP_ENEMY, NAME_ZAKO);
		enemy_now += TSKgetGroupNameCnt(GROUP_ENEMY, NAME_MIDD);
		enemy_now += TSKgetGroupNameCnt(GROUP_ENEMY, NAME_BOSS);
		if(enemy_wait != 0){
			enemy_wait--;
		}else if(enemy_stable != 0){
			if(enemy_now == 0){
				enemy_wait = 0;
				enemy_stable = 0;
				enemy_get_playerside();
			}
		}else if(enemy_now < enemy_max){
			if((tp = TSKsetTopName(enemy_adr[enemy_num], GROUP_ENEMY, NAME_ZAKO)) != NULL){
				tp->serial = enemy_serial;
				TSKexecDirect(tp);
				enemy_now++;
				enemy_serial++;
				enemy_serial %= enemy_max;
				if(enemy_serial == 0){
					enemy_stable = 1;
				}else{
					enemy_wait = 10;
				}
			}
		}
	}

	return mode;
}

int debug_menu_enemy_draw(int mode)
{
	TSK *tp;

	if(items != DEBUG_ENEMY_EXEC){
		MENUdraw(menu_data, items, items_max);
	}else{
		if(pads & PAD_BUTTON3){
			for(tp = TskIndex[GROUP_ENEMY][TSK_TOP].next_tsk; tp != &(TskIndex[GROUP_ENEMY][TSK_BOTTOM]); tp = tp->next_tsk){
				COLLISIONdraw(tp);
			}
			for(tp = TskIndex[GROUP_ESHOT][TSK_TOP].next_tsk; tp != &(TskIndex[GROUP_ESHOT][TSK_BOTTOM]); tp = tp->next_tsk){
				COLLISIONdraw(tp);
			}
		}
		disp_blink++;
		if(disp_blink & 0x40){
			SDL_wheel_DispAscii_Bank(FONT_16x16, "'Q' KEY EXIT.", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
		}else{
			SDL_wheel_DispAscii_Bank(FONT_16x16, "C KEY : DRAW COLLISION", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
		}
	}

	return mode;
}

SDL_bool debug_enemy_isMenu(void)
{
	SDL_bool ret = SDL_TRUE;

	if(items == DEBUG_ENEMY_EXEC) ret = SDL_FALSE;

	return ret;
}
