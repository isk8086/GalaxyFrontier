#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

static void tsk_quake_exit(TSK *tp);
static void tsk_quake_init(TSK *tp);

void tsk_quake(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_quake_exit;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			//cam[X] = (rand() % tp->pat * 2) - tp->pat;
			//cam[Y] = (rand() % tp->pat * 2) - tp->pat;
			cam[X] = (Sint16)(FUNCrandFloat(tp->ofs[X]) - (tp->ofs[X] / 2.0f));
			cam[Y] = (Sint16)(FUNCrandFloat(tp->ofs[Y]) - (tp->ofs[Y] / 2.0f));
			if(tp->cnt > 0){
				tp->cnt--;
				if(tp->cnt > 0){
					tp->ofs[X] -= tp->ftmp;
					tp->ofs[Y] -= tp->ftmp;
					break;
				}
				tp->step = STEP_EXIT;
			}
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_quake_exit(TSK *tp)
{
	cam[X] = 0;
	cam[Y] = 0;
}

void tsk_quake_init(TSK *tp)
{
}

