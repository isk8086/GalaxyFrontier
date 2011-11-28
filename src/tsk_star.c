#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

#define STAR_MAX (256)
#define STAR_RAD_RATE (SCR_HX * 1.4142f)

static void tsk_star(TSK *tp);
static void tsk_star_draw(TSK *tp);
static void tsk_star_exit(TSK *tp);

static TSK *tskp_star = NULL;

static SDL_Rect rect[] = { 
							{  0, 0,  2, 2 },
							{  2, 0,  4, 2 },
							{  4, 0,  6, 2 },
							{  6, 0,  8, 2 },
							{  8, 0, 10, 2 },
							{ 10, 0, 12, 2 },
							{ 12, 0, 14, 2 },
						};

void set_star_stable(int cnt)
{
	TSK *etp;
	STAR *p;
	int i, j;

	if(cnt > STAR_MAX) cnt = STAR_MAX;

	if(tskp_star != NULL){
		TSKclear(tskp_star);
	}

	etp = TSKsetTopName(tsk_star, GROUP_BG, NAME_STAR);
	if(etp){
		etp->work = malloc(sizeof(STAR) * cnt);
		if(etp->work == NULL){
			TSKclear(etp);
			return;
		}
		tskp_star = etp;
		etp->cnt = cnt;
		etp->next = STEP_MAIN;
		for(i = 0; i < cnt; i++){
			j = rand() % 7;
			p = &((STAR *)etp->work)[i];
			p->pos.x = FUNCrandFloat((float)SCR_X);
			p->pos.y = FUNCrandFloat((float)SCR_Y);
			p->vel.x = 0.0f;
			p->vel.y = 0.0f;
			p->vel_org.x = 0.0f;
			p->vel_org.y = 0.0f;
			p->acc.x = 0.0f;
			p->acc.y = 0.0f;
			p->rect.x = rect[j].x;
			p->rect.y = rect[j].y;
			p->rect.w = rect[j].w;
			p->rect.h = rect[j].h;
			p->draw = (rand() % 100) > 50 ? SDL_TRUE : SDL_FALSE;
			p->wait = 0;
		}
	}
}

void set_star_vertical(int cnt)
{
	TSK *etp;
	STAR *p;
	int i, j;

	if(cnt > STAR_MAX) cnt = STAR_MAX;

	if(tskp_star != NULL){
		TSKclear(tskp_star);
	}

	etp = TSKsetTopName(tsk_star, GROUP_BG, NAME_STAR);
	if(etp){
		etp->work = malloc(sizeof(STAR) * cnt);
		if(etp->work == NULL){
			TSKclear(etp);
			return;
		}
		tskp_star = etp;
		etp->cnt = cnt;
		etp->next = STEP_MAIN;
		for(i = 0; i < cnt; i++){
			j = rand() % 7;
			p = &((STAR *)etp->work)[i];
			p->pos.x = FUNCrandFloat((float)SCR_X);
			p->pos.y = FUNCrandFloat((float)SCR_Y);
			p->vel.x = 0.0f;
			p->vel.y = FUNCrandFloat(3.75f) + 0.25f;
			p->vel_org.x = p->vel.x;
			p->vel_org.y = p->vel.y;
			p->acc.x = 0.0f;
			p->acc.y = 0.0f;
			p->rect.x = rect[j].x;
			p->rect.y = rect[j].y;
			p->rect.w = rect[j].w;
			p->rect.h = rect[j].h;
			p->draw = (rand() % 100) > 50 ? SDL_TRUE : SDL_FALSE;
			p->wait = 0;
		}
	}
}

void set_star_rolling(int cnt, float rad, float rot)
{
	TSK *etp;
	STAR *p;
	int i, j;
	int wait;

	if(cnt > STAR_MAX) cnt = STAR_MAX;

	if(tskp_star != NULL){
		TSKclear(tskp_star);
	}

	wait = STAR_MAX / cnt;

	etp = TSKsetTopName(tsk_star, GROUP_BG, NAME_STAR);
	if(etp){
		etp->work = malloc(sizeof(STAR) * cnt);
		if(etp->work == NULL){
			TSKclear(etp);
			return;
		}
		tskp_star = etp;
		etp->cnt = cnt;
		etp->next = STEP_MAIN + 1;
		for(i = 0; i < cnt; i++){
			j = rand() % 7;
			p = &((STAR *)etp->work)[i];
			p->cen.x = (float)SCR_X / 2.0f;
			p->cen.y = (float)SCR_Y / 2.0f;
			p->rot.x = FUNCrandFloat(PI * 2.0f);
			p->rot.y = p->rot.x;
			p->rot_add.x = rot;
			p->rot_add.y = rot;
			p->rad.x = 0.0f;
			p->rad.y = 0.0f;
			p->rad_add.x = rad;
			p->rad_add.y = rad;
			p->rect.x = rect[j].x;
			p->rect.y = rect[j].y;
			p->rect.w = rect[j].w;
			p->rect.h = rect[j].h;
			p->draw = SDL_FALSE;
			p->wait = wait * i;
		}
	}
}

void set_star_acc(float x, float y, int frame)
{
	int i;
	STAR *p;

	if(tskp_star != NULL){
		tskp_star->wait = frame;
		for(i = 0; i < tskp_star->cnt; i++){
			p = &((STAR *)tskp_star->work)[i];
			p->acc.x = x;
			p->acc.y = y;
		}
	}
}

void set_star_vel(float x, float y, int frame)
{
	int i, j;
	STAR *p;

	if(tskp_star != NULL){
		for(i = 0; i < tskp_star->cnt; i++){
			p = &((STAR *)tskp_star->work)[i];
			p->vel.x = p->vel_org.x;
			p->vel.y = p->vel_org.y;
			for(j = 0; j < frame; j++){
				p->vel.x += x;
				p->vel.y += y;
			}
		}
	}
}

void set_star_switch(int f)
{
	if(f == 1){
		scroll_f = SDL_TRUE;
	}else{
		scroll_f = SDL_FALSE;
	}
}

void tsk_star(TSK *tp)
{
	STAR *p;
	int i;

	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_star_draw;
			tp->exit_adr = tsk_star_exit;
			if(tp->next != 0) tp->step = tp->next;
			else			  tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tp->wait != 0){
				tp->wait--;
			}
			for(i = 0; i < tp->cnt; i++){
				p = &((STAR *)tp->work)[i];
				if(tp->wait == 0){
					p->acc.x = 0.0f;
					p->acc.y = 0.0f;
				}
				if(p->wait){
					p->wait--;
					p->draw = SDL_FALSE;
					continue;
				}
				p->pos.x += p->vel.x;
				p->pos.y += p->vel.y;
				p->vel.x += p->acc.x;
				p->vel.y += p->acc.y;
				if((p->vel.x + scroll[X]) >=0.0f){
					if(p->pos.x >= (float)SCR_X) p->pos.x -= (float)SCR_X;
				}else{
					if(p->pos.x < 0.0f) p->pos.x += (float)SCR_X;
				}
				if((p->vel.y + scroll[Y]) >=0.0f){
					if(p->pos.y >= (float)SCR_Y) p->pos.y -= (float)SCR_Y;
				}else{
					if(p->pos.y < 0.0f) p->pos.y += (float)SCR_Y;
				}
				p->draw = (rand() % 100) > 10 ? SDL_TRUE : SDL_FALSE;
			}
			break;

		case STEP_MAIN+1:
			for(i = 0; i < tp->cnt; i++){
				p = &((STAR *)tp->work)[i];
				if(p->wait){
					p->wait--;
					continue;
				}
				p->draw = SDL_TRUE;
				p->pos.x = p->cen.x + cos(p->rot.x) * p->rad.x;
				p->pos.y = p->cen.y + sin(p->rot.y) * p->rad.y;
				p->rot.x += p->rot_add.x;
				p->rot.y += p->rot_add.y;
				p->rad.x += p->rad_add.x;
				p->rad.y += p->rad_add.y;
				if(p->rad.x > STAR_RAD_RATE){
					p->rad.x -= STAR_RAD_RATE;
					p->rad.y -= STAR_RAD_RATE;
				}
			}
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_star_draw(TSK *tp)
{
	STAR *p;
	int i;

	if(scroll_f == SDL_TRUE){
		for(i = 0; i < tp->cnt; i++){
			p = &((STAR *)tp->work)[i];
			if(p->draw == SDL_TRUE) SDL_wheel_DispSurface(surface[SURFACE_STAR], p->pos.x + cam[X], p->pos.y + cam[Y], p->rect.x, p->rect.y, p->rect.w, p->rect.h);
		}
	}
}

void tsk_star_exit(TSK *tp)
{
	if(tp->work != NULL) free(tp->work);
	tp->work = NULL;
	tskp_star = NULL;
}
