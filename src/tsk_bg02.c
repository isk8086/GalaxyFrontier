#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

#define WARPBG_MAX (3)
#define WARPBG_PAGE (2)

static void tsk_bg02_draw(TSK *tp);
static void tsk_bg02_exit(TSK *tp);
static void tsk_bg02_init(TSK *tp);
static void tsk_bg02_move(TSK *tp);

static SDL_Rect pat_rect[] = {
	// warp01
	{  40, 232,  56, 472 },
	{  88,  40, 104, 344 },
	{ 224, 152, 240, 392 },
	{ 392,  64, 408, 432 },
	{ 528,  48, 544, 320 },
	{ 592, 104, 608, 432 },

	// warp02
	{  80,  32,  96, 344 },
	{ 192, 136, 208, 440 },
	{ 344,  40, 368, 352 },
	{ 456, 152, 472, 408 },
	{ 576,  40, 592, 360 },

	// warp03
	{ 144,  56, 160, 328 },
	{ 288, 104, 304, 344 },
	{ 424,  24, 448, 232 },
	{ 488, 192, 512, 464 },
};

static VEC bg_vel[WARPBG_MAX] = {
	{ +0.0f,  +3.123725f },
	{ +0.0f,  +8.613167f },
	{ +0.0f, +13.716845f },
};

static WARPBG warp_bg[WARPBG_MAX][WARPBG_PAGE];

static TSK *bg_tskp = NULL;

static float bg_alpha[WARPBG_MAX];
static float bg_alpha_add[WARPBG_MAX];

void tsk_bg02(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			bg_tskp = tp;
			tp->draw_adr = tsk_bg02_draw;
			tp->exit_adr = tsk_bg02_exit;
			tsk_bg02_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tsk_bg02_move(tp);
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_bg02_draw(TSK *tp)
{
	int r_mode;
	SDL_Rect *p;
	WARPBG *wp;
	int i, j, k;

	r_mode = SDL_wheel_SetRanderMode(RENDER_LEFTTOP);
	for(i = 0; i < WARPBG_MAX; i++){
		if(bg_alpha[i] <= 0.0f) continue;
		for(j = 0; j < WARPBG_PAGE; j++){
			wp = &warp_bg[i][j];
			SDL_wheel_SetAlpha(surface[wp->num], (int)(bg_alpha[i] * 255.0f));
			for(k = 0; k < wp->lines; k++){
				p = &pat_rect[wp->pat[k]];
				SDL_wheel_DispSurface(surface[wp->num], (Sint16)wp->pos.x + p->x, (Sint16)wp->pos.y + p->y, p->x, p->y, p->w, p->h);
			}
		}
	}
	SDL_wheel_SetRanderMode(r_mode);
}

void tsk_bg02_exit(TSK *tp)
{
	bg_tskp = NULL;
}

void tsk_bg02_init(TSK *tp)
{
	int i, j, k, l, n = 0;
	int max;

	for(i = 0; i < WARPBG_MAX; i++){
		bg_alpha[i] = 1.0f;
		bg_alpha_add[i] = 0.0f;
		max = 6 - i;
		for(j = 0; j < WARPBG_PAGE; j++){
			warp_bg[i][j].num = SURFACE_WARP01 + i;
			warp_bg[i][j].lines = max;
			warp_bg[i][j].alpha = 0.0f;
			warp_bg[i][j].pos.x = 0.0f;
			warp_bg[i][j].pos.y = -(float)pat_rect[0].h * (float)j;
			warp_bg[i][j].vel.x = bg_vel[i].x;
			warp_bg[i][j].vel.y = bg_vel[i].y;
			for(k = 0, l = 0; k < max; k++, l++){
				warp_bg[i][j].pat[k] = l + n;
			}
			//printf("warp velocity %d is %f,%f\n", i, warp_bg[i][j].vel.x, warp_bg[i][j].vel.y);
		}
		n += max;
	}

	tp->vel[X] = 0.0f;
	tp->vel[Y] = 0.0f;

	tp->wait = 0;
}

void tsk_bg02_move(TSK *tp)
{
	int i;

	if(tp->wait != 0){
		tp->wait--;
		tp->vel[Y] += tp->acc[Y];
	}

	for(i = 0; i < WARPBG_MAX; i++){
		warp_bg[i][0].pos.y += warp_bg[i][0].vel.y + tp->vel[Y];
		warp_bg[i][1].pos.y += warp_bg[i][1].vel.y + tp->vel[Y];
		if(warp_bg[i][0].pos.y >= (float)SCR_Y){
			warp_bg[i][0].pos.y -= (float)SCR_Y;
			warp_bg[i][1].pos.y -= (float)SCR_Y;
		}
		if(bg_alpha_add[i] != 0.0f){
			bg_alpha[i] += bg_alpha_add[i];
			if(bg_alpha[i] >= 1.0f){
				bg_alpha[i] = 1.0f;
				bg_alpha_add[i] = 0.0f;
			}
			if(bg_alpha[i] <= 0.0f){
				bg_alpha[i] = 0.0f;
				bg_alpha_add[i] = 0.0f;
			}
		}
	}
}

void set_bg02_acc(float x, float y, int frame)
{
	if(bg_tskp != NULL){
		bg_tskp->wait = frame;
		bg_tskp->acc[X] = x;
		bg_tskp->acc[Y] = y;
	}
}

void set_bg02_alpha(int n, float a)
{
	bg_alpha[n] = a;
}

void set_bg02_fadein(int n, float f)
{
	bg_alpha_add[n] = 1.0f / f;
}

void set_bg02_fadeout(int n, float f)
{
	bg_alpha_add[n] = -1.0f / f;
}
