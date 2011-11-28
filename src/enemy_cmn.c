#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

static int core_small_pat[] = {
	SURFACE_ZAKOSTONE1,
	SURFACE_ZAKOSTONE2,
	SURFACE_ZAKOSTONE3,
	SURFACE_ZAKOSTONE4,
};

static int core_middle_pat[] = {
	SURFACE_MIDDSTONE1,
	SURFACE_MIDDSTONE2,
	SURFACE_MIDDSTONE3,
	SURFACE_MIDDSTONE4,
};

static void enemy_shot_fire(TSK *tp, int type, float dir, int x, int y);

static void tsk_core_small(TSK *tp);
static void tsk_core_small_exit(TSK *tp);
static void tsk_core_small_draw(TSK *tp);
static void tsk_core_small_init(TSK *tp);
static void tsk_core_small_anim(TSK *tp);
static void tsk_core_small_move(TSK *tp);

static void tsk_core_middle(TSK *tp);
static void tsk_core_middle_exit(TSK *tp);
static void tsk_core_middle_draw(TSK *tp);
static void tsk_core_middle_init(TSK *tp);
static void tsk_core_middle_anim(TSK *tp);
static void tsk_core_middle_move(TSK *tp);

void enemy_reset(void)
{
	tsk_zako01_reset();
	tsk_zako02_reset();
	tsk_zako03_reset();
	tsk_zako04_reset();
	tsk_zako05_reset();
}

void enemy_set_small_core(TSK *tp)
{
	enemy_set_small_core_pos(tp, 0, 0);
}

void enemy_set_small_core_pos(TSK *tp, int x, int y)
{
	TSK *etp;

	tp->tsk_child = NULL;

	etp = TSKsetBtmName(tsk_core_small, GROUP_ENEMY, NAME_CORE);
	if(etp != NULL){
		tp->tsk_child = etp;
		etp->tsk_parent = tp;
		etp->ofs[X] = x;
		etp->ofs[Y] = y;
		TSKexecDirect(etp);
	}
}

void enemy_set_middle_core(TSK *tp)
{
	enemy_set_middle_core_pos(tp, 0, 0);
}

void enemy_set_middle_core_pos(TSK *tp, int x, int y)
{
	TSK *etp;

	tp->tsk_child = NULL;

	etp = TSKsetBtmName(tsk_core_middle, GROUP_ENEMY, NAME_CORE);
	if(etp != NULL){
		tp->tsk_child = etp;
		etp->tsk_parent = tp;
		etp->ofs[X] = x;
		etp->ofs[Y] = y;
		TSKexecDirect(etp);
	}
}

void enemy_release_core(TSK *tp)
{
	TSK *etp = tp->tsk_child;

	if(etp != NULL){
		etp->step = STEP_EXIT;
	}
}

void enemy_release_core_all(TSK *tp)
{
	TSK *etp;

	for(etp = TskIndex[GROUP_ENEMY][TSK_TOP].next_tsk; etp != &(TskIndex[GROUP_ENEMY][TSK_BOTTOM]); etp = etp->next_tsk){
		if(etp->name == NAME_CORE && etp->tsk_parent == tp){
			etp->step = STEP_EXIT;
		}
	}
}

void enemy_move_vec_reset(TSK *tp)
{
	tp->vec[X] = tp->pos[X];
	tp->vec[Y] = tp->pos[Y];
}

void enemy_move_vec_set(TSK *tp)
{
	tp->vec[X] = tp->pos[X] - tp->vec[X];
	tp->vec[Y] = tp->pos[Y] - tp->vec[Y];
}

void enemy_calc_destrate(TSK *tp, float rate)
{
	tp->dest_rate -= rate;
	if(tp->dest_rate < 1.0f) tp->dest_rate = 1.0f;
}

void enemy_shot_1way(TSK *tp, int x, int y)
{
	if(ship_tsk != NULL){
		enemy_shot_fire(tp, BULLET_S, 0.0f, x, y);
	}
}

void enemy_shot_2way(TSK *tp, int x, int y)
{
	if(ship_tsk != NULL){
		enemy_shot_fire(tp, BULLET_M, -(PI / 7.5f), x, y);
		enemy_shot_fire(tp, BULLET_M, +(PI / 7.5f), x, y);
	}
}

void enemy_shot_3way(TSK *tp, int x, int y)
{
	if(ship_tsk != NULL){
		enemy_shot_fire(tp, BULLET_L, -(PI / 5.0f), x, y);
		enemy_shot_fire(tp, BULLET_L, 0.0f, x, y);
		enemy_shot_fire(tp, BULLET_L, +(PI / 5.0f), x, y);
	}
}

void enemy_damage(TSK *tp, TSK *etp, int score, int dest_step)
{
	TSK *eetp;
	SDL_bool f;
	int bonus = 0;

	f = (etp->tskid & TSKID_NUCLEAR) != 0 ? SDL_TRUE : SDL_FALSE;

	enemy_score(1);

	set_particle(tp, 2, SDL_FALSE, 0.0f, 0.0f, 0.0f, 0.0f);

	if(tp->energy > 0){
		tp->energy -= etp->energy;
	}

	if(tp->energy <= 0){
		if(f == SDL_FALSE && nuclear_exec == SDL_FALSE){
			nuclear_power += (float)score / ENEMY_BASE_RATE * tp->dest_rate;
			if(nuclear_power >= NUCLEAR_MAX) nuclear_power = NUCLEAR_MAX;
		}
		if(f == SDL_TRUE){
			total_bonus += bonus_score;
			bonus = total_bonus;
		}
		enemy_score(score + bonus);
		tp->intr_adr = NULL;
		tp->draw_adr = NULL;
		tp->step = dest_step;
		if(f == SDL_TRUE && (etp->tskid & TSKID_NUCLEAR) != 0){
			eetp = TSKsetTopName(tsk_bonus, GROUP_INFO, NAME_INFO);
			if(eetp != NULL){
				eetp->pos[X] = tp->pos[X];
				eetp->pos[Y] = tp->pos[Y];
				eetp->num = total_bonus;
			}
		}
	}else{
		tp->tskid |= TSKID_DAMAGE;
	}
}

void enemy_explode(TSK *tp, int type, int x, int y)
{
	float spd;

	switch(type){
		case EXPTYPE_SMALL_RAND:
			spd = sqrt((double)(tp->vec[X] * tp->vec[X] + tp->vec[Y] * tp->vec[Y]));
			if(spd < 2.0f){
				tsk_explodeS1_manage(tp, x, y);
			}else{
				tsk_explodeS2_manage(tp, x, y);
			}
			break;
		case EXPTYPE_SMALL_TYPE1:
			tsk_explodeS1_manage(tp, x, y);
			break;
		case EXPTYPE_SMALL_TYPE2:
			tsk_explodeS2_manage(tp, x, y);
			break;
		case EXPTYPE_LARGE_RAND:
			if((rand() % 100) > 50){
				tsk_explodeL1_manage(tp, x, y);
			}else{
				tsk_explodeL2_manage(tp, x, y);
			}
			break;
		case EXPTYPE_LARGE_TYPE1:
			tsk_explodeL1_manage(tp, x, y);
			break;
		case EXPTYPE_LARGE_TYPE2:
			tsk_explodeL2_manage(tp, x, y);
			break;
		default:
			break;
	}
}

void enemy_zako_explode(TSK *tp)
{
	enemy_explode(tp, EXPTYPE_SMALL_RAND, 0, 0);
}

void enemy_score(Sint32 add_score)
{
	if(game_over == SDL_TRUE) return;

	score += add_score;
}

void enemy_get_playerside(void)
{
	if(ship_tsk != NULL){
		player_side = ship_tsk->pos[X] < 320.0f ? -1 : 1;
	}else{
		player_side = 0;
	}
}

float enemy_getaim(TSK *tp, float dir)
{
	float rot = 0.0f;

	rot = (float)atan2((double)(ship_tsk->pos[Y] - tp->pos[Y]), (double)(ship_tsk->pos[X] - tp->pos[X]));
	rot += dir;

	return rot;
}

void enemy_shot_fire(TSK *tp, int type, float dir, int x, int y)
{
	TSK *etp;

	etp = TSKsetTopName(tsk_eshot, GROUP_ESHOT, NAME_ESHOT);
	if(etp != NULL){
		etp->pos[X] = tp->pos[X] + x;
		etp->pos[Y] = tp->pos[Y] + y;
		etp->rot[X] = enemy_getaim(etp, dir);
		etp->rot[Y] = etp->rot[X];
		etp->mode = type;
		TSKexecDirect(etp);
	}
}

void tsk_core_small(TSK *tp)
{
	TSK *etp = tp->tsk_parent;

	switch(tp->step){
		case STEP_INIT:
			tp->tskid |= TSKID_NODMG;
			tp->exit_adr = tsk_core_small_exit;
			tp->draw_adr = tsk_core_small_draw;
			tsk_core_small_init(tp);
			tsk_core_small_move(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(etp != NULL && etp->step != STEP_EXIT){
				tsk_core_small_anim(tp);
				tsk_core_small_move(tp);
				break;
			}
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_core_small_exit(TSK *tp)
{
}

void tsk_core_small_draw(TSK *tp)
{
	SDL_wheel_DispSurface(surface[core_small_pat[tp->pat]], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], 0, 0, 64, 64);
}

void tsk_core_small_init(TSK *tp)
{
	tp->pat = 0;
	tp->wait = 0;
}

void tsk_core_small_anim(TSK *tp)
{
	tp->wait++;
	if(tp->wait >= 4){
		tp->pat++;
		tp->pat %= 4;
		tp->wait = 0;
	}
}

void tsk_core_small_move(TSK *tp)
{
	TSK *etp = tp->tsk_parent;

	if(etp != NULL && etp->step != STEP_EXIT){
		tp->pos[X] = etp->pos[X] + tp->ofs[X];
		tp->pos[Y] = etp->pos[Y] + tp->ofs[Y];
	}
}

void tsk_core_middle(TSK *tp)
{
	TSK *etp = tp->tsk_parent;

	switch(tp->step){
		case STEP_INIT:
			tp->tskid |= TSKID_NODMG;
			tp->exit_adr = tsk_core_middle_exit;
			tp->draw_adr = tsk_core_middle_draw;
			tsk_core_middle_init(tp);
			tsk_core_middle_move(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(etp != NULL && etp->step != STEP_EXIT){
				tsk_core_middle_anim(tp);
				tsk_core_middle_move(tp);
				break;
			}
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_core_middle_exit(TSK *tp)
{
}

void tsk_core_middle_draw(TSK *tp)
{
	SDL_wheel_DispSurface(surface[core_middle_pat[tp->pat]], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], 0, 0, 64, 64);
}

void tsk_core_middle_init(TSK *tp)
{
	tp->pat = 0;
	tp->wait = 0;
}

void tsk_core_middle_anim(TSK *tp)
{
	tp->wait++;
	if(tp->wait >= 4){
		tp->pat++;
		tp->pat %= 4;
		tp->wait = 0;
	}
}

void tsk_core_middle_move(TSK *tp)
{
	TSK *etp = tp->tsk_parent;

	if(etp != NULL && etp->step != STEP_EXIT){
		tp->pos[X] = etp->pos[X] + tp->ofs[X];
		tp->pos[Y] = etp->pos[Y] + tp->ofs[Y];
	}
}
