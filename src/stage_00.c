#include "global.h"
#include "game.h"

// game start opening
SEQUENCE stage00[] = {
	{ CMD_OPNBG_SET    ,   GROUP_BG2,   NAME_BGOPN,              0,  0, NULL },
	{ CMD_FADE         , GROUP_GCTRL,  SCRFADE_BIN,             30,  0, NULL },
	{ CMD_PLAYER_POS   ,         160,          388,              0,  0, NULL },
	{ CMD_PLAYER_LOCK  ,           0,            0,              0,  0, NULL },
	{ CMD_WAIT         ,          60,            0,              0,  0, NULL },
	{ CMD_PLAYER_BNON  ,           0,            0,              0,  0, NULL },
	{ CMD_OPNBG_SKIP   ,           0,            1,              0,  0, "LABEL1" },
	{ CMD_OPNBG_WAIT2  ,  NAME_BGOPN,            0,              0,  0, NULL },
	{ CMD_OPNBG_SKIP   ,           0,            0,              0,  0, "LABEL1" },
	{ CMD_BG01_SET     ,    GROUP_BG,   NAME_BGEAR,              0,  0, NULL },
	{ CMD_OPNBG_WAIT3  ,  NAME_BGOPN,            0,              0,  0, NULL },
	{ CMD_JUMP         ,           0,            0,              0,  0, "LABEL2" },

	{ CMD_LABEL        ,           0,            0,              0,  0, "LABEL1" },
	{ CMD_FADE         , GROUP_SSHOT, SCRFADE_BOUT,             30,  0, NULL },
	{ CMD_WAIT         ,          30,            0,              0,  0, NULL },
	{ CMD_BG01_SET     ,    GROUP_BG,   NAME_BGEAR,              0,  0, NULL },
	{ CMD_OPNBG_KILL   ,   GROUP_BG2,   NAME_BGOPN,              0,  0, NULL },
	{ CMD_PLAYER_BNKILL,           0,            0,              0,  0, NULL },
	{ CMD_PLAYER_UNLOCK,           0,            0,              0,  0, NULL },
	{ CMD_SE_STOP      ,           0,            0,              0,  0, NULL },
	{ CMD_FADE         , GROUP_SSHOT,  SCRFADE_BIN,             30,  0, NULL },

	{ CMD_LABEL        ,           0,            0,              0,  0, "LABEL2" },
	{ CMD_BG_STEP      ,    GROUP_BG,   NAME_BGEAR,  STEP_MAIN + 1,  0, NULL },
	{ CMD_PLAYER_BNKILL,           0,            0,              0,  0, NULL },
	{ CMD_PLAYER_UNLOCK,           0,            0,              0,  0, NULL },

	{ CMD_SEQ_NEXT     ,      STG_01,            0,              0,  0, NULL },

	{ CMD_TERMINATE    , 0, 0, 0, 0, NULL }
};
