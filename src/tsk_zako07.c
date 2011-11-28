#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define ZAKO07_MINX ((float)SCR_LEFT   - 24.0f)
#define ZAKO07_MAXX ((float)SCR_RIGHT  + 24.0f)
#define ZAKO07_MAXY ((float)SCR_BOTTOM + 24.0f)

static void tsk_zako07_exit(TSK *tp);
static void tsk_zako07_intr(TSK *tp);
static void tsk_zako07_draw(TSK *tp);
static void tsk_zako07_init(TSK *tp);
static SDL_bool tsk_zako07_move(TSK *tp);
static SDL_bool tsk_zako07_escape(TSK *tp);
static void tsk_zako07_shot(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0,  64,  64 },
};

static COLLISION zako_collision[] = {
	{ {   +0.0f,   +0.0f }, {  +16.0f,  +16.0f } },
};

void tsk_zako07(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_zako07_exit;
			tp->intr_adr = tsk_zako07_intr;
			tp->draw_adr = tsk_zako07_draw;
			TSKsetCollision(tp, zako_collision);
			tsk_zako07_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_zako07_move(tp) == SDL_TRUE){
				tsk_zako07_shot(tp);
				break;
			}
			tp->step = STEP_EXIT;
			break;

		case STEP_MAIN+1:
			if(tsk_zako07_escape(tp) == SDL_TRUE){
				tsk_zako07_shot(tp);
				break;
			}
			tp->step = STEP_MAIN + 0;
			break;

		default:
			enemy_release_core(tp);
			TSKclear(tp);
			break;
	}
}

void tsk_zako07_exit(TSK *tp)
{
}

void tsk_zako07_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if(etp != NULL){
		if(etp->group_no == GROUP_SSHOT){
			enemy_damage(tp, etp, 100, STEP_EXIT);
			if(tp->energy > 0){
				tp->mode = 0;
				tp->step = STEP_MAIN + 1;
			}
		}else if(etp->group_no == GROUP_NSHOT && (etp->tskid & TSKID_NUCLEAR) != 0){
			enemy_damage(tp, etp, 100, STEP_EXIT);
			if(tp->energy > 0){
				tp->mode = 0;
				tp->step = STEP_MAIN + 1;
			}
		}
	}
}

void tsk_zako07_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_ZAKO07], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_zako07_init(TSK *tp)
{
	tp->energy = 10;

	tp->pos[X] = FUNCrandFloat((float)SCR_X - 96.0f) + 48.0f;
	tp->pos[Y] = -24.0f;
	tp->vel[X] = 0.0f;
	tp->vel[Y] = 2.13725f;
	tp->mode = 0;
	tp->cnt = 0;
	tp->pat = 0;
	tp->dest_rate = ENEMY_DEFAULT_RATE;

	enemy_set_small_core(tp);
}

SDL_bool tsk_zako07_move(TSK *tp)
{
	tp->cnt++;

	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];

	if(tp->pos[X] < ZAKO07_MINX) return SDL_FALSE;
	if(tp->pos[X] > ZAKO07_MAXX) return SDL_FALSE;
	if(tp->pos[Y] > ZAKO07_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

SDL_bool tsk_zako07_escape(TSK *tp)
{
	if(tp->mode == 0){
		tp->trg[X] = FUNCrandFloat((float)SCR_X - 96.0f) + 48.0f;
		tp->num = 0;
		tp->mode = 1;
	}

	tp->pos[X] += (tp->trg[X] - tp->pos[X]) / 5.0f;

	tp->num++;
	tp->cnt++;

	if(tp->num >= 15){
		return SDL_FALSE;
	}

	return SDL_TRUE;
}

void tsk_zako07_shot(TSK *tp)
{
	if((tp->cnt % 15) == 0){
		enemy_shot_1way(tp, 0, 0);
	}
}
