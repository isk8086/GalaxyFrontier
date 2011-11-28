#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"

enum{
	DEBUG_SEQUENCE_INIT = 0,

	DEBUG_SEQUENCE_NUM = DEBUG_SEQUENCE_INIT,
	DEBUG_SEQUENCE_CONTINUE,
	DEBUG_SEQUENCE_DEMO,
	DEBUG_SEQUENCE_EXIT,

	DEBUG_SEQUENCE_EXEC,

	DEBUG_SEQUENCE_MAX
};

static SDL_bool debug_sequence_isMenu(void);

static SDL_bool initialized = SDL_FALSE;

static char *stg_type[] = {
	"SORTIE",
	"SCENE-1",
	"SCENE-2",
	"SCENE-3",

	"SCENE-TEST",
};

static char *seq_type[] = {
	"OFF",
	"ON",
};

static char *demo_type[] = {
	"OFF",
	"ON",
};

static MENUdata menu_data[] = {
	{ SDL_FALSE, "*** SEQUENCE MENU ***",      NULL,       -1,      -1,  -1, -1 },
	{ SDL_TRUE,  "  SEQUENCE NO      ",          stg_type, STG_INIT, STG_MAX,   1, -1 },
	{ SDL_TRUE,  "  SEQUENCE CONTINUE",          seq_type,        0,       2,   1, -1 },
	{ SDL_TRUE,  "  DEMO PLAY        ",         demo_type,        0,       2,   1, -1 },
	{ SDL_TRUE,  "  TEST EXIT        ",              NULL,       -1,      -1,  -1, -1 },
};

static MENUdata *menu_p = & menu_data[1];

static int items;
static int items_min;
static int items_max;

static int seq_num;

static int disp_blink;

void debug_menu_sequence_init(void)
{
	int i;

	if(initialized == SDL_FALSE){
		initialized = SDL_TRUE;
		items_min = 0;
		items_max = sizeof(menu_data) / sizeof(MENUdata) - 1;
		for(i = 0; i <= items_max; i++){
			menu_data[i].now = 0;
		}
		printf("sequence menu init %d(%d/%d)\n", items, items_min, items_max);
	}
	items = items_min;
	menu_p[DEBUG_SEQUENCE_NUM].now = STG_INIT;
	seq_num = -1;
	disp_blink = 0;
}

int debug_menu_sequence(int mode)
{
	int ret;

	if(debug_sequence_isMenu() == SDL_TRUE){
		ret = MENUexec(menu_data, &items, items_max);
		if(seq_num != menu_p[DEBUG_SEQUENCE_NUM].now){
			seq_num = menu_p[DEBUG_SEQUENCE_NUM].now;
		}
		if(menu_p[DEBUG_SEQUENCE_DEMO].now == 1){
			demo_play = SDL_TRUE;
		}else{
			demo_play = SDL_FALSE;
		}
		if(menu_p[DEBUG_SEQUENCE_CONTINUE].now == 1){
			seq_continue = SDL_TRUE;
		}else{
			seq_continue = SDL_FALSE;
		}
		if(ret == 0){
			if(mode != DEBUG_MENU_EXEC) mode = DEBUG_MENU_EXEC;
		}else if(ret == 1){
			switch(items){
				case DEBUG_SEQUENCE_NUM:		/* SEQUENCE NO */
					tsk_gctrl_start(NULL, seq_num);
					items = DEBUG_SEQUENCE_EXEC;
					break;
				case DEBUG_SEQUENCE_CONTINUE:	/* DEMO PLAY */
					break;
				case DEBUG_SEQUENCE_DEMO:		/* DEMO PLAY */
					break;
				case DEBUG_SEQUENCE_EXIT:		/* TEST EXIT */
					mode = DEBUG_MENU_TOP;
					break;

				default:
					break;
			}
		}
	}else{
		if(debug_key_isCancel() == SDL_TRUE){
			tsk_gctrl_release(NULL);
			items = DEBUG_SEQUENCE_NUM;
		}else{
			if(pause_skip == SDL_TRUE || pause == SDL_FALSE){
				if(time_exec == SDL_TRUE) game_time++;
				STAGEexec();
			}
		}
	}

	return mode;
}

int debug_menu_sequence_draw(int mode)
{
	if(items != DEBUG_SEQUENCE_EXEC){
		MENUdraw(menu_data, items, items_max);
	}else{
		disp_blink++;
		if(disp_blink & 0x40){
			SDL_wheel_DispAscii_Bank(FONT_16x16, "'Q' KEY EXIT.", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
		}else{
			if(STAGEisExec() == SDL_TRUE){
				SDL_wheel_DispAscii_Bank(FONT_16x16, "NOW EXECUTE...", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
			}else{
				SDL_wheel_DispAscii_Bank(FONT_16x16, "EXECUTE STOP", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
			}
		}
	}

	return mode;
}

SDL_bool debug_sequence_isMenu(void)
{
	SDL_bool ret = SDL_TRUE;

	if(items == DEBUG_SEQUENCE_EXEC) ret = SDL_FALSE;

	return ret;
}

