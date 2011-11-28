#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define ESHOT_MINX ((float)SCR_LEFT   - 16.0f)
#define ESHOT_MAXX ((float)SCR_RIGHT  + 16.0f)
#define ESHOT_MINY ((float)SCR_TOP    - 16.0f)
#define ESHOT_MAXY ((float)SCR_BOTTOM + 16.0f)

#define ESHOT_ROT_CNT (16)
#define ESHOT_ROT_RATE ((PI * 2.0f) / (float)ESHOT_ROT_CNT)
#define ESHOT_ROT_HRATE (ESHOT_ROT_RATE / 2.0f)

static void tsk_eshot_exit(TSK *tp);
static void tsk_eshot_intr(TSK *tp);
static void tsk_eshot_draw(TSK *tp);
static void tsk_eshot_init(TSK *tp);
static SDL_bool tsk_eshot_move(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0,  32,  32 },
	{  32,   0,  64,  32 },
	{  64,   0,  96,  32 },
	{  96,   0, 128,  32 },
};

static COLLISION eshot_collision[] = {
	{ {   +0.0f,   +0.0f }, {   +2.0f,   +2.0f } },
};

void tsk_eshot(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_eshot_exit;
			tp->intr_adr = tsk_eshot_intr;
			tp->draw_adr = tsk_eshot_draw;
			TSKsetCollision(tp, eshot_collision);
			tsk_eshot_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_eshot_move(tp) == SDL_TRUE) break;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_eshot_exit(TSK *tp)
{
}

void tsk_eshot_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if(etp){
		tp->intr_adr = NULL;
		tp->step = STEP_EXIT;
		return;
	}
}

void tsk_eshot_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_ESHOT_S + tp->mode], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_eshot_init(TSK *tp)
{
	tp->vel[X] = cos(tp->rot[X]) * eshot_vel;
	tp->vel[Y] = sin(tp->rot[Y]) * eshot_vel;
	tp->pat = 0;
}

SDL_bool tsk_eshot_move(TSK *tp)
{
	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];

#if 0
#else
	tp->pat++;
	tp->pat %= 4;
#endif

	if(tp->pos[X] < ESHOT_MINX) return SDL_FALSE;
	if(tp->pos[X] > ESHOT_MAXX) return SDL_FALSE;
	if(tp->pos[Y] < ESHOT_MINY) return SDL_FALSE;
	if(tp->pos[Y] > ESHOT_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}
