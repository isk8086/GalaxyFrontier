#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

enum{
	TOPMENU_INIT = 0,

	TOPMENU_START = TOPMENU_INIT,
	TOPMENU_RESULT,
	TOPMENU_OPTION,
	TOPMENU_EXIT,

	TOPMENU_BOTTOM
};

static void tsk_menu_draw(TSK *tp);
static void tsk_menu_exit(TSK *tp);
static void tsk_menu_init(TSK *tp);

static float item_pos[TOPMENU_BOTTOM];
static float item_trg[TOPMENU_BOTTOM];

static int cursor;

static char *menu_str[] = {
	"  GAME START      ",
	"RESULT INFORMATION",
	"  GAME OPTION     ",
	"  GAME EXIT       ",
};

static SDL_bool menu_draw;

void tsk_menu(TSK *tp)
{
	int i;

	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_menu_draw;
			tp->exit_adr = tsk_menu_exit;
			tsk_menu_init(tp);
			is_start = SDL_FALSE;
			tp->mode = 0;
			tp->cnt = 0;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			for(i = 0; i < TOPMENU_BOTTOM; i++){
				item_pos[i] += (item_trg[i] - item_pos[i]) / 5.0f;
			}
			tp->cnt++;
			if(tp->cnt < 30) break;
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
			if(cursor < 0) cursor = TOPMENU_BOTTOM - 1;
			if(cursor >= TOPMENU_BOTTOM) cursor = 0;
			if(trgs & PAD_BUTTON1){
				SDL_wheel_PlaySe(SE_CORRECT);
				switch(cursor){
					case TOPMENU_START:
						tp->step = STEP_MAIN + 100;
						break;
					case TOPMENU_RESULT:
						tp->step = STEP_MAIN + 200;
						break;
					case TOPMENU_OPTION:
						tp->step = STEP_MAIN + 300;
						break;
					case TOPMENU_EXIT:
						tp->step = STEP_MAIN + 1000;
						break;
					default:
						break;
				}
				break;
			}
			if(trgs & PAD_BUTTON2){
				SDL_wheel_PlaySe(SE_CANCEL);
				tp->step = STEP_EXIT;
				break;
			}
			break;

		/* game start */
		case STEP_MAIN+100:
			SDL_wheel_SetFade(FADE_OUT_BLACK, 60);
			tp->cnt = 0;
			tp->step++;
		case STEP_MAIN+101:
			tp->cnt++;
			if(tp->cnt < 60) break;
			is_start = SDL_TRUE;
			logo_disp = SDL_FALSE;
			TSKclearGroupName(tp, GROUP_BG, NAME_STAR);
			tp->step = STEP_EXIT;
			break;

		/* result indormation */
		case STEP_MAIN+200:
			SDL_wheel_SetFade(FADE_OUT_BLACK, 60);
			if(SDL_wheel_IsPlayMusic() == SDL_TRUE) SDL_wheel_SetFadeMusic(1, 60);
			tp->cnt = 0;
			tp->step++;
		case STEP_MAIN+201:
			tp->cnt++;
			if(tp->cnt < 60) break;
			TSKclearGroupName(tp, GROUP_BG, NAME_STAR);
			TSKsetTopName(tsk_result, GROUP_INFO, NAME_RSLT);
			logo_disp = SDL_FALSE;
			menu_draw = SDL_FALSE;
			tp->step++;
			break;
		case STEP_MAIN+202:
			if(TSKgetName(NAME_RSLT) != NULL) break;
			if(SDL_wheel_IsPlayMusic() == SDL_TRUE) break;
			SDL_wheel_PlayMusic(BGM_OPENING, SDL_FALSE);
			menu_draw = SDL_TRUE;
			SDL_wheel_SetFade(FADE_IN_BLACK, 30);
			set_star_vertical(256);
			logo_disp = SDL_TRUE;
			tp->step = STEP_MAIN + 1;
			break;

		/* option */
		case STEP_MAIN+300:
			SDL_wheel_SetFadeMusic(1, 60);
			menu_draw = SDL_FALSE;
			tp->step++;
		case STEP_MAIN+301:
			if(SDL_wheel_IsPlaySe(0) == SDL_TRUE) break;
			TSKsetBtmName(tsk_option, GROUP_INFO, NAME_OPTN);
			tp->step++;
			break;
		case STEP_MAIN+302:
			if(TSKgetName(NAME_OPTN) != NULL) break;
			SDL_wheel_PlayMusic(BGM_OPENING, SDL_FALSE);
			menu_draw = SDL_TRUE;
			tp->step = STEP_MAIN + 1;
			break;

		/* game exit */
		case STEP_MAIN+1000:
			if(SDL_wheel_IsPlaySe(0) == SDL_TRUE) break;
			is_exec = SDL_FALSE;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_menu_draw(TSK *tp)
{
	int i;
	Sint16 y;

	if(menu_draw == SDL_TRUE){
		y = 256;
		for(i = 0; i < TOPMENU_BOTTOM; i++){
			if(i != cursor) SDL_wheel_DispAscii_Bank(FONT_16x16G, menu_str[i], (Sint16)item_pos[i], y);
			else SDL_wheel_DispAscii_Bank(FONT_16x16, menu_str[i], (Sint16)item_pos[i], y);
			y += 32;
		}
	}

	SDL_wheel_DrawFade();
}

void tsk_menu_exit(TSK *tp)
{
}

void tsk_menu_init(TSK *tp)
{
	int i;

	cursor = -1;

	for(i = 0; i < TOPMENU_BOTTOM; i++){
		if((i % 2) == 0){
			item_pos[i] = -SDL_wheel_GetWidthAscii_Bank(FONT_16x16, menu_str[0]);
		}else{
			item_pos[i] = +640;
		}
		item_trg[i] = (float)(320 - (SDL_wheel_GetWidthAscii_Bank(FONT_16x16, menu_str[0]) / 2));
	}

	menu_draw = SDL_TRUE;
}
