#ifndef	__UTIL_COLLISION__
#define	__UTIL_COLLISION__

#include "util.h"
#include "util_tsk.h"

/* ================ */

/* collision color */
enum{
	COLLISION_R = 255,
	COLLISION_G = 0,
	COLLISION_B = 0,
	COLLISION_A = 255,

	COLLISION_MAX
};

/* ================ */

extern void COLLISIONexec(void);
extern void COLLISIONdraw(TSK *tp);

#endif	/* __UTIL_COLLISION__ */
