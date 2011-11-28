#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define ZAKO04_MINX  ((float)SCR_LEFT   -  24.0f)
#define ZAKO04_MAXX  ((float)SCR_RIGHT  +  24.0f)
#define ZAKO04_MINY  ((float)SCR_TOP    -  24.0f)
#define ZAKO04_MINY2 ((float)SCR_TOP    - 128.0f)
#define ZAKO04_MAXY  ((float)SCR_BOTTOM +  24.0f)

#define POS_SIZE (8)
#define SHOT_FRAME (60)

#define SPD_ACC (0.173215f)

static void tsk_zako04_exit(TSK *tp);
static void tsk_zako04_intr(TSK *tp);
static void tsk_zako04_draw(TSK *tp);
static void tsk_zako04_init(TSK *tp);
static void tsk_zako04_startL(TSK *tp);
static void tsk_zako04_startR(TSK *tp);
static SDL_bool tsk_zako04_move(TSK *tp);

static void tsk_zako04_opt(TSK *tp);
static void tsk_zako04_opt_exit(TSK *tp);
static void tsk_zako04_opt_intr(TSK *tp);
static void tsk_zako04_opt_draw(TSK *tp);
static void tsk_zako04_opt_init(TSK *tp);
static void tsk_zako04_opt_startL(TSK *tp);
static void tsk_zako04_opt_startR(TSK *tp);
static SDL_bool tsk_zako04_opt_move1(TSK *tp);
static SDL_bool tsk_zako04_opt_move2(TSK *tp);
static SDL_bool tsk_zako04_opt_move3(TSK *tp);
static void tsk_zako04_opt_shot(TSK *tp);
static SDL_bool tsk_zako04_opt_isParent(TSK *tp, SDL_bool f);

static SDL_Rect pat_rectR[] = {
	{   0,   0,  64,  64 },
	{  64,   0, 128,  64 },
	{ 128,   0, 192,  64 },
	{ 192,   0, 256,  64 },
	{ 256,   0, 320,  64 },
	{ 320,   0, 384,  64 },
	{ 384,   0, 448,  64 },
	{ 448,   0, 521,  64 },
};

static SDL_Rect pat_rectL[] = {
	{   0,  64,  64, 128 },
	{  64,  64, 128, 128 },
	{ 128,  64, 192, 128 },
	{ 192,  64, 256, 128 },
	{ 256,  64, 320, 128 },
	{ 320,  64, 384, 128 },
	{ 384,  64, 448, 128 },
	{ 448,  64, 521, 128 },
};

static COLLISION zako_collision1[] = {
	{ {   +0.0f,   +0.0f }, {  +48.0f,  +24.0f } },
};

static COLLISION zako_collision2[] = {
	{ {   +0.0f,   +0.0f }, {  +24.0f,  +24.0f } },
};

static int shot_level[] = { 10,  8,  6,  4,  3 };

static Sint16 zako_posTableL[POS_SIZE];
static Sint16 zako_posTableR[POS_SIZE];
static int zako_posL[POS_SIZE];
static int zako_posR[POS_SIZE];
static int zako_indexL;
static int zako_indexR;

void tsk_zako04_reset(void)
{
	int i;

	for(i = 0; i < POS_SIZE; i++){
		zako_posTableL[i] = 0 + 80 + ((SCR_HX - 80 * 2) / POS_SIZE) * i;
		zako_posTableR[i] = SCR_HX + 80 + ((SCR_HX - 80 * 2) / POS_SIZE) * i;
	}

	FUNCinitTbl(zako_posL, POS_SIZE, 0);
	FUNCinitTbl(zako_posR, POS_SIZE, 0);
	zako_indexL = POS_SIZE;
	zako_indexR = POS_SIZE;
}

void tsk_zako04(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_zako04_exit;
			tp->intr_adr = tsk_zako04_intr;
			tp->draw_adr = tsk_zako04_draw;
			TSKsetCollision(tp, zako_collision1);
			tsk_zako04_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tp->tskid &= ~TSKID_DAMAGE;
			if(tsk_zako04_move(tp) == SDL_TRUE){
				break;
			}
			tp->step = STEP_EXIT;
			break;

		case STEP_MAIN+100:
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_zako04_exit(TSK *tp)
{
}

void tsk_zako04_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if(etp != NULL){
		if(etp->group_no == GROUP_SSHOT){
			enemy_damage(tp, etp, 100, STEP_MAIN + 100);
		}else if(etp->group_no == GROUP_NSHOT && (etp->tskid & TSKID_NUCLEAR) != 0){
			enemy_damage(tp, etp, 100, STEP_MAIN + 100);
		}
	}
}

void tsk_zako04_draw(TSK *tp)
{
}

void tsk_zako04_init(TSK *tp)
{
	TSK *etp;

	tp->energy = 25;

	if(player_side < 0){
		tsk_zako04_startR(tp);
	}else if(player_side > 0){
		tsk_zako04_startL(tp);
	}else{
		if((rand() % 100) < 50){
			tsk_zako04_startL(tp);
		}else{
			tsk_zako04_startR(tp);
		}
	}

	tp->pos[Y] = ZAKO04_MINY;
	tp->vel[Y] = 1.37125f;

	etp = TSKsetBtmName(tsk_zako04_opt, GROUP_ENEMY, NAME_ZAKO);
	if(etp != NULL){
		etp->tsk_parent = tp;
		etp->mode = 0;
		TSKexecDirect(etp);
	}

	etp = TSKsetBtmName(tsk_zako04_opt, GROUP_ENEMY, NAME_ZAKO);
	if(etp != NULL){
		etp->tsk_parent = tp;
		etp->mode = 1;
		TSKexecDirect(etp);
	}
}

void tsk_zako04_startL(TSK *tp)
{
	if(zako_indexL == POS_SIZE){
		FUNCshuffleTbl(zako_posL, POS_SIZE);
		zako_indexL = 0;
	}
	tp->pos[X] = zako_posTableL[zako_posL[zako_indexL]];
	zako_indexL++;
}

void tsk_zako04_startR(TSK *tp)
{
	if(zako_indexR == POS_SIZE){
		FUNCshuffleTbl(zako_posR, POS_SIZE);
		zako_indexR = 0;
	}
	tp->pos[X] = zako_posTableR[zako_posR[zako_indexR]];
	zako_indexR++;
}

SDL_bool tsk_zako04_move(TSK *tp)
{
	tp->pos[Y] += tp->vel[Y];

	if(tp->pos[Y] > ZAKO04_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

void tsk_zako04_opt(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_zako04_opt_exit;
			tp->intr_adr = NULL;
			tp->draw_adr = tsk_zako04_opt_draw;
			TSKsetCollision(tp, zako_collision2);
			tsk_zako04_opt_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_zako04_opt_isParent(tp, SDL_FALSE) == SDL_TRUE){
				tp->draw_adr = NULL;
				tp->step = STEP_EXIT;
				break;
			}
			if(tsk_zako04_opt_isParent(tp, SDL_TRUE) == SDL_TRUE){
				if(tp->mode == 0){
					tsk_zako04_opt_startR(tp);
				}else{
					tsk_zako04_opt_startL(tp);
				}
				tp->wait = 0;
				tp->step++;
				break;
			}
			tsk_zako04_opt_move1(tp);
			break;
		case STEP_MAIN+1:
			if(tsk_zako04_opt_move2(tp) == SDL_TRUE) break;
			tp->wait = SHOT_FRAME;
			tp->intr_adr = tsk_zako04_opt_intr;
			tp->step++;
			break;
		case STEP_MAIN+2:
			if(tsk_zako04_opt_move3(tp) == SDL_TRUE){
				tsk_zako04_opt_shot(tp);
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

void tsk_zako04_opt_exit(TSK *tp)
{
}

void tsk_zako04_opt_intr(TSK *tp)
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

void tsk_zako04_opt_draw(TSK *tp)
{
	TSK *etp = tp->tsk_parent;
	SDL_Rect *p;

	if(tp->mode == 0){
		p = &pat_rectR[tp->pat];
	}else{
		p = &pat_rectL[tp->pat];
	}

	if(etp != NULL){
		if((etp->tskid & TSKID_DAMAGE) == 0){
			SDL_wheel_DispSurface(surface[SURFACE_ZAKO04], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
		}else{
			SDL_wheel_DispSurface(surface[SURFACE_ZAKO04R], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
		}
	}else{
		if((tp->tskid & TSKID_DAMAGE) == 0){
			SDL_wheel_DispSurface(surface[SURFACE_ZAKO04], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
		}else{
			SDL_wheel_DispSurface(surface[SURFACE_ZAKO04R], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
			tp->tskid &= ~TSKID_DAMAGE;
		}
	}
}

void tsk_zako04_opt_init(TSK *tp)
{
	tp->energy = 15;
	tp->dest_rate = ENEMY_DEFAULT_RATE;

	tsk_zako04_opt_move1(tp);

	enemy_set_small_core(tp);
}

void tsk_zako04_opt_startL(TSK *tp)
{
	tp->trg[X] = tp->pos[X] - 56.0f;
	tp->trg[Y] = tp->pos[Y] - 128.0f;
}

void tsk_zako04_opt_startR(TSK *tp)
{
	tp->trg[X] = tp->pos[X] + 56.0f;
	tp->trg[Y] = tp->pos[Y] - 128.0f;
}

SDL_bool tsk_zako04_opt_move1(TSK *tp)
{
	TSK *etp = tp->tsk_parent;

	enemy_move_vec_reset(tp);
	if(etp != NULL){
		if(tp->mode == 0){
			tp->pos[X] = etp->pos[X] + 14.0f;
		}else{
			tp->pos[X] = etp->pos[X] - 14.0f;
		}
		tp->pos[Y] = etp->pos[Y];
	}
	enemy_move_vec_set(tp);

	return SDL_TRUE;
}

SDL_bool tsk_zako04_opt_move2(TSK *tp)
{
	float dir;

	tp->wait++;
	tp->pat++;
	tp->pat %= 8;

	enemy_move_vec_reset(tp);
	tp->pos[X] += (tp->trg[X] - tp->pos[X]) / 10.0f;
	tp->pos[Y] += (tp->trg[Y] - tp->pos[Y]) / 20.0f;
	enemy_move_vec_set(tp);
	enemy_calc_destrate(tp, ENEMY_DEFAULT_RATE / 60.0f);

	if(tp->wait == 30){
		dir = enemy_getaim(tp, 0.0f);
		tp->vel[X] = 0.0f;
		tp->vel[Y] = 0.0f;
		tp->acc[X] = cos(dir) * SPD_ACC;
		tp->acc[Y] = sin(dir) * SPD_ACC;
		tp->pat = 2;
		return SDL_FALSE;
	}

	return SDL_TRUE;
}

SDL_bool tsk_zako04_opt_move3(TSK *tp)
{
	enemy_move_vec_reset(tp);
	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];
	tp->vel[X] += tp->acc[X];
	tp->vel[Y] += tp->acc[Y];
	enemy_move_vec_set(tp);
	enemy_calc_destrate(tp, ENEMY_DEFAULT_RATE / 60.0f);

	if(tp->pos[X] < ZAKO04_MINX)  return SDL_FALSE;
	if(tp->pos[X] > ZAKO04_MAXX)  return SDL_FALSE;
	if(tp->pos[Y] < ZAKO04_MINY2) return SDL_FALSE;
	if(tp->pos[Y] > ZAKO04_MAXY)  return SDL_FALSE;

	return SDL_TRUE;
}

void tsk_zako04_opt_shot(TSK *tp)
{
	if(tp->pos[Y] > (float)SCR_HY) return;

	if((tp->wait % shot_level[game_level]) == 0){
		enemy_shot_1way(tp, 0, 0);
	}

	if(tp->wait != 0){
		tp->wait--;
	}else{
		tp->wait = SHOT_FRAME * 2;
	}
}

SDL_bool tsk_zako04_opt_isParent(TSK *tp, SDL_bool f)
{
	TSK *etp = tp->tsk_parent;
	SDL_bool ret = SDL_FALSE;

	if(etp != NULL){
		if(f == SDL_FALSE){
			if(etp->step == STEP_EXIT && etp->energy > 0){
				tp->tsk_parent = NULL;
				ret = SDL_TRUE;
			}
		}else{
			if(etp->step == STEP_EXIT || etp->step == (STEP_MAIN + 100)){
				tp->tsk_parent = NULL;
				ret = SDL_TRUE;
			}
		}
	}else{
		ret = SDL_TRUE;
	}

	return ret;
}
