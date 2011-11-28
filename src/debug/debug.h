#ifndef	__DEBUG_H__
#define	__DEBUG_H__

#include "../global.h"
#include "../game.h"

/* debug menu */
enum{
	DEBUG_MENU_TOP = DEBUG_MENU + 1,
	DEBUG_MENU_STAR,
	DEBUG_MENU_SHIP,
	DEBUG_MENU_SCROLL,
	DEBUG_MENU_ENEMY,
	DEBUG_MENU_SEQUENCE,
	DEBUG_MENU_EFFECT,
	DEBUG_MENU_SURFACE,
	DEBUG_MENU_SOUND,

	DEBUG_MENU_EXEC,

	DEBUG_MENU_MAX
};

typedef int (*DEBUG_FUNC)(int);

/* debug_main.c */
extern int debug_main(int mode);
extern SDL_bool debug_key_isCancel(void);
/* debug_menu_top.c */
extern void debug_menu_top_init(void);
extern int debug_menu_top(int mode);
extern int debug_menu_top_draw(int mode);
/* debug_menu_star.c */
extern void debug_menu_star_init(void);
extern int debug_menu_star(int mode);
extern int debug_menu_star_draw(int mode);
/* debug_menu_ship.c */
extern void debug_menu_ship_init(void);
extern int debug_menu_ship(int mode);
extern int debug_menu_ship_draw(int mode);
/* debug_menu_enemy.c */
extern void debug_menu_enemy_init(void);
extern int debug_menu_enemy(int mode);
extern int debug_menu_enemy_draw(int mode);
/* debug_menu_sequence.c */
extern void debug_menu_sequence_init(void);
extern int debug_menu_sequence(int mode);
extern int debug_menu_sequence_draw(int mode);
/* debug_menu_surface.c */
extern void debug_menu_surface_init(void);
extern int debug_menu_surface(int mode);
extern int debug_menu_surface_draw(int mode);
/* debug_menu_sound.c */
extern void debug_menu_sound_init(void);
extern int debug_menu_sound(int mode);
extern int debug_menu_sound_draw(int mode);

#endif	/* __DEBUG_H__ */

