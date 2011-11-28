#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define ZAKO08_MINX ((float)SCR_LEFT   - 24.0f)
#define ZAKO08_MAXX ((float)SCR_RIGHT  + 24.0f)
#define ZAKO08_MAXY ((float)SCR_BOTTOM + 24.0f)

static void tsk_zako08_exit(TSK *tp);
static void tsk_zako08_intr(TSK *tp);
static void tsk_zako08_draw(TSK *tp);
static void tsk_zako08_init(TSK *tp);
static SDL_bool tsk_zako08_move(TSK *tp);
static void tsk_zako08_shot(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0,  64,  64 },
};

static COLLISION zako_collision[] = {
	{ {   +0.0f,   +0.0f }, {  +16.0f,  +16.0f } },
};

void tsk_zako08(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_zako08_exit;
			tp->intr_adr = tsk_zako08_intr;
			tp->draw_adr = tsk_zako08_draw;
			TSKsetCollision(tp, zako_collision);
			tsk_zako08_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_zako08_move(tp) == SDL_TRUE){
				tsk_zako08_shot(tp);
				break;
			}
			tp->step = STEP_EXIT;
			break;

		default:
			enemy_release_core(tp);
			TSKclear(tp);
			break;
	}
}

void tsk_zako08_exit(TSK *tp)
{
}

void tsk_zako08_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if(etp != NULL){
		if(etp->group_no == GROUP_SSHOT){
			enemy_damage(tp, etp, 100, STEP_EXIT);
			if(tp->energy > 0){
				tp->mode = 1;
				tp->acc[X] += tp->vel[X] / 10.0f;
				tp->acc[Y] += tp->vel[Y] / 10.0f;
			}
		}else if(etp->group_no == GROUP_NSHOT && (etp->tskid & TSKID_NUCLEAR) != 0){
			enemy_damage(tp, etp, 100, STEP_EXIT);
			if(tp->energy > 0){
				tp->mode = 1;
				tp->acc[X] += tp->vel[X] / 10.0f;
				tp->acc[Y] += tp->vel[Y] / 10.0f;
			}
		}
	}
}

void tsk_zako08_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_ZAKO08], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_zako08_init(TSK *tp)
{
	tp->energy = 10;

	tp->pos[X] = FUNCrandFloat((float)SCR_X - 96.0f) + 48.0f;
	tp->pos[Y] = -24.0f;
	if(ship_tsk != NULL){
		if(tp->pos[X] < ship_tsk->pos[X]){
			tp->vel[X] = +FUNCrandFloat(3.0f) + 0.75f;
		}else{
			tp->vel[X] = -FUNCrandFloat(3.0f) - 0.75f;
		}
	}else{
		if(tp->pos[X] < 320.0f){
			tp->vel[X] = +FUNCrandFloat(3.0f) + 0.75f;
		}else{
			tp->vel[X] = -FUNCrandFloat(3.0f) - 0.75f;
		}
	}
	tp->vel[Y] = 1.21375f;
	tp->acc[Y] = 0.0f;
	tp->mode = 0;
	tp->cnt = 0;
	tp->pat = 0;
	tp->dest_rate = ENEMY_DEFAULT_RATE;

	enemy_set_small_core(tp);
}

SDL_bool tsk_zako08_move(TSK *tp)
{
	tp->cnt++;

	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];
	tp->vel[Y] += tp->acc[Y];

	if(tp->pos[X] < ZAKO08_MINX) return SDL_FALSE;
	if(tp->pos[X] > ZAKO08_MAXX) return SDL_FALSE;
	if(tp->pos[Y] > ZAKO08_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

void tsk_zako08_shot(TSK *tp)
{
	if(tp->mode == 1){
		if((tp->cnt % 15) == 0){
			enemy_shot_1way(tp, 0, 0);
		}
	}
}
