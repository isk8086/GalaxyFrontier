#include "global.h"
#include "game.h"

SEQUENCE stage01[] = {
	{ CMD_WAIT       ,         1,          0,              0,  0, NULL },

	{ CMD_TIME_START ,         0,          0,              0,  0, NULL },
	{ CMD_BG01_SET   ,  GROUP_BG, NAME_BGEAR,              0,  0, NULL },
	{ CMD_BG_STEP    ,  GROUP_BG, NAME_BGEAR,  STEP_MAIN + 1,  0, NULL },
	{ CMD_BGM_REQUEST, BGM_STG01,          0,              0,  0, NULL },
	{ CMD_SET_SCENE  ,    STG_01,          0,              0,  0, NULL },

	{ CMD_LABEL      ,         0,          0,              0,  0, "LABEL1" },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        45,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        45,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        45,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        45,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          4,              2,  0, NULL },
	{ CMD_WAIT       ,        20,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          4,              2,  0, NULL },
	{ CMD_WAIT       ,        20,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          4,              2,  0, NULL },
	{ CMD_WAIT       ,        20,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          4,              2,  0, NULL },
	{ CMD_WAIT       ,        20,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          4,              2,  0, NULL },
	{ CMD_WAIT       ,        20,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          4,              2,  0, NULL },
	{ CMD_WAIT       ,        20,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          4,              2,  0, NULL },
	{ CMD_WAIT       ,        20,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          4,              2,  0, NULL },
	{ CMD_WAIT       ,        20,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        60,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        60,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        60,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              2,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        60,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        60,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_01,          8,              4,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_02,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_03,          8,              8,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_BGM_FADE   ,        90,          0,              0,  0, NULL },

	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },
	{ CMD_PLAYER_SIDE,         0,          0,              0,  0, NULL },
	{ CMD_SET_ENEMY  ,   ZAKO_04,          2,              0,  0, NULL },
	{ CMD_WAIT       ,        15,          0,              0,  0, NULL },

	{ CMD_SET_ENEMY  ,   MIDD_01,          1,              0,  0, NULL },
	{ CMD_WAIT       ,        30,          0,              0,  0, NULL },



	// debug loop sequence
//	{ CMD_WAIT       ,       120,          0,              0,  0, NULL },
//	{ CMD_JUMP       ,         0,          0,              0,  0, "LABEL1" },

	{ CMD_SEQ_NEXT   ,    STG_02,          0,              0,  0, NULL },
//	{ CMD_TIME_STOP  ,         0,          0,              0,  0, NULL },

	{ CMD_TERMINATE  , 0, 0, 0, 0, NULL }
};

static void tsk_scene01_draw(TSK *tp);
static void tsk_scene01_exit(TSK *tp);
static void tsk_scene01_init(TSK *tp);

#define SCENE01_WAIT_1 (60 * 45)
#define SCENE01_WAIT_2 (60 * 60)
#define SCENE01_WAIT_3 (60 * 65)
#define SCENE01_WAIT_4 (60 * 68)
#define SCENE01_WAIT_5 (60 * 70)

void tsk_scene01(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_scene01_draw;
			tp->exit_adr = tsk_scene01_exit;
			tsk_scene01_init(tp);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tp->wait == (60 *  5)) SDL_wheel_PlaySe(VOICE_START);
			if(tp->wait == (60 * 10)) SDL_wheel_PlaySe(VOICE_ROGER);
			tp->wait++;
			if(tp->wait < SCENE01_WAIT_1) break;
			SDL_wheel_PlaySe(VOICE_EARTHOUT);
			set_bg01_acc(0.0f, 0.01f, 180);
			tp->step++;
			break;
		case STEP_MAIN+1:
			tp->wait++;
			if(tp->wait < SCENE01_WAIT_2) break;
			set_star_acc(0.0f, 0.01f, 1000);
			tp->step++;
			break;
		case STEP_MAIN+2:
			tp->wait++;
			if(tp->wait < SCENE01_WAIT_3) break;
			SDL_wheel_PlaySe(VOICE_WARPIN);
			tp->step++;
			break;
		case STEP_MAIN+3:
			tp->wait++;
			if(tp->wait < SCENE01_WAIT_4) break;
			SDL_wheel_PlaySe(VOICE_ROGER);
			tp->step++;
			break;
		case STEP_MAIN+4:
			tp->wait++;
			if(tp->wait < SCENE01_WAIT_5) break;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_scene01_draw(TSK *tp)
{
}

void tsk_scene01_exit(TSK *tp)
{
	puts("scene-1 control task exit.");
}

void tsk_scene01_init(TSK *tp)
{
	tp->wait = 0;
}
