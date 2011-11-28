#include "global.h"
#include "game.h"

SEQUENCE stage03[] = {
	{ CMD_WAIT       ,         1,  0,  0,  0, NULL },

	{ CMD_TIME_START ,         0,  0,  0,  0, NULL },
	{ CMD_BGM_REQUEST, BGM_STG03,  0,  0,  0, NULL },

	{ CMD_LABEL      ,         0,  0,  0,  0, "LABEL1" },

	{ CMD_WAIT       ,         1,  0,  0,  0, NULL },

	{ CMD_JUMP       ,         0,  0,  0,  0, "LABEL1" },

	{ CMD_TIME_STOP  ,         0,  0,  0,  0, NULL },

	{ CMD_TERMINATE  , 0, 0, 0, 0, NULL }
};
