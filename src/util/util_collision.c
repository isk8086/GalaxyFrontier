
#include "SDL.h"
#include "SDL_wheel.h"

#include "util_collision.h"

static void collision_sub(TSK *tp, int group);

void COLLISIONexec(void)
{
	TSK *tp;

	for(tp = TskIndex[GROUP_SSHOT][TSK_TOP].next_tsk; tp != &(TskIndex[GROUP_SSHOT][TSK_BOTTOM]); tp = tp->next_tsk){
		if(tp->intr_adr != NULL) collision_sub(tp, GROUP_ENEMY);
	}
	for(tp = TskIndex[GROUP_ESHOT][TSK_TOP].next_tsk; tp != &(TskIndex[GROUP_ESHOT][TSK_BOTTOM]); tp = tp->next_tsk){
		if(tp->intr_adr != NULL) collision_sub(tp, GROUP_SHIP);
	}
	for(tp = TskIndex[GROUP_ENEMY][TSK_TOP].next_tsk; tp != &(TskIndex[GROUP_ENEMY][TSK_BOTTOM]); tp = tp->next_tsk){
		if(tp->intr_adr != NULL) collision_sub(tp, GROUP_SHIP);
	}
}

void COLLISIONdraw(TSK *tp)
{
	int i;
	COLLISION * cp;
	Sint16 sx, sy, ex, ey;

	for(i = 0; i < tp->collision_num; i++){
		if(tp->collision != NULL){
			cp = &tp->collision[i];
			sx = (int)((tp->pos[X] + cp->pos[X]) - cp->col[X]);
			sy = (int)((tp->pos[Y] + cp->pos[Y]) - cp->col[Y]);
			ex = (int)((tp->pos[X] + cp->pos[X]) + cp->col[X]);
			ey = (int)((tp->pos[Y] + cp->pos[Y]) + cp->col[Y]);
			SDL_wheel_FillSurfacePrimary(sx, sy, ex, ey, SDL_wheel_MapRgba(COLLISION_R, COLLISION_G, COLLISION_B, COLLISION_A));
		}
	}
}

void collision_sub(TSK *tp, int group)
{
	int i, j;
	TSK *etp;
	COLLISION *cp;
	COLLISION *ecp;
	int	coll_flag;
	int	ssx, ssy;
	int	sex, sey;
	int	dsx, dsy;
	int	dex, dey;

	if(tp->pos[Y] < 0.0f) return;

	for(i = 0; i < tp->collision_num; i++){
		if(tp->pos[Y] < 0.0f) continue;
		if(tp->collision != NULL){
			cp = &tp->collision[i];
			ssx = (int)((tp->pos[X] + cp->pos[X]) - cp->col[X]);
			ssy = (int)((tp->pos[Y] + cp->pos[Y]) - cp->col[Y]);
			sex = (int)((tp->pos[X] + cp->pos[X]) + cp->col[X]);
			sey = (int)((tp->pos[Y] + cp->pos[Y]) + cp->col[Y]);
			for(etp = TskIndex[group][TSK_TOP].next_tsk; etp != &(TskIndex[group][TSK_BOTTOM]); etp = etp->next_tsk){
				if(etp->intr_adr != NULL){
					for(j = 0; j < etp->collision_num; j++){
						if(etp->pos[Y] < 0.0f) continue;
						if(etp->collision != NULL){
							ecp = &etp->collision[j];
							dsx = (int)((etp->pos[X] + ecp->pos[X]) - ecp->col[X]);
							dsy = (int)((etp->pos[Y] + ecp->pos[Y]) - ecp->col[Y]);
							dex = (int)((etp->pos[X] + ecp->pos[X]) + ecp->col[X]);
							dey = (int)((etp->pos[Y] + ecp->pos[Y]) + ecp->col[Y]);
							coll_flag = ((ssx - dex) & (dsx - sex) & (ssy - dey) & (dsy - sey)) >> 31;
							if(coll_flag){
								etp->tsk_collision = tp;
								etp->intr_adr(etp);
								tp->tsk_collision = etp;
								tp->intr_adr(tp);
								return;
							}
						}
					}
				}
			}
		}
	}
}
