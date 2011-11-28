#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define ZAKO03_MINX  ((float)SCR_LEFT   - 24.0f)
#define ZAKO03_MAXX  ((float)SCR_RIGHT  + 24.0f)
#define ZAKO03_MINY  ((float)SCR_TOP    - 24.0f)
#define ZAKO03_MAXY  ((float)SCR_BOTTOM + 24.0f)

#define POS_SIZE (16)
#define SHOT_FRAME (3)

#define SPD_ACC (0.173215f)

static void tsk_zako03_exit(TSK *tp);
static void tsk_zako03_intr(TSK *tp);
static void tsk_zako03_draw(TSK *tp);
static void tsk_zako03_init(TSK *tp);
static SDL_bool tsk_zako03_move1(TSK *tp);
static SDL_bool tsk_zako03_move2(TSK *tp);
static SDL_bool tsk_zako03_move3(TSK *tp);
static void tsk_zako03_shot(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0,  64,  64 },
	{  64,   0, 128,  64 },
	{ 128,   0, 192,  64 },
	{ 192,   0, 256,  64 },
};

static COLLISION zako_collision[] = {
	{ {   +0.0f,   +0.0f }, {  +16.0f,  +16.0f } },
};

static int shot_level[] = { 20, 15, 10,  5,  5 };

static Sint16 zako_posTableS[POS_SIZE];
static Sint16 zako_posTableE[POS_SIZE];
static int zako_posS[POS_SIZE];
static int zako_posE[POS_SIZE];
static int zako_indexS;
static int zako_indexE;

void tsk_zako03_reset(void)
{
	int i;

	for(i = 0; i < POS_SIZE; i++){
		zako_posTableS[i] = 0 + 32 + ((SCR_X - 32) / POS_SIZE) * i;
		zako_posTableE[i] = 0 + 32 + ((SCR_X - 32) / POS_SIZE) * i;
	}

	FUNCinitTbl(zako_posS, POS_SIZE, 0);
	FUNCinitTbl(zako_posE, POS_SIZE, 0);
	zako_indexS = POS_SIZE;
	zako_indexE = POS_SIZE;
}

void tsk_zako03(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_zako03_exit;
			tp->intr_adr = tsk_zako03_intr;
			tp->draw_adr = tsk_zako03_draw;
			TSKsetCollision(tp, zako_collision);
			tsk_zako03_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_zako03_move1(tp) == SDL_TRUE){
				break;
			}
			tp->step++;
			break;
		case STEP_MAIN+1:
			if(tsk_zako03_move2(tp) == SDL_TRUE){
				tsk_zako03_shot(tp);
				break;
			}
			tp->step++;
			break;
		case STEP_MAIN+2:
			if(tsk_zako03_move3(tp) == SDL_TRUE){
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

void tsk_zako03_exit(TSK *tp)
{
}

void tsk_zako03_intr(TSK *tp)
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

void tsk_zako03_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];
	if((tp->tskid & TSKID_DAMAGE) == 0){
		SDL_wheel_DispSurface(surface[SURFACE_ZAKO03], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
	}else{
		SDL_wheel_DispSurface(surface[SURFACE_ZAKO03R], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
		tp->tskid &= ~TSKID_DAMAGE;
	}
}

void tsk_zako03_init(TSK *tp)
{
	tp->energy = 13;

	if(zako_indexS == POS_SIZE){
		FUNCshuffleTbl(zako_posS, POS_SIZE);
		zako_indexS = 0;
	}
	tp->pos[X] = zako_posTableS[zako_posS[zako_indexS]];
	zako_indexS++;

	if(zako_indexE == POS_SIZE){
		FUNCshuffleTbl(zako_posE, POS_SIZE);
		zako_indexE = 0;
	}
	tp->trg[X] = zako_posTableE[zako_posE[zako_indexE]];
	zako_indexE++;

	tp->pos[Y] = -24.0f;
	tp->trg[Y] = (float)SCR_HY - 64.0f;
	tp->dest_rate = ENEMY_DEFAULT_RATE;

	enemy_set_small_core(tp);
}

SDL_bool tsk_zako03_move1(TSK *tp)
{
	enemy_move_vec_reset(tp);
	tp->vel[Y]  = tp->pos[Y];
	tp->pos[X] += (tp->trg[X] - tp->pos[X]) / 15.0f;
	tp->pos[Y] += (tp->trg[Y] - tp->pos[Y]) / 20.0f;
	tp->vel[Y]  = tp->pos[Y] - tp->vel[Y];
	enemy_move_vec_set(tp);
	enemy_calc_destrate(tp, ENEMY_DEFAULT_RATE / 80.0f);

	tp->cnt++;
	if(tp->cnt > 40){
		tp->cnt = 0;
		tp->wait = 0;
		return SDL_FALSE;
	}

	return SDL_TRUE;
}

SDL_bool tsk_zako03_move2(TSK *tp)
{
	enemy_move_vec_reset(tp);
	tp->pos[Y] += tp->vel[Y];
	enemy_move_vec_set(tp);
	enemy_calc_destrate(tp, ENEMY_DEFAULT_RATE / 80.0f);

	tp->cnt++;
	if(tp->cnt > 65){
		tp->pat = 0;
		tp->acc[Y] = SPD_ACC;
		tp->cnt = 0;
		return SDL_FALSE;
	}

	tp->pat++;
	tp->pat %= 4;

	return SDL_TRUE;
}

SDL_bool tsk_zako03_move3(TSK *tp)
{
	enemy_move_vec_reset(tp);
	tp->pos[Y] += tp->vel[Y];
	tp->vel[Y] += tp->acc[Y];
	enemy_move_vec_set(tp);

	if(tp->pos[Y] > ZAKO03_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

void tsk_zako03_shot(TSK *tp)
{
	if(tp->wait != 0) tp->wait--;

	if(tp->wait == 0){
		tp->wait = SHOT_FRAME;
		enemy_shot_1way(tp, 0, 0);
		tp->num++;
		if(tp->num == 4){
			tp->num = 0;
			tp->wait = shot_level[game_level];
		}
	}
}
