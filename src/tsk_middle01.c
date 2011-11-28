#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define MIDD01_MINY ((float)SCR_TOP    - 32.0f)
#define MIDD01_MAXY ((float)SCR_BOTTOM + 32.0f)

#define SHOT_FRAME (30)
#define EXP_CNT (5)
#define EXP_WAIT (3)

static void tsk_middle01_exit(TSK *tp);
static void tsk_middle01_intr(TSK *tp);
static void tsk_middle01_draw(TSK *tp);
static void tsk_middle01_init(TSK *tp);
static SDL_bool tsk_middle01_move1(TSK *tp);
static SDL_bool tsk_middle01_moveD(TSK *tp);
static void tsk_middle01_shot(TSK *tp);
static void tsk_middle01_explode(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0, 128, 128 },
};

static COLLISION zako_collision[] = {
	{ {   +0.0f,   +0.0f }, {  +32.0f,  +32.0f } },
};

static int shot_level[] = { 10,  8,  6,  4,  3 };

void tsk_middle01(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_middle01_exit;
			tp->intr_adr = tsk_middle01_intr;
			tp->draw_adr = tsk_middle01_draw;
			TSKsetCollision(tp, zako_collision);
			tsk_middle01_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_middle01_move1(tp) == SDL_TRUE){
				tsk_middle01_shot(tp);
				break;
			}
			tp->step = STEP_EXIT;
			break;

		case STEP_MAIN+1000:
			tp->cnt = EXP_CNT * 2;
			tp->wait = 0;
			tp->step++;
		case STEP_MAIN+1001:
			tsk_middle01_moveD(tp);
			tp->tskid |= TSKID_DAMAGE;
			if(tp->wait == 0){
				tsk_middle01_explode(tp);
				tp->wait = EXP_WAIT;
				tp->cnt--;
			}else{
				tp->wait--;
			}
			if(tp->cnt > 0) break;
			enemy_explode(tp, EXPTYPE_SMALL_TYPE2, -24, -24);
			enemy_explode(tp, EXPTYPE_SMALL_TYPE2, +24, +24);
			enemy_explode(tp, EXPTYPE_SMALL_TYPE2,  +0,  +0);
			enemy_explode(tp, EXPTYPE_SMALL_TYPE2, +24, -24);
			enemy_explode(tp, EXPTYPE_SMALL_TYPE2, -24, +24);
			tp->step = STEP_EXIT;
			break;

		default:
			enemy_release_core_all(tp);
			TSKclear(tp);
			break;
	}
}

void tsk_middle01_exit(TSK *tp)
{
}

void tsk_middle01_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if(etp != NULL){
		if(etp->group_no == GROUP_SSHOT){
			enemy_damage(tp, etp, 5000, STEP_MAIN + 1000);
		}else if(etp->group_no == GROUP_NSHOT && (etp->tskid & TSKID_NUCLEAR) != 0){
			enemy_damage(tp, etp, 5000, STEP_MAIN + 1000);
		}
	}

	if(tp->draw_adr == NULL){
		tp->draw_adr = tsk_middle01_draw;
	}
}

void tsk_middle01_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];
	if((tp->tskid & TSKID_DAMAGE) == 0){
		SDL_wheel_DispSurface(surface[SURFACE_MIDL01], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
	}else{
		SDL_wheel_DispSurface(surface[SURFACE_MIDL01R], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
		tp->tskid &= ~TSKID_DAMAGE;
	}
}

void tsk_middle01_init(TSK *tp)
{
	tp->energy = 250;

	if(ship_tsk == NULL){
		tp->pos[X] = FUNCrandFloat((float)SCR_X - 128.0f) + 64.0f;
	}else{
		if(ship_tsk->pos[X] < (float)SCR_HX){
			tp->pos[X] = ship_tsk->pos[X] + (float)SCR_HX * 0.8f;
		}else{
			tp->pos[X] = ship_tsk->pos[X] - (float)SCR_HX * 0.8f;
		}
	}
	tp->pos[Y] = MIDD01_MINY;
	tp->trg[Y] = 160.0f;
	tp->cnt = 0;
	tp->num = 0;
	tp->pat = 0;
	tp->dest_rate = 2.5f;

	enemy_set_small_core_pos(tp, -16.0f, -12.0f);
	enemy_set_small_core_pos(tp, +16.0f, -12.0f);
	enemy_set_small_core_pos(tp, -16.0f, +12.0f);
	enemy_set_small_core_pos(tp, +16.0f, +12.0f);
}

SDL_bool tsk_middle01_move1(TSK *tp)
{
	enemy_move_vec_reset(tp);
	if(tp->cnt < 96){
		tp->wait = SHOT_FRAME * 2;
		tp->vel[Y]  = tp->pos[Y];
		tp->pos[Y] += (tp->trg[Y] - tp->pos[Y]) / 120.0f;
		tp->vel[Y]  = tp->pos[Y] - tp->vel[Y];
	}else{
		tp->pos[Y] += tp->vel[Y];
	}
	enemy_move_vec_set(tp);

	tp->cnt++;

	if(tp->pos[Y] > MIDD01_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

SDL_bool tsk_middle01_moveD(TSK *tp)
{
	tp->pos[Y] += tp->vec[Y];

	return SDL_TRUE;
}

void tsk_middle01_shot(TSK *tp)
{
	if(tp->wait <= SHOT_FRAME && (tp->wait % shot_level[game_level]) == 0){
		if((tp->num % 2) != 0){
			enemy_shot_1way(tp, -16, -12);
			enemy_shot_1way(tp, +16, -12);
			enemy_shot_1way(tp, -16, +12);
			enemy_shot_1way(tp, +16, +12);
		}else{
			enemy_shot_2way(tp, -16, -12);
			enemy_shot_2way(tp, +16, -12);
			enemy_shot_2way(tp, -16, +12);
			enemy_shot_2way(tp, +16, +12);
		}
		tp->num++;
	}

	if(tp->wait != 0){
		tp->wait--;
	}else{
		tp->wait = SHOT_FRAME * 2;
	}
}

void tsk_middle01_explode(TSK *tp)
{
	switch(tp->cnt % 5){
		case EXP_CNT-1: enemy_explode(tp, EXPTYPE_SMALL_TYPE1, -24, -24); break;
		case EXP_CNT-2: enemy_explode(tp, EXPTYPE_SMALL_TYPE1, +24, +24); break;
		case EXP_CNT-3: enemy_explode(tp, EXPTYPE_SMALL_TYPE1,  +0,  +0); break;
		case EXP_CNT-4: enemy_explode(tp, EXPTYPE_SMALL_TYPE1, +24, -24); break;
		case EXP_CNT-5: enemy_explode(tp, EXPTYPE_SMALL_TYPE1, -24, +24); break;
		default: break;
	}
}

