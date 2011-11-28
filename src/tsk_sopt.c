#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define OFS_X (28.0f)
#define OFS_Y (32.0f)
#define OPT_RATE1 (3.712375f)

static void tsk_sopt_exit(TSK *tp);
static void tsk_sopt_intr(TSK *tp);
static void tsk_sopt_draw(TSK *tp);
static void tsk_sopt_init(TSK *tp);
static void tsk_sopt_move(TSK *tp);
static void tsk_sopt_anim(TSK *tp);

static void tsk_sopt_particle(TSK *tp);
static void tsk_sopt_particle_exit(TSK *tp);
static void tsk_sopt_particle_draw(TSK *tp);
static void tsk_sopt_particle_init(TSK *tp);
static void tsk_sopt_particle_move(TSK *tp);
static void tsk_sopt_particle_anim(TSK *tp);

static SDL_Rect anim_rect[] = {
	{   0,   0,  48,  48 },
};

void tsk_sopt(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_sopt_exit;
			tp->intr_adr = tsk_sopt_intr;
			tp->draw_adr = tsk_sopt_draw;
			tsk_sopt_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(player_lock == SDL_FALSE){
				tsk_sopt_move(tp);
				tsk_sopt_anim(tp);
			}else{
				tp->pat = -8;
				if(ship_tsk != NULL){
					tp->draw_adr = tsk_sopt_draw;
					tp->pos[X] = ship_tsk->pos[X];
					tp->pos[Y] = ship_tsk->pos[Y] + 8.0f;
				}else{
					tp->draw_adr = NULL;
				}
			}
			break;

		/* game over */
		case STEP_MAIN+10:
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_sopt_exit(TSK *tp)
{
}

void tsk_sopt_intr(TSK *tp)
{
}

void tsk_sopt_draw(TSK *tp)
{
	SDL_Rect *p;

	if((tp->cnt & 0x01) == 0){
		p = &anim_rect[0];
		SDL_wheel_DispSurface(surface[SURFACE_EFFBALL24 + tp->pat], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
	}
}

void tsk_sopt_init(TSK *tp)
{
}

void tsk_sopt_move(TSK *tp)
{
	TSK *etp = tp->tsk_parent;

	if(etp == NULL) return;

	if(tp->mode == 0){
		tp->trg[X] = etp->pos[X] - OFS_X;
	}else{
		tp->trg[X] = etp->pos[X] + OFS_X;
	}
	tp->trg[Y] = etp->pos[Y] + OFS_Y;

	tp->pos[X] += (tp->trg[X] - tp->pos[X]) / OPT_RATE1;
	tp->pos[Y] += (tp->trg[Y] - tp->pos[Y]) / OPT_RATE1;

	etp = TSKsetTopName(tsk_sopt_particle, GROUP_EFFECT, NAME_EFCT);
	if(etp != NULL){
		etp->mode = tp->mode;
		etp->pos[X] = tp->pos[X];
		etp->pos[Y] = tp->pos[Y];
		TSKexecDirect(etp);
	}
}

void tsk_sopt_anim(TSK *tp)
{
	tp->cnt++;
	tp->pat++;
	tp->pat %= 8;
}

void tsk_sopt_particle(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_sopt_particle_exit;
			tp->draw_adr = tsk_sopt_particle_draw;
			tsk_sopt_particle_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tsk_sopt_particle_move(tp);
			tsk_sopt_particle_anim(tp);
			if(tp->pat >= 0) break;
			tp->draw_adr = NULL;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_sopt_particle_exit(TSK *tp)
{
}

void tsk_sopt_particle_draw(TSK *tp)
{
	SDL_Rect *p;

	if((tp->cnt % 2) == 0){
		p = &anim_rect[0];
		SDL_wheel_DispSurface(surface[SURFACE_EFFBALL01 + tp->pat], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
	}
}

void tsk_sopt_particle_init(TSK *tp)
{
	tp->vel[X] = (sin(FUNCrandFloat(1.5f) - 0.75f)) * 1.750f;
	tp->vel[Y] = (cos(0.0f)) * (FUNCrandFloat(4.0f) + 4.0f);
	tp->pat = SURFACE_EFFBALL13 - SURFACE_EFFBALL01;
}

void tsk_sopt_particle_move(TSK *tp)
{
	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];
}

void tsk_sopt_particle_anim(TSK *tp)
{
	tp->cnt++;
	tp->pat--;
}

