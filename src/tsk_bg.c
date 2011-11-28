#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

#define BG_SIZE (32)
#define BG_SPACE (8)

static void tsk_bg_draw(TSK *tp);
static void tsk_bg_exit(TSK *tp);
static void tsk_bg_init(TSK *tp);
static void setNextScroll(TSK *tp);

int bg_random;

static SDL_Rect rect[] = { 
							{ BG_SIZE * 1, BG_SIZE * 0, BG_SIZE * 2, BG_SIZE * 1 },
							{ BG_SIZE * 2, BG_SIZE * 0, BG_SIZE * 3, BG_SIZE * 1 },
							{ BG_SIZE * 3, BG_SIZE * 0, BG_SIZE * 4, BG_SIZE * 1 },
							{ BG_SIZE * 4, BG_SIZE * 0, BG_SIZE * 5, BG_SIZE * 1 },
							{ BG_SIZE * 5, BG_SIZE * 0, BG_SIZE * 6, BG_SIZE * 1 },
							{ BG_SIZE * 6, BG_SIZE * 0, BG_SIZE * 7, BG_SIZE * 1 },
							{ BG_SIZE * 7, BG_SIZE * 0, BG_SIZE * 8, BG_SIZE * 1 },
							{ BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 2, BG_SIZE * 2 },
							{ BG_SIZE * 0, BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 1 },
							{ BG_SIZE * 0, BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 1 },
							{ BG_SIZE * 0, BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 1 },
							{ BG_SIZE * 0, BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 1 },
							{ BG_SIZE * 0, BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 1 },
							{ BG_SIZE * 0, BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 1 },
							{ BG_SIZE * 0, BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 1 },
							{ BG_SIZE * 0, BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 1 },
							{ BG_SIZE * 0, BG_SIZE * 0, BG_SIZE * 1, BG_SIZE * 1 },
						};

static int bg_tile[(SCR_Y / BG_SIZE) * 2][(SCR_X / BG_SIZE) + 2];

static float scr_vely;

void tsk_bg(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_bg_draw;
			tp->exit_adr = tsk_bg_exit;
			tp->pos[X] = -(float)BG_SIZE / 2.0f;
			tp->pos[Y] = -(float)SCR_Y;
			tsk_bg_init(tp);
			bg_random = -1;
			scr_vely = +11.25f;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tp->vel[Y] = scr_vely;
			tp->pos[Y] += tp->vel[Y];
			if(tp->pos[Y] > +0.0f){
				setNextScroll(tp);
				tp->pos[Y] -= (float)SCR_Y;
			}
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_bg_draw(TSK *tp)
{
	int i, j, pat;
	float x, y;
	SDL_Rect *p;

	y = tp->pos[Y];
	for(i = 0; i < ((SCR_Y / BG_SIZE) * 2); i++){
		if((y + (float)(BG_SIZE / 2)) >= 0.0f && (y - (float)(BG_SIZE / 2)) <= (float)SCR_Y){
			x = tp->pos[X];
			for(j = 0; j < ((SCR_X / BG_SIZE) + 2); j++){
				pat = bg_tile[i][j];
				if(pat < BG_SPACE){
					p = &rect[pat];
					SDL_wheel_DispSurface(surface[SURFACE_BG00], (Sint16)x + cam[X], (Sint16)y + cam[Y], p->x, p->y, p->w, p->h);
				}
				x += (float)BG_SIZE;
			}
		}
		y += (float)BG_SIZE;
	}
}

void tsk_bg_exit(TSK *tp)
{
}

void tsk_bg_init(TSK *tp)
{
	int i, j;

	for(i = 0; i < ((SCR_Y / BG_SIZE) * 2); i++){
		for(j = 0; j < ((SCR_X / BG_SIZE) + 2); j++){
			bg_tile[i][j] = BG_SPACE;
		}
	}
}

void setNextScroll(TSK *tp)
{
	int i, j;

	for(i = 0; i < (SCR_Y / BG_SIZE); i++){
		for(j = 0; j < ((SCR_X / BG_SIZE) + 2); j++){
			bg_tile[i + (SCR_Y / BG_SIZE)][j] = bg_tile[i][j];
		}
	}

	for(i = 0; i < (SCR_Y / BG_SIZE); i++){
		for(j = 0; j < ((SCR_X / BG_SIZE) + 2); j++){
			if(bg_random != -1){
				bg_tile[i][j] = rand() % bg_random;
			}else{
				bg_tile[i][j] = BG_SPACE;
			}
		}
	}
}

