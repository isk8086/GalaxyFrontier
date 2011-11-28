#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

#define EARTH_FRAME (60.0f * 30.0f)
#define DEFAULT_XVEL (0.0f)
#define DEFAULT_YVEL ((640.0f - (float)SCR_Y) / EARTH_FRAME)

static void tsk_bg01_draw(TSK *tp);
static void tsk_bg01_exit(TSK *tp);
static void tsk_bg01_init(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0, 640, 640 },
};

static TSK *bg_tskp = NULL;

void tsk_bg01(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			bg_tskp = tp;
			tp->draw_adr = tsk_bg01_draw;
			tp->exit_adr = tsk_bg01_exit;
			tp->pos[X] = (float)SCR_HX;
			tp->pos[Y] = (float)SCR_HY - ((640.0f - (float)SCR_Y) / 2.0f);
			tsk_bg01_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			break;

		case STEP_MAIN+1:
			tp->vel[X] = DEFAULT_XVEL;
			tp->vel[Y] = DEFAULT_YVEL;
			tp->step++;
			break;
		case STEP_MAIN+2:
			tp->pos[Y] += tp->vel[Y];
			if(tp->wait != 0){
				tp->wait--;
				tp->vel[Y] += tp->acc[Y];
			}
			if(tp->pos[Y] < (float)(SCR_Y + (SCR_HY / 2))) break;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_bg01_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_BG01], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_bg01_exit(TSK *tp)
{
	bg_tskp = NULL;
}

void tsk_bg01_init(TSK *tp)
{
	tp->pat = 0;
	tp->wait = 0;
	tp->acc[X] = 0.0f;
	tp->acc[Y] = 0.0f;
}

void set_bg01_acc(float x, float y, int frame)
{
	if(bg_tskp != NULL){
		bg_tskp->wait = frame;
		bg_tskp->acc[X] = x;
		bg_tskp->acc[Y] = y;
	}
}
