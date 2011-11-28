#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define BODY_SIZEX (368)
#define BODY_SIZEY (136)

static void tsk_boss01_exit(TSK *tp);
static void tsk_boss01_intr(TSK *tp);
static void tsk_boss01_draw(TSK *tp);
static void tsk_boss01_init(TSK *tp);
static SDL_bool tsk_boss01_move(TSK *tp);
static void tsk_boss01_shot(TSK *tp);

static void tsk_gauge(TSK *tp);
static void tsk_gauge_draw(TSK *tp);
static void tsk_gauge_exit(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0, 512, 256 },
};

static COLLISION zako_collision[] = {
	{ {   +0.0f,   +0.0f }, { BODY_SIZEX / 2, BODY_SIZEY / 2 } },
};

static int singleton = 0;

void tsk_boss01(TSK *tp)
{
	int x, y;

	switch(tp->step){
		case STEP_INIT:
			if(singleton == 1){
				TSKclear(tp);
				break;
			}
			singleton = 1;
			tp->exit_adr = tsk_boss01_exit;
			tp->intr_adr = tsk_boss01_intr;
			tp->draw_adr = tsk_boss01_draw;
			TSKsetCollision(tp, zako_collision);
			tsk_boss01_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tsk_boss01_move(tp) == SDL_TRUE){
				tsk_boss01_shot(tp);
				break;
			}
			tp->step = STEP_EXIT;
			break;

		case STEP_MAIN+1000:
			tp->i = 0;
			tp->step++;
		case STEP_MAIN+1001:
/*
			x = rand() % BODY_SIZEX - (BODY_SIZEX / 2);
			y = rand() % BODY_SIZEY - (BODY_SIZEY / 2);
			switch(rand() % 5){
				case 0: enemy_explode(tp, "がっかーん", x, y); break;
				case 1: enemy_explode(tp, "ぎっかーん", x, y); break;
				case 2: enemy_explode(tp, "ぐっかーん", x, y); break;
				case 3: enemy_explode(tp, "げっかーん", x, y); break;
				case 4: enemy_explode(tp, "ごっかーん", x, y); break;
			}
*/
			tp->i++;
			if(tp->i == 50){
				tp->draw_adr = NULL;
				tp->step = STEP_MAIN + 1003;
				break;
			}
			tp->wait = rand() % 3 + 2;
			tp->step++;
		case STEP_MAIN+1002:
			tsk_boss01_move(tp);
			tp->wait--;
			if(tp->wait == 0){
				tp->step--;
			}
			break;
		case STEP_MAIN+1003:
			for(tp->i = 0; tp->i < 16; tp->i++){
				x = rand() % BODY_SIZEX - (BODY_SIZEX / 2);
				y = rand() % BODY_SIZEY - (BODY_SIZEY / 2);
//				enemy_explode(tp, "どっかーん", x, y);
			}
			destroy_total++;
			enemy_score(100000);
			tp->step = STEP_EXIT;
			break;

		default:
			is_boss = SDL_FALSE;
			TSKclear(tp);
			break;
	}
}

void tsk_boss01_exit(TSK *tp)
{
	singleton = 0;
}

void tsk_boss01_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;

	if(etp != NULL && etp->group_no == GROUP_SSHOT){
		enemy_damage(tp, etp, 0, STEP_MAIN + 10000);
	}
}

void tsk_boss01_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &pat_rect[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_BOSS01], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_boss01_init(TSK *tp)
{
	TSK *etp;

	tp->pos[X] = +320.0f;
	tp->pos[Y] = -144.0f;
	tp->trg[X] = 320.0f;
	tp->trg[Y] = 144.0f;
	tp->rad[X] = 0.0f;
	tp->cnt = 0;
	tp->num = 0;
	tp->pat = 0;
	tp->energy = 750;
	/*tp->energy = 10;*/
	is_boss = SDL_TRUE;
	etp = TSKsetBtmName(tsk_gauge, GROUP_INFO, NAME_INFO);
	if(etp != NULL){
		etp->tsk_parent = tp;
	}
}

SDL_bool tsk_boss01_move(TSK *tp)
{
	switch(tp->mode){
		case 0:
			tp->pos[X] += (tp->trg[X] - tp->pos[X]) / 20.0f;
			tp->pos[Y] += (tp->trg[Y] - tp->pos[Y]) / 10.0f;
			tp->num++;
			if(tp->num < 80) break;
			tp->num = 0;
			tp->cen[X] = tp->pos[X];
			tp->cen[Y] = tp->pos[Y];
			tp->rad[X] = 16.0f;
			tp->rad[Y] = 8.0f;
			tp->rot[X] = 0.0f;
			tp->rot[Y] = 0.0f;
			tp->mode++;
			break;
		case 1:
			tp->rot[X] += PI / +258.2371265f;
			tp->rot[Y] += PI / -113.612537125f;
			tp->pos[X] = tp->cen[X] + sin(tp->rot[X]) * tp->rad[X];
			tp->pos[Y] = tp->cen[Y] + sin(tp->rot[Y]) * tp->rad[Y];
			break;
		default:
			break;
	}

	tp->cnt++;

	return SDL_TRUE;
}

void tsk_boss01_shot(TSK *tp)
{
	int shot = rand() % 3;
	int x = 0, y = 0;

	if((tp->cnt % 3) == 0){
		if(tp->mode != 0){
			x = (rand() % BODY_SIZEX) - (BODY_SIZEX / 2);
			y = (rand() % BODY_SIZEY) - (BODY_SIZEY / 2);
			switch(shot){
				case 0: enemy_shot_1way(tp, x, y); break;
				case 1: enemy_shot_2way(tp, x, y); break;
				case 2: enemy_shot_3way(tp, x, y); break;
				default:  break;
			}
		}
	}

	if((tp->cnt % 2) == 0){
		shot = rand() % 8;
		switch(shot){
			case 0: x =   +0; y =  -96; break;
			case 1: x =   +0; y =  +96; break;
			case 2: x = -224; y =   +0; break;
			case 3: x = +224; y =   +0; break;
			case 4: x = -216; y =  -88; break;
			case 5: x = +216; y =  -88; break;
			case 6: x = -216; y =  +88; break;
			case 7: x = +216; y =  +88; break;
			default:  break;
		}
		shot = rand() % 3;
		switch(shot){
			case 0: enemy_shot_1way(tp, x, y); break;
			case 1: enemy_shot_2way(tp, x, y); break;
			case 2: enemy_shot_3way(tp, x, y); break;
			default:  break;
		}
	}
}

void tsk_gauge(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_gauge_draw;
			tp->exit_adr = tsk_gauge_exit;
			tp->pos[X] = 192.0f;
			tp->pos[Y] = 16.0f;
			tp->num = tp->tsk_parent->energy;
			tp->energy = tp->tsk_parent->energy;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tp->energy = tp->tsk_parent->energy;
			if(tp->energy <= 0){
				tp->step = STEP_EXIT;
			}
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_gauge_draw(TSK *tp)
{
	int r_mode;
	float w;

	r_mode = SDL_wheel_SetRanderMode(RENDER_LEFTTOP);
	w = (384.0f * ((float)tp->energy / (float)tp->num));
	SDL_wheel_DispSurface(surface[SURFACE_ENERGY], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], 0, 0, (Sint16)w, 16);
	SDL_wheel_SetRanderMode(r_mode);
}

void tsk_gauge_exit(TSK *tp)
{
}

