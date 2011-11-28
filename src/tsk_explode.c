#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

#define EXP_WAIT_SMALL1 (2)
#define EXP_WAIT_SMALL2 (1)

static void tsk_explodeS1(TSK *tp);
static void tsk_explodeS1_exit(TSK *tp);
static void tsk_explodeS1_draw(TSK *tp);
static void tsk_explodeS2(TSK *tp);
static void tsk_explodeS2_exit(TSK *tp);
static void tsk_explodeS2_draw(TSK *tp);
static void tsk_explodeL1(TSK *tp);
static void tsk_explodeL1_exit(TSK *tp);
static void tsk_explodeL1_draw(TSK *tp);
static void tsk_explodeL2(TSK *tp);
static void tsk_explodeL2_exit(TSK *tp);
static void tsk_explodeL2_draw(TSK *tp);

static void tsk_explode_move(TSK *tp);

static SDL_Rect anim_rect_small1[] = {
	{   0,   0,  80,  80 },
	{  80,   0, 160,  80 },
	{ 160,   0, 240,  80 },
	{ 240,   0, 320,  80 },
	{ 320,   0, 400,  80 },
	{ 400,   0, 480,  80 },
	{ 480,   0, 560,  80 },
	{ 560,   0, 640,  80 },
	{ 640,   0, 720,  80 },
	{ 720,   0, 800,  80 },
};

static SDL_Rect anim_rect_small2[] = {
	{   0,   0,  80,  80 },
	{  80,   0, 160,  80 },
	{ 160,   0, 240,  80 },
	{ 240,   0, 320,  80 },
	{ 320,   0, 400,  80 },
	{ 400,   0, 480,  80 },
	{ 480,   0, 560,  80 },
	{ 560,   0, 640,  80 },

	{   0,  80,  80, 160 },
	{  80,  80, 160, 160 },
	{ 160,  80, 240, 160 },
	{ 240,  80, 320, 160 },
	{ 320,  80, 400, 160 },
	{ 400,  80, 480, 160 },
	{ 480,  80, 560, 160 },
	{ 560,  80, 640, 160 },

	{   0, 160,  80, 240 },
	{  80, 160, 160, 240 },
	{ 160, 160, 240, 240 },
	{ 240, 160, 320, 240 },
	{ 320, 160, 400, 240 },
	{ 400, 160, 480, 240 },
	{ 480, 160, 560, 240 },
	{ 560, 160, 640, 240 },
};

void tsk_explodeS1_manage(TSK *tp, int x, int y)
{
	TSK *etp;

	etp = TSKsetBtmName(tsk_explodeS1, GROUP_EFFECT, NAME_EFCT);
	if(etp != NULL){
		etp->pos[X] = tp->pos[X] + x + FUNCrandFloat(16.0f) - 8.0f;
		etp->pos[Y] = tp->pos[Y] + y + FUNCrandFloat(16.0f) - 8.0f;
		etp->vel[X] = tp->vec[X];
		etp->vel[Y] = tp->vec[Y];
		TSKexecDirect(etp);
	}
}

void tsk_explodeS2_manage(TSK *tp, int x, int y)
{
	TSK *etp;

	etp = TSKsetBtmName(tsk_explodeS2, GROUP_EFFECT, NAME_EFCT);
	if(etp != NULL){
		etp->pos[X] = tp->pos[X] + x + FUNCrandFloat(16.0f) - 8.0f;
		etp->pos[Y] = tp->pos[Y] + y + FUNCrandFloat(16.0f) - 8.0f;
		etp->vel[X] = tp->vec[X];
		etp->vel[Y] = tp->vec[Y];
		TSKexecDirect(etp);
	}
}

void tsk_explodeL1_manage(TSK *tp, int x, int y)
{
	TSK *etp;

	etp = TSKsetBtmName(tsk_explodeL1, GROUP_EFFECT, NAME_EFCT);
	if(etp != NULL){
		etp->pos[X] = tp->pos[X] + x + FUNCrandFloat(32.0f) - 16.0f;
		etp->pos[Y] = tp->pos[Y] + y + FUNCrandFloat(32.0f) - 16.0f;
		etp->vel[X] = tp->vec[X];
		etp->vel[Y] = tp->vec[Y];
		TSKexecDirect(etp);
	}
}

void tsk_explodeL2_manage(TSK *tp, int x, int y)
{
	TSK *etp;

	etp = TSKsetBtmName(tsk_explodeL2, GROUP_EFFECT, NAME_EFCT);
	if(etp != NULL){
		etp->pos[X] = tp->pos[X] + x + FUNCrandFloat(32.0f) - 16.0f;
		etp->pos[Y] = tp->pos[Y] + y + FUNCrandFloat(32.0f) - 16.0f;
		etp->vel[X] = tp->vec[X];
		etp->vel[Y] = tp->vec[Y];
		TSKexecDirect(etp);
	}
}

void tsk_explodeS1(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_explodeS1_exit;
			tp->draw_adr = tsk_explodeS1_draw;
			tp->pat = 0;
			tp->wait = EXP_WAIT_SMALL1;
			SDL_wheel_PlaySe(SE_EXPLODE_S);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN:
			tsk_explode_move(tp);
			tp->wait--;
			if(tp->wait <= 0){
				tp->wait = EXP_WAIT_SMALL1;
				tp->pat++;
			}
			if(tp->pat < 10) break;
			tp->draw_adr = NULL;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_explodeS1_exit(TSK *tp)
{
}

void tsk_explodeS1_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &anim_rect_small1[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_EXPLODE_S1], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_explodeS2(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_explodeS2_exit;
			tp->draw_adr = tsk_explodeS2_draw;
			tp->pat = 0;
			tp->wait = EXP_WAIT_SMALL2;
			SDL_wheel_PlaySe(SE_EXPLODE_M);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN:
			tsk_explode_move(tp);
			tp->wait--;
			if(tp->wait <= 0){
				tp->wait = EXP_WAIT_SMALL2;
				tp->pat++;
			}
			if(tp->pat < 23) break;
			tp->draw_adr = NULL;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_explodeS2_exit(TSK *tp)
{
}

void tsk_explodeS2_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &anim_rect_small2[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_EXPLODE_S2], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);
}

void tsk_explodeL1(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_explodeL1_exit;
			tp->draw_adr = tsk_explodeL1_draw;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN:
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_explodeL1_exit(TSK *tp)
{
}

void tsk_explodeL1_draw(TSK *tp)
{
}

void tsk_explodeL2(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_explodeL2_exit;
			tp->draw_adr = tsk_explodeL2_draw;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN:
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_explodeL2_exit(TSK *tp)
{
}

void tsk_explodeL2_draw(TSK *tp)
{
}

void tsk_explode_move(TSK *tp)
{
	tp->pos[X] += tp->vel[X];
	tp->pos[Y] += tp->vel[Y];
}

