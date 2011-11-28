#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define SSHOT_MINX ((float)SCR_LEFT   - 32.0f)
#define SSHOT_MAXX ((float)SCR_RIGHT  + 32.0f)
#define SSHOT_MINY ((float)SCR_TOP    - 32.0f)
#define SSHOT_MAXY ((float)SCR_BOTTOM + 32.0f)

#define SSHOT_ROT_CNT (16)
#define SSHOT_ROT_RATE ((PI * 2.0f) / (float)SSHOT_ROT_CNT)
#define SSHOT_ROT_HRATE (SSHOT_ROT_RATE / 2.0f)

static void tsk_sshot_exit(TSK *tp);
static void tsk_sshot_intr(TSK *tp);
static void tsk_sshot_draw(TSK *tp);
static void tsk_sshot_init(TSK *tp);
static SDL_bool tsk_sshot_move(TSK *tp);

static void tsk_sshot_touch(TSK *tp);
static void tsk_sshot_touch_exit(TSK *tp);
static void tsk_sshot_touch_draw(TSK *tp);
static void tsk_sshot_touch_init(TSK *tp);
static void tsk_sshot_touch_anim(TSK *tp);

static void tsk_sshot_particle(TSK *tp);
static void tsk_sshot_particle_exit(TSK *tp);
static void tsk_sshot_particle_draw(TSK *tp);
static void tsk_sshot_particle_init(TSK *tp);
static void tsk_sshot_particle_move(TSK *tp);
static void tsk_sshot_particle_anim(TSK *tp);

static SDL_Rect pat_rect[] = {
	{   0,   0,  64,   64 },
	{  64,   0, 128,   64 },
	{ 128,   0, 192,   64 },
	{ 192,   0, 256,   64 },
	{ 256,   0, 320,   64 },
	{ 320,   0, 384,   64 },
	{ 384,   0, 448,   64 },
	{ 448,   0, 512,   64 },
	{   0,  64,  64,  128 },
	{  64,  64, 128,  128 },
	{ 128,  64, 192,  128 },
	{ 192,  64, 256,  128 },
	{ 256,  64, 320,  128 },
	{ 320,  64, 384,  128 },
	{ 384,  64, 448,  128 },
	{ 448,  64, 512,  128 },
};

static SDL_Rect anim_rect_touch[] = {
	{   0,   0,  64,  64 },
	{  64,   0, 128,  64 },
	{ 128,   0, 192,  64 },
	{ 192,   0, 256,  64 },
};

static SDL_Rect anim_rect_efball[] = {
	{   0,   0,  48,  48 },
};

static int pat_num[] = {
	  0,  
	  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,
	  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14,
	 15, 15,
	  0,
};

static COLLISION sshot_collision[] = {
	{ {   +0.0f,   +0.0f }, {   +8.0f,   +8.0f } },
};

void tsk_sshot(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_sshot_exit;
			tp->intr_adr = tsk_sshot_intr;
			tp->draw_adr = tsk_sshot_draw;
			tp->num = 0;
			TSKsetCollision(tp, sshot_collision);
			tsk_sshot_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tp->num++;
			tp->num %= 8;
			if(tsk_sshot_move(tp) == SDL_TRUE) break;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_sshot_exit(TSK *tp)
{
}

void tsk_sshot_intr(TSK *tp)
{
	TSK *etp = tp->tsk_collision;
	int i;

	if(etp != NULL && etp->group_no == GROUP_ENEMY){
		tp->intr_adr = NULL;
		tp->step = STEP_EXIT;
		etp = TSKsetTopName(tsk_sshot_touch, GROUP_EFFECT, NAME_EFCT);
		if(etp != NULL){
			etp->pos[X] = tp->pos[X];
			etp->pos[Y] = tp->pos[Y];
			TSKexecDirect(etp);
		}
		for(i = 0; i < 4; i++){
			etp = TSKsetTopName(tsk_sshot_particle, GROUP_EFFECT, NAME_EFCT);
			if(etp != NULL){
				etp->pos[X] = tp->pos[X];
				etp->pos[Y] = tp->pos[Y];
				etp->vel[X] = tp->vel[X] / (FUNCrandFloat(3.0f) + 1.0f) + (FUNCrandFloat(2.0f) - 1.0f);
				etp->vel[Y] = tp->vel[Y] / (FUNCrandFloat(3.0f) + 1.0f);
				TSKexecDirect(etp);
			}
		}
	}
}

void tsk_sshot_draw(TSK *tp)
{
	SDL_Rect *p;
	Sint16 y, h;

	p = &pat_rect[tp->pat];
	y = p->y + tp->num * 128;
	h = p->h + tp->num * 128;
	if(tp->mode == 0) SDL_wheel_DispSurface(surface[SURFACE_SSHOT], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, y, p->w, h);
	else			  SDL_wheel_DispSurface(surface[SURFACE_SSHOT2], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, y, p->w, h);
}

void tsk_sshot_init(TSK *tp)
{
	float rot;

	if(tp->mode == 0){
		tp->energy = 5;
		tp->vel[X] = sin(PI - tp->rot[X]) * sshot_vel;
		tp->vel[Y] = cos(PI - tp->rot[Y]) * sshot_vel;
	}else{
		tp->energy = 15;
		tp->vel[X] = sin(PI - tp->rot[X]) * sshot_vel * 1.5f;
		tp->vel[Y] = cos(PI - tp->rot[Y]) * sshot_vel * 1.5f;
	}

	rot = tp->rot[X];
	if(rot < 0.0f) rot += PI * 2.0f;
	tp->pat = pat_num[(int)(rot / SSHOT_ROT_HRATE)];
}

SDL_bool tsk_sshot_move(TSK *tp)
{
	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];

	if(tp->pos[X] < SSHOT_MINX) return SDL_FALSE;
	if(tp->pos[X] > SSHOT_MAXX) return SDL_FALSE;
	if(tp->pos[Y] < SSHOT_MINY) return SDL_FALSE;
	if(tp->pos[Y] > SSHOT_MAXY) return SDL_FALSE;

	return SDL_TRUE;
}

void tsk_sshot_touch(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_sshot_touch_exit;
			tp->draw_adr = tsk_sshot_touch_draw;
			tsk_sshot_touch_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tsk_sshot_touch_anim(tp);
			if(tp->pat < 4) break;
			tp->draw_adr = NULL;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_sshot_touch_exit(TSK *tp)
{
}

void tsk_sshot_touch_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &anim_rect_touch[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_SSHOT_T], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_sshot_touch_init(TSK *tp)
{
	tp->pat = 0;
	tp->pat_wait = 0;
}

void tsk_sshot_touch_anim(TSK *tp)
{
	tp->pat++;
}

void tsk_sshot_particle(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_sshot_particle_exit;
			tp->draw_adr = tsk_sshot_particle_draw;
			tsk_sshot_particle_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tsk_sshot_particle_move(tp);
			tsk_sshot_particle_anim(tp);
			if(tp->pat >= 0) break;
			tp->draw_adr = NULL;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_sshot_particle_exit(TSK *tp)
{
}

void tsk_sshot_particle_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &anim_rect_efball[0];
	SDL_wheel_DispSurface(surface[SURFACE_EFFBALL01 + tp->pat], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_sshot_particle_init(TSK *tp)
{
	tp->pat = SURFACE_EFFBALL16 - SURFACE_EFFBALL01;
}

void tsk_sshot_particle_move(TSK *tp)
{
	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];
}

void tsk_sshot_particle_anim(TSK *tp)
{
	tp->pat--;
}

