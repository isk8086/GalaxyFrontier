#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"

enum{
	DEBUG_SOUND_INIT = 0,

	DEBUG_SOUND_MUSIC = DEBUG_SOUND_INIT,
	DEBUG_SOUND_SFX,
	DEBUG_SOUND_EXIT,

	DEBUG_SOUND_EXEC,

	DEBUG_SOUND_MAX
};

static SDL_bool debug_sound_isMenu(void);

static SDL_bool initialized = SDL_FALSE;

static MENUdata menu_data[] = {
	{ SDL_FALSE, "*** SOUND MENU ***",      NULL,       -1,      -1,  -1, -1 },
	{ SDL_TRUE,  "  MUSIC NO",           value_int, BGM_INIT, BGM_MAX,   1, -1 },
	{ SDL_TRUE,  "    SFX NO",           value_int,  SE_INIT,  SE_MAX,   1, -1 },
	{ SDL_TRUE,  "   TEST EXIT",              NULL,       -1,      -1,  -1, -1 },
};

static MENUdata *menu_p = & menu_data[1];

static int items;
static int items_min;
static int items_max;

static int music_num;
static int sfx_num;

static int disp_blink;

void debug_menu_sound_init(void)
{
	int i;

	if(initialized == SDL_FALSE){
		initialized = SDL_TRUE;
		items_min = 0;
		items_max = sizeof(menu_data) / sizeof(MENUdata) - 1;
		for(i = 0; i <= items_max; i++){
			menu_data[i].now = 0;
		}
		printf("sound menu init %d(%d/%d)\n", items, items_min, items_max);
	}
	items = items_min;
	menu_p[DEBUG_SOUND_MUSIC].now = BGM_INIT;
	music_num = -1;
	disp_blink = 0;
}

int debug_menu_sound(int mode)
{
	int ret;

	if(debug_sound_isMenu() == SDL_TRUE){
		ret = MENUexec(menu_data, &items, items_max);
		if(music_num != menu_p[DEBUG_SOUND_MUSIC].now){
			music_num = menu_p[DEBUG_SOUND_MUSIC].now;
		}
		if(sfx_num != menu_p[DEBUG_SOUND_SFX].now){
			sfx_num = menu_p[DEBUG_SOUND_SFX].now;
		}
		if(ret == 0){
			if(mode != DEBUG_MENU_EXEC) mode = DEBUG_MENU_EXEC;
		}else if(ret == 1){
			switch(items){
				case DEBUG_SOUND_MUSIC:		/* MUSIC NO */
					SDL_wheel_PlayMusic(music_num, SDL_FALSE);
					items = DEBUG_SOUND_EXEC;
					break;
				case DEBUG_SOUND_SFX:		/* SFX NO */
					SDL_wheel_PlaySe(sfx_num);
					break;
				case DEBUG_SOUND_EXIT:	/* TEST EXIT */
					mode = DEBUG_MENU_TOP;
					break;

				default:
					break;
			}
		}
	}else{
		if(debug_key_isCancel() == SDL_TRUE){
			SDL_wheel_StopMusic();
			items = DEBUG_SOUND_MUSIC;
		}
	}

	return mode;
}

int debug_menu_sound_draw(int mode)
{
	if(items != DEBUG_SOUND_EXEC){
		MENUdraw(menu_data, items, items_max);
	}else{
		disp_blink++;
		if(disp_blink & 0x40){
			SDL_wheel_DispAscii_Bank(FONT_16x16, "'Q' KEY EXIT.", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
		}else{
			if(SDL_wheel_IsPlayMusic() == SDL_TRUE){
				SDL_wheel_DispAscii_Bank(FONT_16x16, "NOW PLAYING...", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
			}else{
				SDL_wheel_DispAscii_Bank(FONT_16x16, "PLAY STOP", 16, SCR_Y - (FONT_H + FONT_BOTTOM));
			}
		}
	}

	return mode;
}

SDL_bool debug_sound_isMenu(void)
{
	SDL_bool ret = SDL_TRUE;

	if(items == DEBUG_SOUND_EXEC) ret = SDL_FALSE;

	return ret;
}

