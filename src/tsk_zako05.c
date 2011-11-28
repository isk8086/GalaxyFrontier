#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define ZAKO05_MINX  ((float)SCR_LEFT   - 24.0f)
#define ZAKO05_MAXX  ((float)SCR_RIGHT  + 24.0f)
#define ZAKO05_MINY  ((float)SCR_TOP    - 24.0f)
#define ZAKO05_MAXY  ((float)SCR_BOTTOM + 24.0f)

#define POS_SIZE (16)
#define MOV_VEL (7.61325f)
#define SHOT_FRAME (4)
#define PAT_WAIT (2)

#define SPD_ACC (0.173215f)

static void tsk_zako05_exit(TSK *tp);
static void tsk_zako05_intr(TSK *tp);
static void tsk_zako05_draw(TSK *tp);
static void tsk_zako05_init(TSK *tp);
static SDL_bool tsk_zako05_move1(TSK *tp);
static SDL_bool tsk_zako05_move2(TSK *tp);
static void tsk_zako05_shot(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0,  64,  64 },
	{  64,   0, 128,  64 },
	{ 128,   0, 192,  64 },
	{ 192,   0, 256,  64 },
	{   0,  64,  64, 128 },
	{  64,  64, 128, 128 },
	{ 128,  64, 192, 128 },
	{ 192,  64, 256, 128 },
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

void tsk_zako05_reset(void)
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

void tsk_zako05(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->tskid |= TSKID_NODMG;
			tp->exit_adr = tsk_zako05_exit;
			tp->intr_adr = tsk_zako05_intr;
			tp->draw_adr = tsk_zako05_draw;
			TSKsetCollision(tp, zako_collision);
			tsk_zako05_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_zako05_move1(tp) == SDL_TRUE){
				break;
			}
			tp->step++;
			break;
		case STEP_MAIN+1:
			tp->i++;
			if(tp->i < 60){
				tsk_zako05_shot(tp);
				break;
			}
			if(tsk_zako05_move2(tp) == SDL_TRUE) break;
			tp->step++;
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

void tsk_zako05_exit(TSK *tp)
{
}

void tsk_zako05_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if((tp->tskid & TSKID_NODMG) != 0){
		set_particle(tp, 2, SDL_FALSE, 0.0f, 0.0f, 0.0f, 0.0f);
		enemy_score(1);
		return;
	}

	if(etp != NULL){
		if(etp->group_no == GROUP_SSHOT){
			enemy_damage(tp, etp, 300, STEP_MAIN + 1000);
		}else if(etp->group_no == GROUP_NSHOT && (etp->tskid & TSKID_NUCLEAR) != 0){
			enemy_damage(tp, etp, 300, STEP_MAIN + 1000);
		}
	}
}

void tsk_zako05_draw(TSK *tp)
{
	SDL_Rect *p;

	if((tp->tskid & TSKID_DAMAGE) == 0){
		p = &pat_rect[tp->pat];
		SDL_wheel_DispSurface(surface[SURFACE_ZAKO05], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
	}else{
		p = &pat_rect[0];
		SDL_wheel_DispSurface(surface[SURFACE_ZAKO05R], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
		tp->tskid &= ~TSKID_DAMAGE;
	}
}

void tsk_zako05_init(TSK *tp)
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

	tp->pos[Y] = FUNCrandFloat((float)SCR_HY * 0.8f - 64) + 80.0f;

	tp->pat = 1;
	tp->pat_wait = PAT_WAIT * 5;
	tp->dest_rate = ENEMY_DEFAULT_RATE;

	enemy_set_small_core(tp);

	SDL_wheel_PlaySe(SE_EWARP_IN);
}

SDL_bool tsk_zako05_move1(TSK *tp)
{
	enemy_move_vec_reset(tp);
	enemy_move_vec_set(tp);

	if(tp->pat_wait == 0){
		tp->pat_wait = PAT_WAIT;
		tp->pat++;
		tp->pat %= 8;
	}else{
		tp->pat_wait--;
	}

	if(tp->pat == 0){
		tp->tskid &= ~TSKID_NODMG;
		tp->i = 0;
		tp->cnt = 0;
		tp->wait = 0;
		if(ship_tsk == NULL){
			tp->ang[X] = FUNCrandFloat(PI / 3.0f) - (PI / 6.0f);
			tp->vel[X] = sin(tp->ang[X]) * MOV_VEL;
			tp->vel[Y] = cos(tp->ang[X]) * MOV_VEL;
		}else{
			tp->ang[X] = enemy_getaim(tp, FUNCrandFloat(PI / 6.0f) - (PI / 12.0f));
			tp->vel[X] = cos(tp->ang[X]) * MOV_VEL;
			tp->vel[Y] = sin(tp->ang[X]) * MOV_VEL;
		}
		return SDL_FALSE;
	}

	return SDL_TRUE;
}

SDL_bool tsk_zako05_move2(TSK *tp)
{
	enemy_move_vec_reset(tp);
	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];
	enemy_move_vec_set(tp);
	enemy_calc_destrate(tp, ENEMY_DEFAULT_RATE / 60.0f);

	if(tp->pos[X] < ZAKO05_MINX) return SDL_FALSE;
	if(tp->pos[X] > ZAKO05_MAXX) return SDL_FALSE;
	if(tp->pos[Y] < ZAKO05_MINY) return SDL_FALSE;
	if(tp->pos[Y] > ZAKO05_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

void tsk_zako05_shot(TSK *tp)
{
	if(tp->wait != 0) tp->wait--;

	if(tp->wait == 0){
		tp->wait = SHOT_FRAME;
		enemy_shot_2way(tp, 0, 0);
		tp->num++;
		if(tp->num == 4){
			tp->num = 0;
			tp->wait = shot_level[game_level];
		}
	}
}
