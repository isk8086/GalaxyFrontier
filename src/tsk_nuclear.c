#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

static void tsk_nuclear_exit(TSK *tp);
static void tsk_nuclear_intr(TSK *tp);
static void tsk_nuclear_draw(TSK *tp);
static void tsk_nuclear_init(TSK *tp);
static SDL_bool tsk_nuclear_move(TSK *tp);
static void tsk_nuclear_request(TSK *tp);

static SDL_Rect pat_rect[] = {
	{ 144 * 0,   0, 144 * 1, 144 * 1 },
	{ 144 * 1,   0, 144 * 2, 144 * 1 },
	{ 144 * 2,   0, 144 * 3, 144 * 1 },
	{ 144 * 3,   0, 144 * 4, 144 * 1 },
	{ 144 * 4,   0, 144 * 5, 144 * 1 },
	{ 144 * 0, 144, 144 * 1, 144 * 2 },
	{ 144 * 1, 144, 144 * 2, 144 * 2 },
	{ 144 * 2, 144, 144 * 3, 144 * 2 },
	{ 144 * 3, 144, 144 * 4, 144 * 2 },
	{ 144 * 4, 144, 144 * 5, 144 * 2 },
};

static COLLISION nuclear_collision[] = {
	{ {   +0.0f,   +0.0f }, {  +32.0f,  +32.0f } },
};

void tsk_nuclear(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->tskid |= TSKID_NUCLEAR;
			tp->exit_adr = tsk_nuclear_exit;
			tp->intr_adr = NULL;
			tp->draw_adr = tsk_nuclear_draw;
			TSKsetCollision(tp, nuclear_collision);
			tsk_nuclear_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_nuclear_move(tp) == SDL_TRUE) break;
			tp->intr_adr = NULL;
			tp->draw_adr = NULL;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_nuclear_exit(TSK *tp)
{
}

void tsk_nuclear_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if(etp != NULL){
		if(etp->group_no == GROUP_ESHOT){
			bonus_score++;
		}
		enemy_score(10);
		tsk_nuclear_request(tp);
	}
}

void tsk_nuclear_draw(TSK *tp)
{
	SDL_Rect *p;

	if((tp->cnt & 0x01) == 0){
		p = &pat_rect[tp->pat];
		SDL_wheel_DispSurface(surface[SURFACE_NUCLEAR], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
	}
}

void tsk_nuclear_init(TSK *tp)
{
	tp->energy = 1;
	tp->cnt = 0;
	tp->pat = 0;
}

SDL_bool tsk_nuclear_move(TSK *tp)
{
	tp->cnt++;
	if(tp->cnt & 0x01){
		tp->intr_adr = NULL;
	}else{
		tp->intr_adr = tsk_nuclear_intr;
	}

	tp->pat++;
	if(tp->pat >= 10) return SDL_FALSE;

	return SDL_TRUE;
}

void tsk_nuclear_request(TSK *tp)
{
	TSK *etp;

	if(nuclear_exec == SDL_TRUE){
		etp = TSKsetTopName(tsk_nuclear, GROUP_NSHOT, NAME_NSHOT);
		if(etp != NULL){
			etp->pos[X] = tp->pos[X] + FUNCrandFloat(96.0f) - 48.0f;
			etp->pos[Y] = tp->pos[Y] + FUNCrandFloat(96.0f) - 48.0f;
		}
	}
}
