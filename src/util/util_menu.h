#ifndef	__UTIL_MENU__
#define	__UTIL_MENU__

#include "SDL.h"

#include "util.h"

typedef struct tagMENUdata MENUdata;
struct tagMENUdata
{
	SDL_bool select;
	char *caption;
	char **item;
	int min;
	int max;
	int add;
	int now;
};

extern void MENUinit(int font_w, int font_g);
extern int MENUexec(MENUdata *menu, int *items, int items_max);
extern void MENUdraw(MENUdata *menu, int items, int items_max);

extern char *value_int[];
extern char *value_float[];

#endif	/* __UTIL_MENU__ */
