#ifndef	__GLOBAL_H__
#define	__GLOBAL_H__

#include "SDL.h"
#include "SDL_wheel.h"

enum {
	SCR_X = SCREEN_SX,
	SCR_Y = SCREEN_SY,

	SCR_HX = SCR_X / 2,
	SCR_HY = SCR_Y / 2,
};

typedef struct tagVEC VEC;
struct tagVEC{
	float x;
	float y;
};

typedef struct tagCOLOR COLOR;
struct tagCOLOR{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};

#include "util/util_tsk.h"
#include "util/util_func.h"
#include "util/util_collision.h"
#include "util/util_platinum.h"
#include "util/util_menu.h"

#define DEBUG_MENU (1)
#define DEBUG_MODE (1)

#define PANDORA_MODE (0)

#define ASSERT(exp){\
	if(exp == SDL_FALSE){\
		printf("assertion error : %s, (lines %d)\n", __FUNCTION__, __LINE__);\
		exit(-1);\
	}\
}

#endif	/* __GLOBAL_H__ */
