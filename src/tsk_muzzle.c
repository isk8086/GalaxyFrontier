#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

static void tsk_muzzle_exit(TSK *tp);
static void tsk_muzzle_draw(TSK *tp);
static void tsk_muzzle_init(TSK *tp);
static void tsk_muzzle_move(TSK *tp);
static void tsk_muzzle_anim(TSK *tp);

static SDL_Rect anim_rect_muzzle[] = {
	{   0,   0,  64,  64 },
	{  64,   0, 128,  64 },
	{ 128,   0, 192,  64 },
	{ 192,   0, 256,  64 },
};

void tsk_muzzle(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_muzzle_exit;
			tp->draw_adr = tsk_muzzle_draw;
			tsk_muzzle_init(tp);
			tsk_muzzle_move(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tsk_muzzle_move(tp);
			tsk_muzzle_anim(tp);
			if(ship_tsk != NULL && tp->pat < 4) break;
			tp->draw_adr = NULL;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_muzzle_exit(TSK *tp)
{
}

void tsk_muzzle_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &anim_rect_muzzle[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_SSHOT_M], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_muzzle_init(TSK *tp)
{
	tp->pat = 0;
	tp->pat_wait = 0;

	tp->ofs[X] += FUNCrandFloat(4.0f) - 2.0f;
	tp->ofs[Y] += FUNCrandFloat(4.0f) - 2.0f - 24.0f;
}

void tsk_muzzle_move(TSK *tp)
{
	TSK *etp = tp->tsk_parent;

	if(etp == NULL) return;

	tp->pos[X] = etp->pos[X] + tp->ofs[X];
	tp->pos[Y] = etp->pos[Y] + tp->ofs[Y];
}

void tsk_muzzle_anim(TSK *tp)
{
	tp->pat++;
}

