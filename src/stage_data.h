#ifndef	__STAGE_DATA_H__
#define	__STAGE_DATA_H__

enum {
	CMD_INIT = 0,

	CMD_WAIT,
	CMD_LABEL,
	CMD_JUMP,

	CMD_BGM_REQUEST,
	CMD_BGM_FADE,

	CMD_SE_STOP,

	CMD_SURFACE_COLOR,

	CMD_SET_SCENE,

	CMD_SET_ENEMY,
	CMD_IS_MIDDLE,
	CMD_IS_BOSS,

	CMD_TIME_START,
	CMD_TIME_STOP,

	CMD_SCROLL_VEL,
	CMD_SCROLL_SWITCH,

	CMD_WARP_SET,
	CMD_WARP_WAIT,

	CMD_BG00_SET,
	CMD_BG01_SET,
	CMD_BG02_SET,
	CMD_BG02_ALPHA,
	CMD_BGMASK_SET,
	CMD_BGMASK_ALPHA,
	CMD_BG_STEP,

	CMD_FADE,

	CMD_PLAYER_SIDE,
	CMD_PLAYER_POS,
	CMD_PLAYER_LOCK,
	CMD_PLAYER_UNLOCK,
	CMD_PLAYER_BNON,
	CMD_PLAYER_BNOFF,
	CMD_PLAYER_BNKILL,

	CMD_OPNBG_SET,
	CMD_OPNBG_KILL,
	CMD_OPNBG_WAIT1,
	CMD_OPNBG_WAIT2,
	CMD_OPNBG_WAIT3,
	CMD_OPNBG_SKIP,

	CMD_SEQ_NEXT,

	CMD_PRINT,

	CMD_TERMINATE,

	CMD_BOTTOM
};

/* stage number */
enum{
	STG_INIT = 0,

	STG_00 = STG_INIT,
	STG_01,
	STG_02,
	STG_03,

	STG_99,

	STG_MAX
};

typedef struct tagSEQUENCE SEQUENCE;
struct tagSEQUENCE{
	int command;
	int param_val1;
	int param_val2;
	int param_val3;
	int param_val4;
	char *param_str1;
};

typedef struct tagLABEL LABEL;
struct tagLABEL{
	int line;
	char *name;
};

/* stage_data.c */
extern void STAGEinit(void);
extern void STAGEreset(void);
extern void STAGEcompile(int num);
extern void STAGEexec(void);
extern SDL_bool STAGEisExec(void);
/* stage_??.c */
extern SEQUENCE stage00[];
extern SEQUENCE stage01[];
extern SEQUENCE stage02[];
extern SEQUENCE stage03[];
extern SEQUENCE stage99[];
extern void tsk_scene01(TSK *tp);
extern void tsk_scene02(TSK *tp);

#endif	/* __STAGE_DATA_H__ */
