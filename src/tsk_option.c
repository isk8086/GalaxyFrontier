#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

enum{
	OPTMENU_INIT = 0,

	OPTMENU_GAMEMODE = OPTMENU_INIT,
	OPTMENU_VOLBGM,
	OPTMENU_VOLSE,
	OPTMENU_EXIT,

	OPTMENU_BOTTOM
};

static void tsk_option_draw(TSK *tp);
static void tsk_option_exit(TSK *tp);
static void tsk_option_init(TSK *tp);
static void tsk_option_solumn_str(TSK *tp);

static float item_pos[OPTMENU_BOTTOM * 2];
static float item_trg[OPTMENU_BOTTOM * 2];

static int cursor;
static int column[OPTMENU_BOTTOM];

static char *menu_str[] = {
	"  GAME MODE ",
	" BGM VOLUME ",
	"  SE VOLUME ",
	"       EXIT ",
};

static char column_str[OPTMENU_BOTTOM][16];

static int test_se;

void tsk_option(TSK *tp)
{
	int i;

	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_option_draw;
			tp->exit_adr = tsk_option_exit;
			tsk_option_init(tp);
			is_start = SDL_FALSE;
			tp->mode = 0;
			tp->cnt = 0;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			for(i = 0; i < (OPTMENU_BOTTOM * 2); i++){
				item_pos[i] += (item_trg[i] - item_pos[i]) / 5.0f;
			}
			tp->cnt++;
			if(tp->cnt < 30) break;
			SDL_wheel_StopMusic();
			cursor = 0;
			tp->step++;
			break;
		case STEP_MAIN+1:
			if(reps & PAD_DOWN){
				SDL_wheel_PlaySe(SE_CURSOR);
				cursor++;
			}
			if(reps & PAD_UP){
				SDL_wheel_PlaySe(SE_CURSOR);
				cursor--;
			}
			switch(cursor){
				case OPTMENU_GAMEMODE:
				case OPTMENU_EXIT:
					SDL_wheel_StopMusic();
					SDL_wheel_StopSe(SE_DAMAGE);
					SDL_wheel_StopSe(SE_EXPLODE_S);
					SDL_wheel_StopSe(SE_EXPLODE_M);
					SDL_wheel_StopSe(SE_EXPLODE_L);
					test_se = 0;
					break;
				case OPTMENU_VOLBGM:
				case OPTMENU_VOLSE:
/*
					if(SDL_wheel_IsPlayMusic() == 0){
						SDL_wheel_PlayMusic(BGM_STAGE, 0);
					}
					if(SDL_wheel_IsPlaySe(test_se + 1) == 0){
						test_se++;
						if(test_se >= 4) test_se = 0;
						SDL_wheel_PlaySe(SE_DAMAGE + test_se);
					}
*/
					break;
				default:
					break;
			}
			if((reps & (PAD_LEFT|PAD_RIGHT)) != 0){
				switch(cursor){
					case OPTMENU_GAMEMODE:
						SDL_wheel_PlaySe(SE_COLUMN);
						column[OPTMENU_GAMEMODE]++;
						column[OPTMENU_GAMEMODE] %= 2;
						break;
					case OPTMENU_VOLBGM:
					case OPTMENU_VOLSE:
						SDL_wheel_PlaySe(SE_COLUMN);
						if(reps & PAD_RIGHT) column[cursor]++;
						if(reps & PAD_LEFT) column[cursor]--;
						if(column[cursor] > SND_VOL_RATE) column[cursor] = 0;
						if(column[cursor] < 0) column[cursor] = SND_VOL_RATE;
						SDL_wheel_SetVolMusic(column[OPTMENU_VOLBGM]);
						SDL_wheel_SetVolSe(column[OPTMENU_VOLSE]);
						break;
					default:
						break;
				}
				tsk_option_solumn_str(tp);
			}
			if(cursor < 0) cursor = OPTMENU_BOTTOM - 1;
			if(cursor >= OPTMENU_BOTTOM) cursor = 0;
			if(trgs & PAD_BUTTON1){
				switch(cursor){
					case OPTMENU_EXIT:
						SDL_wheel_PlaySe(SE_CORRECT);
						config.game_mode = column[OPTMENU_GAMEMODE];
						config.vol_bgm = column[OPTMENU_VOLBGM];
						config.vol_se = column[OPTMENU_VOLSE];
						game_config_update();
						tp->draw_adr = NULL;
						tp->step = STEP_MAIN + 100;
						break;
					default:
						break;
				}
				break;
			}
			if(trgs & PAD_BUTTON2){
				SDL_wheel_StopMusic();
				SDL_wheel_StopSe(SE_DAMAGE);
				SDL_wheel_StopSe(SE_EXPLODE_S);
				SDL_wheel_StopSe(SE_EXPLODE_M);
				SDL_wheel_StopSe(SE_EXPLODE_L);
				SDL_wheel_SetVolMusic(config.vol_bgm);
				SDL_wheel_SetVolSe(config.vol_se);
				SDL_wheel_PlaySe(SE_CANCEL);
				tp->draw_adr = NULL;
				tp->step = STEP_MAIN + 100;
				break;
			}
			break;

		/* exit */
		case STEP_MAIN+100:
			if(SDL_wheel_IsPlaySe(0) == SDL_TRUE) break;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_option_draw(TSK *tp)
{
	int i;
	Sint16 y;

	y = 256;
	for(i = 0; i < OPTMENU_BOTTOM; i++){
		if(i != cursor){
			SDL_wheel_DispAscii_Bank(FONT_16x16G, menu_str[i], (Sint16)item_pos[i], y);
			SDL_wheel_DispAscii_Bank(FONT_16x16G, column_str[i], (Sint16)item_pos[i + OPTMENU_BOTTOM], y);
		}else{
			SDL_wheel_DispAscii_Bank(FONT_16x16, menu_str[i], (Sint16)item_pos[i], y);
			SDL_wheel_DispAscii_Bank(FONT_16x16, column_str[i], (Sint16)item_pos[i + OPTMENU_BOTTOM], y);
		}
		y += 32;
	}
}

void tsk_option_exit(TSK *tp)
{
}

void tsk_option_init(TSK *tp)
{
	int i;

	cursor = -1;

	column[OPTMENU_GAMEMODE] = config.game_mode;
	column[OPTMENU_VOLBGM] = config.vol_bgm;
	column[OPTMENU_VOLSE] = config.vol_se;
	column[OPTMENU_EXIT] = -1;

	tsk_option_solumn_str(tp);

	for(i = 0; i < OPTMENU_BOTTOM; i++){
		item_pos[i] = -SDL_wheel_GetWidthAscii_Bank(FONT_16x16, menu_str[0]);
		item_trg[i] = (float)(320 - SDL_wheel_GetWidthAscii_Bank(FONT_16x16, menu_str[0]));
	}

	for(i = OPTMENU_BOTTOM; i < (OPTMENU_BOTTOM * 2); i++){
		item_pos[i] = 640.0f;
		item_trg[i] = 320.0f;
	}
}

void tsk_option_solumn_str(TSK *tp)
{
	strcpy(column_str[OPTMENU_GAMEMODE], column[OPTMENU_GAMEMODE] == GAMEMODE_SUPERHARD ? "SUPER-HARD" : "SUPER-EASY");
	sprintf(column_str[OPTMENU_VOLBGM], "%d", column[OPTMENU_VOLBGM]);
	sprintf(column_str[OPTMENU_VOLSE], "%d", column[OPTMENU_VOLSE]);
	strcpy(column_str[OPTMENU_EXIT], "");
}

