#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

static void tsk_fade_draw(TSK *tp);
static void tsk_fade_exit(TSK *tp);
static void tsk_fade_init(TSK *tp);

void tsk_fade(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_fade_draw;
			tp->exit_adr = tsk_fade_exit;
			tsk_fade_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tp->wait--;
			if(tp->wait == 0){
				tp->step = STEP_EXIT;
				break;
			}
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_fade_draw(TSK *tp)
{
	SDL_wheel_DrawFade();
}

void tsk_fade_exit(TSK *tp)
{
}

void tsk_fade_init(TSK *tp)
{
	switch(tp->mode){
		case SCRFADE_BIN:
			SDL_wheel_SetFade(FADE_IN_BLACK, tp->wait);
			break;
		case SCRFADE_BOUT:
			SDL_wheel_SetFade(FADE_OUT_BLACK, tp->wait);
			break;
		case SCRFADE_WIN:
			SDL_wheel_SetFade(FADE_IN_WHITE, tp->wait);
			break;
		case SCRFADE_WOUT:
			SDL_wheel_SetFade(FADE_OUT_WHITE, tp->wait);
			break;
		default:
			tp->wait = 0;
			break;
	}

	if(tp->wait < 1) tp->wait = 1;
}
