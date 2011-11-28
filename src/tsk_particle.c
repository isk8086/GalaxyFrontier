#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

static void tsk_particle(TSK *tp);
static void tsk_particle_draw(TSK *tp);

void set_particle(TSK *tp, int cnt, float x, float y, SDL_bool f, float ang, float ofs)
{
	int i;
	TSK *etp;

	if(f == SDL_FALSE) SDL_wheel_PlaySe(SE_DAMAGE);

	for(i = 0; i < cnt; i++){
		etp = TSKsetTopName(tsk_particle, GROUP_EFFECT, NAME_EFCT);
		if(etp != NULL){
			etp->pos[X] = tp->pos[X] + x;
			etp->pos[Y] = tp->pos[Y] + y;
			etp->rad[X] = FUNCrandFloat(8.0f) + 8.0f;
			if(f == SDL_FALSE){
				etp->rot[X] = FUNCrandFloat(PI * 2.0f);
			}else{
				etp->rot[X] = ang + (FUNCrandFloat(ofs) - (ofs / 2.0f));
			}
			TSKexecDirect(etp);
		}
		
	}
}

void tsk_particle(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_particle_draw;
			tp->vel[X] = sin(tp->rot[X]) * (tp->rad[X]);
			tp->vel[Y] = cos(tp->rot[X]) * (tp->rad[X]);
			tp->num = rand() % 3;
			tp->cnt = 0;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN:
			tp->pos[X] += tp->vel[X];
			tp->pos[Y] += tp->vel[Y];
			tp->cnt++;
			if(tp->cnt < 30) break;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_particle_draw(TSK *tp)
{
	Sint16 sx, sy;
	Sint16 ex, ey;

	sx = 4 * (tp->num % 4);
	sy = 0;
	ex = sx + 4;
	ey = sy + 4;

	SDL_wheel_DispSurface(surface[SURFACE_PARTICLE], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], sx, sy, ex, ey);
}
