#include "global.h"
#include "game.h"

SEQUENCE stage02[] = {
	{ CMD_WAIT          ,            1,          0,    0,  0, NULL },

	{ CMD_TIME_START    ,            0,          0,    0,  0, NULL },
	{ CMD_BGM_REQUEST   ,    BGM_STG02,          0,    0,  0, NULL },
	{ CMD_SET_SCENE     ,       STG_02,          0,    0,  0, NULL },
	{ CMD_SCROLL_VEL    ,            0,         10, 1000,  0, NULL },
	{ CMD_SURFACE_COLOR , SURFACE_MASK,          8,    8, 32, NULL },
	{ CMD_WARP_SET      ,            0,          0,    0,  0, NULL },

	{ CMD_BGMASK_SET    ,     GROUP_BG, NAME_BGMSK,    0,  0, NULL },
	{ CMD_BGMASK_ALPHA  ,            0,          0,    0,  0, NULL },
	{ CMD_BG02_SET      ,     GROUP_BG, NAME_BGWRP,    0,  0, NULL },
	{ CMD_BG02_ALPHA    ,            0,          0,    0,  0, NULL },
	{ CMD_BG02_ALPHA    ,            1,          0,    0,  0, NULL },
	{ CMD_BG02_ALPHA    ,            2,          0,    0,  0, NULL },

//	{ CMD_LABEL         ,            0,          0,    0,  0, "LABEL1" },
//	{ CMD_WAIT          ,            1,          0,    0,  0, NULL },
//	{ CMD_JUMP          ,            0,          0,    0,  0, "LABEL1" },

	{ CMD_WARP_WAIT     ,            1,          0,    0,  0, NULL },

	{ CMD_LABEL         ,            0,          0,    0,  0, "LABEL1" },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,   12,  0, NULL },
	{ CMD_WAIT          ,           60,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,   12,  0, NULL },
	{ CMD_WAIT          ,           60,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,   12,  0, NULL },
	{ CMD_WAIT          ,          120,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,    8,  0, NULL },
	{ CMD_WAIT          ,           60,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,    8,  0, NULL },
	{ CMD_WAIT          ,           60,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,    8,  0, NULL },
	{ CMD_WAIT          ,           60,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,    8,  0, NULL },
	{ CMD_WAIT          ,          120,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,    4,  0, NULL },
	{ CMD_WAIT          ,           60,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,    4,  0, NULL },
	{ CMD_WAIT          ,           60,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,    4,  0, NULL },
	{ CMD_WAIT          ,           60,          0,    0,  0, NULL },
	{ CMD_SET_ENEMY     ,      ZAKO_05,          4,    4,  0, NULL },
	{ CMD_WAIT          ,          120,          0,    0,  0, NULL },
	{ CMD_JUMP          ,            0,          0,    0,  0, "LABEL1" },

	{ CMD_TIME_STOP     ,            0,          0,    0,  0, NULL },

	{ CMD_TERMINATE, 0, 0, 0, 0, NULL }
};

static void tsk_scene02_draw(TSK *tp);
static void tsk_scene02_exit(TSK *tp);
static void tsk_scene02_init(TSK *tp);
static void tsk_scene02_quake(TSK *tp, int pow, int frame);

#define SCENE02_WAIT_1 (60 *  2)
#define SCENE02_WAIT_2 (60 *  6)
#define SCENE02_WAIT_3 (60 *  8)
#define SCENE02_WAIT_4 (60 *  9)
#define SCENE02_WAIT_5 (60 * 14)
#define SCENE02_WAIT_6 (60 * 15)
#define SCENE02_WAIT_7 (60 * 20)

#define ACC_FRAME (60 * 30)

void tsk_scene02(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_scene02_draw;
			tp->exit_adr = tsk_scene02_exit;
			tsk_scene02_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tp->wait++;
			if(tp->wait < SCENE02_WAIT_1) break;
			SDL_wheel_PlaySe(VOICE_WARPSTART1);
			tp->step++;
			break;
		case STEP_MAIN+1:
			tp->wait++;
			if(tp->wait < SCENE02_WAIT_2) break;
			SDL_wheel_PlaySe(SE_WARP_IN);
			tsk_scene02_quake(tp, 4, -1);
			tp->step++;
			break;
		case STEP_MAIN+2:
			tp->wait++;
			if(tp->wait < SCENE02_WAIT_3) break;
			set_bg02_fadein(2, 60.0f);
			tsk_scene02_quake(tp, 8, -1);
			tp->step++;
			break;
		case STEP_MAIN+3:
			tp->wait++;
			if(tp->wait < SCENE02_WAIT_4) break;
			set_bgmask_fadein(60.0f);
			tsk_scene02_quake(tp, 12, -1);
			tp->step++;
			break;
		case STEP_MAIN+4:
			tp->wait++;
			if(tp->wait < SCENE02_WAIT_5) break;
			set_bg02_fadein(0, 60.0f);
			set_star_switch(0);
			tp->step++;
			break;
		case STEP_MAIN+5:
			tp->wait++;
			if(tp->wait < SCENE02_WAIT_6) break;
			set_bg02_fadein(1, 60.0f);
			set_bg02_acc(0.0f, 16.0f / (float)ACC_FRAME, ACC_FRAME);
			tsk_scene02_quake(tp, 12, 60);
			tp->step++;
			break;
		case STEP_MAIN+6:
			tp->wait++;
			if(tp->wait < SCENE02_WAIT_7) break;
			warp_wait = 1;
			tp->step++;
			break;
		case STEP_MAIN+7:
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_scene02_draw(TSK *tp)
{
}

void tsk_scene02_exit(TSK *tp)
{
	puts("scene-2 control task exit.");
}

void tsk_scene02_init(TSK *tp)
{
	tp->wait = 0;
}

void tsk_scene02_quake(TSK *tp, int pow, int frame)
{
	TSK *etp;

	etp = TSKgetGroupName(GROUP_EFFECT, NAME_QUAK);
	if(etp == NULL){
		etp = TSKsetTopName(tsk_quake, GROUP_EFFECT, NAME_QUAK);
	}

	if(etp != NULL){
		etp->ofs[X] = (float)pow;
		etp->ofs[Y] = (float)pow;
		etp->cnt = frame;
		etp->ftmp = (float)pow / (float)frame;
		TSKexecDirect(etp);
	}
}
