#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

//#define BG_SIZE (32)
//#define BG_SPACE (8)
#define ACC_SPD (1.0f / 8.0f)
#define MAX_SPD (16.0f)

static void tsk_opnbg_draw(TSK *tp);
static void tsk_opnbg_exit(TSK *tp);
static void tsk_opnbg_init(TSK *tp);
static void setNextScroll(TSK *tp);

static SDL_Rect anim_rect[] = {
	{ 160,   0, 480, 480 },
};

static float bg_pos[4];

void tsk_opnbg(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_opnbg_draw;
			tp->exit_adr = tsk_opnbg_exit;
			tsk_opnbg_init(tp);
			SDL_wheel_PlaySe(SE_SORTIE);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tp->wait--;
			if(tp->wait >= 0) break;
			if(tp->wait == -1) SDL_wheel_PlaySe(SE_DEPARTURE);
			bg_pos[0] += tp->vel[Y];
			bg_pos[1] += tp->vel[Y];
			bg_pos[2] += tp->vel[Y];
			bg_pos[3] += tp->vel[Y];
			tp->vel[Y] += tp->acc[Y];
			if(tp->vel[Y] >= MAX_SPD) tp->vel[Y] = MAX_SPD;
			if(bg_pos[0] >= (float)(SCR_Y)){
				opn_mode = 1;
				tp->mode = 1;
				tp->step++;
			}
			break;
		case STEP_MAIN+1:
			bg_pos[1] += tp->vel[Y];
			bg_pos[2] += tp->vel[Y];
			bg_pos[3] += tp->vel[Y];
			tp->vel[Y] += tp->acc[Y];
			if(tp->vel[Y] >= MAX_SPD) tp->vel[Y] = MAX_SPD;
			if(bg_pos[1] >= (float)(SCR_Y)){
				tp->cnt++;
				if(tp->cnt < 4){
					bg_pos[1] -= (float)SCR_Y;
					bg_pos[2] -= (float)SCR_Y;
					bg_pos[3] -= (float)SCR_Y;
					break;
				}
				opn_mode = 2;
				tp->mode = 2;
				tp->step++;
			}
			break;
		case STEP_MAIN+2:
			bg_pos[2] += tp->vel[Y];
			bg_pos[3] += tp->vel[Y];
			tp->vel[Y] += tp->acc[Y];
			if(tp->vel[Y] >= MAX_SPD) tp->vel[Y] = MAX_SPD;
			if(bg_pos[3] >= (float)(SCR_Y)){
				opn_mode = 3;
				tp->step = STEP_EXIT;
			}
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_opnbg_draw(TSK *tp)
{
	int r_mode;
	SDL_Rect *p;

	r_mode = SDL_wheel_SetRanderMode(RENDER_LEFTTOP);

	switch(tp->mode){
		case 0:
			p = &anim_rect[tp->pat];
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 0],   0.0f + cam[X], bg_pos[0] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 0], 320.0f + cam[X], bg_pos[0] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1],   0.0f + cam[X], bg_pos[1] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1], 320.0f + cam[X], bg_pos[1] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1],   0.0f + cam[X], bg_pos[2] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1], 320.0f + cam[X], bg_pos[2] + cam[Y], p->x, p->y, p->w, p->h);
			break;
		case 1:
			p = &anim_rect[tp->pat];
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1],   0.0f + cam[X], bg_pos[1] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1], 320.0f + cam[X], bg_pos[1] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1],   0.0f + cam[X], bg_pos[2] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1], 320.0f + cam[X], bg_pos[2] + cam[Y], p->x, p->y, p->w, p->h);
			break;
		case 2:
			p = &anim_rect[tp->pat];
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1],   0.0f + cam[X], bg_pos[2] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 1], 320.0f + cam[X], bg_pos[2] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 2],   0.0f + cam[X], bg_pos[3] + cam[Y], p->x, p->y, p->w, p->h);
			SDL_wheel_DispSurface(surface[SURFACE_STARTDEMO1 + 2], 320.0f + cam[X], bg_pos[3] + cam[Y], p->x, p->y, p->w, p->h);
			break;
		default:
			break;
	}

	SDL_wheel_SetRanderMode(r_mode);
}

void tsk_opnbg_exit(TSK *tp)
{
}

void tsk_opnbg_init(TSK *tp)
{
	tp->wait = 60;
	tp->cnt = 0;
	tp->mode = 0;
	tp->pat = 0;

	tp->vel[Y] = 0.0f;
	tp->acc[Y] = ACC_SPD;

	bg_pos[0] = -480.0f * 0.0f;
	bg_pos[1] = -480.0f * 1.0f;
	bg_pos[2] = -480.0f * 2.0f;
	bg_pos[3] = -480.0f * 3.0f;
}

void setNextScroll(TSK *tp)
{
}
