#include "global.h"
#include "game.h"

SEQUENCE stage99[] = {
	{ CMD_WAIT       ,         1,          0,              0,  0, NULL },

	{ CMD_TIME_START ,         0,          0,              0,  0, NULL },
	{ CMD_BG01_SET   ,  GROUP_BG, NAME_BGEAR,              0,  0, NULL },
	{ CMD_BG_STEP    ,  GROUP_BG, NAME_BGEAR,  STEP_MAIN + 1,  0, NULL },
	{ CMD_BGM_REQUEST, BGM_STG01,          0,              0,  0, NULL },

	{ CMD_LABEL      ,         0,          0,              0,  0, "LABEL1" },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        60,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        60,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        60,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_JUMP       ,         0,          0,              0,  0, "LABEL1" },

	{ CMD_TIME_STOP  ,         0,          0,              0,  0, NULL },

	{ CMD_TERMINATE  , 0, 0, 0, 0, NULL }
};
