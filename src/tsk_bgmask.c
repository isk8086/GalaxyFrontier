#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

static void tsk_bgmask_draw(TSK *tp);
static void tsk_bgmask_exit(TSK *tp);
static void tsk_bgmask_init(TSK *tp);
static void tsk_bgmask_move(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0, 640, 480 },
};

static TSK *bg_tskp = NULL;

static float bg_alpha;
static float bg_alpha_add;

void tsk_bgmask(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			bg_tskp = tp;
			tp->draw_adr = tsk_bgmask_draw;
			tp->exit_adr = tsk_bgmask_exit;
			tsk_bgmask_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tsk_bgmask_move(tp);
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_bgmask_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];

	if(bg_alpha <= 0.0f) return;
	SDL_wheel_SetAlpha(surface[SURFACE_MASK], (int)(bg_alpha * 255.0f));
	SDL_wheel_DispSurface(surface[SURFACE_MASK], (Sint16)tp->pos[X], (Sint16)tp->pos[Y], p->x, p->y, p->w, p->h);
}

void tsk_bgmask_exit(TSK *tp)
{
	bg_tskp = NULL;
}

void tsk_bgmask_init(TSK *tp)
{
	tp->pos[X] = (float)SCR_HX;
	tp->pos[Y] = (float)SCR_HY;

	bg_alpha = 1.0f;
	bg_alpha_add = 0.0f;
}

void tsk_bgmask_move(TSK *tp)
{
	if(bg_alpha_add != 0.0f){
		bg_alpha += bg_alpha_add;
		if(bg_alpha >= 1.0f){
			bg_alpha = 1.0f;
			bg_alpha_add = 0.0f;
		}
		if(bg_alpha <= 0.0f){
			bg_alpha = 0.0f;
			bg_alpha_add = 0.0f;
		}
	}
}

void set_bgmask_alpha(float a)
{
	bg_alpha = a;
}

void set_bgmask_fadein(float f)
{
	bg_alpha_add = 1.0f / f;
}

void set_bgmask_fadeout(float f)
{
	bg_alpha_add = -1.0f / f;
}
