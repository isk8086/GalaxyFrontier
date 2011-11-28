#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

static TSK *demoplay_search(TSK *tp);
static TSK *demoplay_search_eshot(TSK *tp, float len);

void demoplay_pad(TSK *tp)
{
	TSK *etp;

	shippad_dat = PAD_BUTTON1;
	shippad_trg = 0;
	shippad_dir = 0;

	if(nuclear_exec == SDL_FALSE && nuclear_power >= NUCLEAR_RATE && (demoplay_search_eshot(tp, 16.0f) != NULL)){
		shippad_trg |= PAD_BUTTON2;
	}

	etp = demoplay_search(tp);
	if(etp != NULL){
		if((tp->pos[Y] - etp->pos[Y]) < +192.0f) shippad_dir |= PAD_DOWN;
		if((tp->pos[Y] - etp->pos[Y]) > +320.0f) shippad_dir |= PAD_UP;
		if((tp->pos[X] - etp->pos[X]) <   -8.0f) shippad_dir |= PAD_RIGHT;
		if((tp->pos[X] - etp->pos[X]) >   +8.0f) shippad_dir |= PAD_LEFT;
	}

	if(nuclear_exec == SDL_FALSE && (shippad_dat & PAD_BUTTON1) != 0){
		ship_demo_fire++;
		if(etp != NULL){
			if(etp->energy >= 30 && ship_demo_fire > 4){
				ship_demo_fire = 0;
				shippad_dat &= ~PAD_BUTTON1;
			}
		}
	}else{
		ship_demo_fire = 0;
	}

	tp->tsk_target = etp;
}

void demoplay_draw(TSK *tp)
{
	TSK *etp = tp->tsk_target;

	if(etp != NULL){
		SDL_wheel_PutLine(primary, (Sint16)tp->pos[X], (Sint16)tp->pos[Y], (Sint16)etp->pos[X], (Sint16)etp->pos[Y], SDL_wheel_MapRgba(255, 255, 255, 255));
	}
}

TSK *demoplay_search(TSK *tp)
{
	TSK *etp;
	TSK *ntp;
	TSK *stp = NULL;
	float len = 10000.0f, now, lpx, lpy;

	for(etp = TskIndex[GROUP_ENEMY][TSK_TOP].next_tsk; etp != &(TskIndex[GROUP_ENEMY][TSK_BOTTOM]); etp = ntp){
		ntp = etp->next_tsk;
		if(etp != tp){
			if(etp->pos[Y] <= 0) continue;
			if(etp->pos[Y] >= (float)SCR_Y) continue;
			if((etp->intr_adr == NULL) != 0) continue;
			if((etp->tskid & TSKID_NODMG) != 0) continue;
			lpx = fabs((etp->pos[X] * 1.00f) - tp->pos[X]);
			lpy = fabs((etp->pos[Y] * 1.25f) - tp->pos[Y]);
			lpx = pow(lpx, 2.0f);
			lpy = pow(lpy, 2.0f);
			now = sqrt(lpx + lpy);
			if(len > now){
				len = now;
				stp = etp;
			}
		}
	}

	return stp;
}

TSK *demoplay_search_eshot(TSK *tp, float len)
{
	TSK *etp;
	TSK *ntp;
	TSK *stp = NULL;
	float now, lpx, lpy;

	if(nuclear_exec == SDL_FALSE){
		for(etp = TskIndex[GROUP_ESHOT][TSK_TOP].next_tsk; etp != &(TskIndex[GROUP_ESHOT][TSK_BOTTOM]); etp = ntp){
			ntp = etp->next_tsk;
			if(etp != tp){
				if(etp->pos[Y] <= 0) continue;
				if(etp->pos[Y] > tp->pos[Y]) continue;
				if((etp->intr_adr == NULL) != 0) continue;
				if((etp->tskid & TSKID_NODMG) != 0) continue;
				lpx = fabs((etp->pos[X] * 1.00f) - tp->pos[X]);
				lpy = fabs((etp->pos[Y] * 1.25f) - tp->pos[Y]);
				lpx = pow(lpx, 2.0f);
				lpy = pow(lpy, 2.0f);
				now = sqrt(lpx + lpy);
				if(len > now){
					len = now;
					stp = etp;
				}
			}
		}
	}

	return stp;
}
