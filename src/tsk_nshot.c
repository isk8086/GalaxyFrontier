#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define NSHOT_MINX ((float)SCR_LEFT   - 16.0f)
#define NSHOT_MAXX ((float)SCR_RIGHT  + 16.0f)
#define NSHOT_MINY ((float)SCR_TOP    - 16.0f)
#define NSHOT_MAXY ((float)SCR_BOTTOM + 16.0f)

#define NSHOT_VEL (27.132751f)

static void tsk_nshot_exit(TSK *tp);
static void tsk_nshot_intr(TSK *tp);
static void tsk_nshot_draw(TSK *tp);
static void tsk_nshot_init(TSK *tp);
static SDL_bool tsk_nshot_move(TSK *tp);
static void tsk_nshot_nuclear(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0,  16,   32 },
	{  16,   0,  40,   40 },
	{  40,   0,  72,   48 },
};

static COLLISION nshot_collision1[] = {
	{ {   +0.0f,   +0.0f }, {   +8.0f,   +16.0f } },
};

static COLLISION nshot_collision2[] = {
	{ {   +0.0f,   +0.0f }, {  +12.0f,   +20.0f } },
};

static COLLISION nshot_collision3[] = {
	{ {   +0.0f,   +0.0f }, {  +16.0f,   +24.0f } },
};

void tsk_nshot(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			if(nshot_level < 0 || nshot_level > 3){
				tp->step = STEP_EXIT;
				break;
			}
			tp->tskid |= TSKID_NUCLEAR;
			tp->exit_adr = tsk_nshot_exit;
			tp->intr_adr = tsk_nshot_intr;
			tp->draw_adr = tsk_nshot_draw;
			if(nshot_level == 1) TSKsetCollision(tp, nshot_collision1);
			if(nshot_level == 2) TSKsetCollision(tp, nshot_collision2);
			if(nshot_level == 3) TSKsetCollision(tp, nshot_collision3);
			tsk_nshot_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_nshot_move(tp) == SDL_TRUE) break;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_nshot_exit(TSK *tp)
{
}

void tsk_nshot_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if(etp != NULL){
		if(etp->group_no == GROUP_ESHOT){
			bonus_score++;
		}
		enemy_score(10);
		tsk_nshot_nuclear(tp);
		tp->intr_adr = NULL;
		tp->draw_adr = NULL;
		tp->step = STEP_EXIT;
	}
}

void tsk_nshot_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_NSHOT], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_nshot_init(TSK *tp)
{
	tp->energy = 1;
	tp->vel[Y] = -NSHOT_VEL;
	tp->pat = nshot_level - 1;
}

SDL_bool tsk_nshot_move(TSK *tp)
{
	tp->pos[Y] += tp->vel[Y];

	if(tp->pos[X] < NSHOT_MINX) return SDL_FALSE;
	if(tp->pos[X] > NSHOT_MAXX) return SDL_FALSE;
	if(tp->pos[Y] < NSHOT_MINY) return SDL_FALSE;
	if(tp->pos[Y] > NSHOT_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

void tsk_nshot_nuclear(TSK *tp)
{
	TSK *etp;

	etp = TSKsetTopName(tsk_nuclear, GROUP_NSHOT, NAME_NSHOT);
	if(etp != NULL){
		etp->pos[X] = tp->pos[X];
		etp->pos[Y] = tp->pos[Y];
		TSKexecDirect(etp);
		tp->wait = 3;
		tp->cnt--;
	}
}
