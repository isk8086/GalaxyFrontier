#ifndef	_UTIL_TSK_
#define	_UTIL_TSK_

#include "util.h"

/* ================ */

enum{
	GROUP_00 = 0,
	GROUP_01,
	GROUP_02,
	GROUP_03,
	GROUP_04,
	GROUP_05,
	GROUP_06,
	GROUP_07,
	GROUP_08,
	GROUP_09,
	GROUP_10,
	GROUP_MAX,

	GROUP_SYSTEM = GROUP_00,
	GROUP_BG = GROUP_01,
	GROUP_BG2 = GROUP_02,

	GROUP_SSHOT = GROUP_03,
	GROUP_ENEMY = GROUP_04,
	GROUP_EFFECT = GROUP_05,
	GROUP_ESHOT = GROUP_06,
	GROUP_SHIP = GROUP_07,
	GROUP_NSHOT = GROUP_07,

	GROUP_MENU = GROUP_09,
	GROUP_INFO = GROUP_09,
	GROUP_GCTRL = GROUP_10,

	TSK_TOP = 0,
	TSK_BOTTOM = 1,

	TSK_GROUP_MAX = GROUP_MAX,
	TSK_MAX = 10000,

	STEP_INIT = 0,
	STEP_MAIN = 100,
	STEP_EXIT = -1,
};

/* ================ */

#define MAKENAME(a,b,c,d) ((a << 24)|(b << 16)|(c << 8)|(d << 0))

#define TSKsetCollision(tp, cp)\
{\
	tp->collision = cp;\
	tp->collision_num = sizeof(cp) / sizeof(COLLISION);\
}

/* ================ */

#define	TSKID_NONE		0x00000000
#define	TSKID_EXIST		0x00000001
#define	TSKID_NUCLEAR	0x00010000
#define	TSKID_DAMAGE	0x10000000
#define	TSKID_NODMG		0x20000000
#define	TSKID_PAUSE		0x40000000
#define	TSKID_SYSTEM	0x80000000

/* ================ */

typedef struct tagCOLLISION COLLISION;
struct tagCOLLISION{
	float pos[XY];	/* offset position */
	float col[XY];	/* collision size */
};

typedef struct tagTSK TSK;
typedef void (*FUNC)(TSK *);
struct tagTSK{
	unsigned int tskid;
	unsigned int group_no;
	unsigned int name;

	TSK *prev_tsk;
	TSK *next_tsk;
	TSK *emptr;
	TSK *tsk_parent;
	TSK *tsk_child;
	TSK *tsk_collision;
	TSK *tsk_target;

	FUNC prog_adr;
	FUNC exit_adr;
	FUNC draw_adr;
	FUNC intr_adr;

	int id;
	int serial;
	int step;
	int next;
	int mode;
	int wait;
	int cnt;
	int num;
	int pat;
	int pat_wait;
	int tmp;
	int i, j, k;
	float frame;
	float ftmp;
	char *str_tmp;

	float pos[XY];
	float ofs[XY];
	float vec[XY];
	float vel[XY];
	float acc[XY];
	float cen[XY];
	float trg[XY];
	float rad[XY];
	float rot[XY];
	float ang[XY];

	int energy;
	float dest_rate;
	int collision_num;
	COLLISION *collision;

	void *work;
};

/* ================ */

extern void TSKinit(void);
extern TSK *TSKsetTop(FUNC exec_adr, int group);
extern TSK *TSKsetBtm(FUNC exec_adr, int group);
extern TSK *TSKsetTopName(FUNC exec_adr, int group, unsigned int name);
extern TSK *TSKsetBtmName(FUNC exec_adr, int group, unsigned int name);
extern void TSKexec(void);
extern void TSKexecDirect(TSK *tskp);
extern void TSKdraw(void);
extern void TSKintExec(TSK *tskp);
extern void TSKclear(TSK *tskp);
extern void TSKclearAll(TSK *tskp);
extern void TSKclearGroup(TSK *tskp, int group);
extern void TSKclearName(TSK *tp, unsigned int name);
extern void TSKclearGroupName(TSK *tp, int group, unsigned int name);
extern int TSKgetGroupCnt(int group);
extern int TSKgetGroupNameCnt(int group, unsigned int name);
extern TSK *TSKgetName(unsigned int name);
extern TSK *TSKgetGroupName(int group, unsigned int name);

extern TSK TskIndex[TSK_GROUP_MAX][2];
extern TSK TskBuf[TSK_MAX];
extern int TskEntCnt;

/* ================ */

#endif	/* _UTIL_TSK_ */

