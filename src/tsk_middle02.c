#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define MIDL02_MINX  ((float)SCR_LEFT   - 48.0f)
#define MIDL02_MAXX  ((float)SCR_RIGHT  + 48.0f)
#define MIDL02_MINY  ((float)SCR_TOP    - 48.0f)
#define MIDL02_MAXY  ((float)SCR_BOTTOM + 48.0f)

#define PAT_WAIT (2)
#define EXP_CNT (5)
#define EXP_WAIT (3)

static void tsk_middle02_exit(TSK *tp);
static void tsk_middle02_intr(TSK *tp);
static void tsk_middle02_draw(TSK *tp);
static void tsk_middle02_init(TSK *tp);
static SDL_bool tsk_middle02_move1(TSK *tp);
static SDL_bool tsk_middle02_move2(TSK *tp);
static SDL_bool tsk_middle02_moveD(TSK *tp);
static void tsk_middle02_shot(TSK *tp);
static void tsk_middle02_explode(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0, 128, 128 },
	{ 128,   0, 256, 128 },
	{ 256,   0, 384, 128 },
	{ 384,   0, 512, 128 },
	{   0, 128, 128, 256 },
	{ 128, 128, 256, 256 },
	{ 256, 128, 384, 256 },
	{ 384, 128, 512, 256 },
};

static COLLISION zako_collision[] = {
	{ {   +0.0f,   +0.0f }, {  +32.0f,  +32.0f } },
};

static int shot_level[] = { 20, 15, 10,  5,  5 };

void tsk_middle02(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->tskid |= TSKID_NODMG;
			tp->exit_adr = tsk_middle02_exit;
			tp->intr_adr = tsk_middle02_intr;
			tp->draw_adr = tsk_middle02_draw;
			TSKsetCollision(tp, zako_collision);
			tsk_middle02_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_middle02_move1(tp) == SDL_TRUE){
				break;
			}
			tp->step++;
			break;
		case STEP_MAIN+1:
			tp->i++;
			if(tp->i < 60){
				tsk_middle02_shot(tp);
				break;
			}
			if(tsk_middle02_move2(tp) == SDL_TRUE) break;
			tp->step++;
			break;

		case STEP_MAIN+1000:
			tp->cnt = EXP_CNT * 2;
			tp->wait = 0;
			tp->step++;
		case STEP_MAIN+1001:
			tsk_middle02_moveD(tp);
			tp->tskid |= TSKID_DAMAGE;
			if(tp->wait == 0){
				tsk_middle02_explode(tp);
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
			enemy_release_core(tp);
			TSKclear(tp);
			break;
	}
}

void tsk_middle02_exit(TSK *tp)
{
}

void tsk_middle02_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if((tp->tskid & TSKID_NODMG) != 0){
		set_particle(tp, 2, SDL_FALSE, 0.0f, 0.0f, 0.0f, 0.0f);
		enemy_score(1);
		return;
	}

	if(etp != NULL){
		if(etp->group_no == GROUP_SSHOT){
			enemy_damage(tp, etp, 5000, STEP_MAIN + 1000);
		}else if(etp->group_no == GROUP_NSHOT && (etp->tskid & TSKID_NUCLEAR) != 0){
			enemy_damage(tp, etp, 5000, STEP_MAIN + 1000);
		}
	}

	if(tp->draw_adr == NULL){
		tp->draw_adr = tsk_middle02_draw;
	}
}

void tsk_middle02_draw(TSK *tp)
{
	SDL_Rect *p;

	if((tp->tskid & TSKID_DAMAGE) == 0){
		p = &pat_rect[tp->pat];
		SDL_wheel_DispSurface(surface[SURFACE_MIDL02], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
	}else{
		p = &pat_rect[0];
		SDL_wheel_DispSurface(surface[SURFACE_MIDL02R], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
		tp->tskid &= ~TSKID_DAMAGE;
	}
}

void tsk_middle02_init(TSK *tp)
{
	tp->energy = 250;

	if(ship_tsk == NULL){
		tp->pos[X] = FUNCrandFloat((float)SCR_X - 160.0f) + 80.0f;
	}else{
		if(ship_tsk->pos[X] < (float)SCR_HX){
			tp->pos[X] = ship_tsk->pos[X] + (float)SCR_HX * 0.75f;
		}else{
			tp->pos[X] = ship_tsk->pos[X] - (float)SCR_HX * 0.75f;
		}
	}
	tp->pos[Y] = FUNCrandFloat((float)SCR_HY * 0.75f - 80) + 80.0f;

	tp->vel[X] = 0.0f;
	tp->vel[Y] = 0.0f;

	tp->pat = 1;
	tp->pat_wait = PAT_WAIT * 5;
	tp->dest_rate = 2.5f;

	enemy_set_small_core_pos(tp, -16.0f, -17.0f);
	enemy_set_small_core_pos(tp, +16.0f, -17.0f);
	enemy_set_small_core_pos(tp,  +0.0f,  +8.0f);
	enemy_set_small_core_pos(tp, -14.0f, +15.0f);
	enemy_set_small_core_pos(tp, +14.0f, +15.0f);

	SDL_wheel_PlaySe(SE_EWARP_IN);
}

SDL_bool tsk_middle02_move1(TSK *tp)
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
		return SDL_FALSE;
	}

	return SDL_TRUE;
}

SDL_bool tsk_middle02_move2(TSK *tp)
{
	enemy_move_vec_reset(tp);
	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];
	enemy_move_vec_set(tp);

	if(tp->pos[X] < MIDL02_MINX) return SDL_FALSE;
	if(tp->pos[X] > MIDL02_MAXX) return SDL_FALSE;
	if(tp->pos[Y] < MIDL02_MINY) return SDL_FALSE;
	if(tp->pos[Y] > MIDL02_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

SDL_bool tsk_middle02_moveD(TSK *tp)
{
	tp->pos[Y] += tp->vec[Y];

	return SDL_TRUE;
}

void tsk_middle02_shot(TSK *tp)
{
#if 0
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
#endif
}

void tsk_middle02_explode(TSK *tp)
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
