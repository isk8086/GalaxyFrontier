#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define ZAKO01_MINY ((float)SCR_TOP    - 24.0f)
#define ZAKO01_MAXY ((float)SCR_BOTTOM + 24.0f)

#define POS_SIZE (8)
#define WAIT_FRAME (90)
#define SHOT_FRAME (60)
#define MOVE_FRAME (50)

static void tsk_zako01_exit(TSK *tp);
static void tsk_zako01_intr(TSK *tp);
static void tsk_zako01_draw(TSK *tp);
static void tsk_zako01_init(TSK *tp);
static void tsk_zako01_startL(TSK *tp);
static void tsk_zako01_startR(TSK *tp);
static SDL_bool tsk_zako01_move(TSK *tp);
static void tsk_zako01_shot(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0,  64,  64 },
};

static COLLISION zako_collision[] = {
	{ { +0.0f, +0.0f }, { +16.0f, +16.0f } },
};

static int shot_level[] = { 20, 15, 10,  5,  5 };

static Sint16 zako_posTableL[POS_SIZE];
static Sint16 zako_posTableR[POS_SIZE];
static int zako_posL[POS_SIZE];
static int zako_posR[POS_SIZE];
static int zako_indexL;
static int zako_indexR;

void tsk_zako01_reset(void)
{
	int i;

	for(i = 0; i < POS_SIZE; i++){
		zako_posTableL[i] = 0 + 56 + ((SCR_HX - 56 * 2) / POS_SIZE) * i;
		zako_posTableR[i] = SCR_HX + 56 + ((SCR_HX - 56 * 2) / POS_SIZE) * i;
	}

	FUNCinitTbl(zako_posL, POS_SIZE, 0);
	FUNCinitTbl(zako_posR, POS_SIZE, 0);
	zako_indexL = POS_SIZE;
	zako_indexR = POS_SIZE;
}

void tsk_zako01(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_zako01_exit;
			tp->intr_adr = tsk_zako01_intr;
			tp->draw_adr = tsk_zako01_draw;
			TSKsetCollision(tp, zako_collision);
			tsk_zako01_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_zako01_move(tp) == SDL_TRUE){
				tsk_zako01_shot(tp);
				break;
			}
			tp->step = STEP_EXIT;
			break;

		case STEP_MAIN+1000:
			tp->cnt = 1;
			tp->step++;
		case STEP_MAIN+1001:
			enemy_zako_explode(tp);
			tp->cnt--;
			if(tp->cnt > 0) break;
			tp->step = STEP_EXIT;
			break;

		default:
			enemy_release_core(tp);
			TSKclear(tp);
			break;
	}
}

void tsk_zako01_exit(TSK *tp)
{
}

void tsk_zako01_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if(etp != NULL){
		if(etp->group_no == GROUP_SSHOT){
			enemy_damage(tp, etp, 100, STEP_MAIN + 1000);
		}else if(etp->group_no == GROUP_NSHOT && (etp->tskid & TSKID_NUCLEAR) != 0){
			enemy_damage(tp, etp, 100, STEP_MAIN + 1000);
		}
	}
}

void tsk_zako01_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];
	if((tp->tskid & TSKID_DAMAGE) == 0){
		SDL_wheel_DispSurface(surface[SURFACE_ZAKO01], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
	}else{
		SDL_wheel_DispSurface(surface[SURFACE_ZAKO01R], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
		tp->tskid &= ~TSKID_DAMAGE;
	}
}

void tsk_zako01_init(TSK *tp)
{
	tp->energy = 7;

	if(player_side < 0){
		tsk_zako01_startR(tp);
	}else if(player_side > 0){
		tsk_zako01_startL(tp);
	}else{
		if((rand() % 100) < 50){
			tsk_zako01_startL(tp);
		}else{
			tsk_zako01_startR(tp);
		}
	}

	tp->pos[Y] = ZAKO01_MINY;
	tp->vel[Y] = 8.1732165f;
	tp->acc[Y] = 0.0f;

	tp->wait = WAIT_FRAME;
	tp->cnt = 0;
	tp->pat = 0;
	tp->mode = 0;
	tp->dest_rate = ENEMY_DEFAULT_RATE;

	enemy_set_small_core(tp);
}

void tsk_zako01_startL(TSK *tp)
{
	if(zako_indexL == POS_SIZE){
		FUNCshuffleTbl(zako_posL, POS_SIZE);
		zako_indexL = 0;
	}
	tp->pos[X] = zako_posTableL[zako_posL[zako_indexL]];
	tp->trg[X] = tp->pos[X] + (FUNCrandFloat(64.0f) + 64.0f);
	zako_indexL++;
}

void tsk_zako01_startR(TSK *tp)
{
	if(zako_indexR == POS_SIZE){
		FUNCshuffleTbl(zako_posR, POS_SIZE);
		zako_indexR = 0;
	}
	tp->pos[X] = zako_posTableR[zako_posR[zako_indexR]];
	tp->trg[X] = tp->pos[X] - (FUNCrandFloat(64.0f) + 64.0f);
	zako_indexR++;
}

SDL_bool tsk_zako01_move(TSK *tp)
{
	enemy_move_vec_reset(tp);
	tp->pos[X] += (tp->trg[X] - tp->pos[X]) / 120.0f;
	tp->pos[Y] += tp->vel[Y];
	tp->vel[Y] += tp->acc[Y];
	enemy_move_vec_set(tp);
	enemy_calc_destrate(tp, ENEMY_DEFAULT_RATE / 120.0f);

	if(tp->mode == 0){
		if(tp->wait > WAIT_FRAME){
			tp->mode = 1;
		}
		if(tp->wait >= MOVE_FRAME){
			tp->vel[Y] += (0.0f - tp->vel[Y]) / 25.0f;
		}
	}else{
		tp->acc[Y] -= 0.021735f;
	}

	tp->cnt++;

	if(tp->pos[Y] < ZAKO01_MINY) return SDL_FALSE;
	if(tp->pos[Y] > ZAKO01_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

void tsk_zako01_shot(TSK *tp)
{
	if(tp->wait != 0){
		tp->wait--;
	}else{
		tp->wait = WAIT_FRAME * 2;
	}

	if(tp->wait <= SHOT_FRAME){
		if((tp->cnt % shot_level[game_level]) == 0){
			enemy_shot_1way(tp, 0, 0);
		}
	}
}
