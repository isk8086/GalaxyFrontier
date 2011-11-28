#ifndef	__GAME_H__
#define	__GAME_H__

#include "global.h"

/* Copyright Name */
#define COPYRIGHT_NAME "(C)HELLO WORLD PROJECT 2011"
#define COOPERATION_NAME "(C)ALPHA SECRET BASE 2011"

/* Application Name */
#define APP_NAME "GALAXY FRONTIER"

/* result file name */
#define FILENAME_RESULT "GalaxyFrontier_result.dat"

/* config file name */
#define FILENAME_CONFIG "GalaxyFrontier_config.dat"

/* macros */
#define convert_time(times, hour, min, se, msec) {\
		hour = times / (60 * 60 * 60);\
		min  = times / (60 * 60);\
		sec  = (times / 60) % 60;\
		msec = (int)(times % 60 * (100.0f / 60.0f));\
	}

/* screen */
enum{
	SCR_LEFT = 0,
	SCR_RIGHT = SCR_X,
	SCR_TOP = 0,
	SCR_BOTTOM = SCR_Y,

	SCR_MAX
};

/* fill color */
enum{
	FILL_R = 8,
	FILL_G = 8,
	FILL_B = 16,
	FILL_A = 255,

	FILL_MAX = 4
};

/* fonts */
enum{
	FONT_INIT = 0,

	FONT_01 = FONT_INIT,
	FONT_02,
	FONT_16x16,
	FONT_16x16G,

	FONT_MAX
};

#define FONT_H (18)
#define FONT_BOTTOM (6)

/* surfaces */
enum{
	SURFACE_INIT = 0,

	SURFACE_OFFSCREEN = FONT_INIT,
	SURFACE_SYSTEM,
	SURFACE_MASK,
	SURFACE_TITLE_BG,
	SURFACE_TITLE_LOGO1,
	SURFACE_TITLE_LOGO2,
	SURFACE_TITLE_SHIP,
	SURFACE_STAR,
	SURFACE_ENERGY,
	SURFACE_BG00,
	SURFACE_BG01,
	SURFACE_BG02,
	SURFACE_WARP01,
	SURFACE_WARP02,
	SURFACE_WARP03,
	SURFACE_EXPLODE_S1,
	SURFACE_EXPLODE_S2,
	SURFACE_EXPLODE_L1,
	SURFACE_EXPLODE_L2,
	SURFACE_PARTICLE,
	SURFACE_SHIP,
	SURFACE_SBANNER,
	SURFACE_SSHOT,
	SURFACE_SSHOT2,
	SURFACE_SSHOT_M,
	SURFACE_SSHOT_T,
	SURFACE_NSHOT,
	SURFACE_NUCLEAR,
	SURFACE_ESHOT,
	SURFACE_ESHOT_S,
	SURFACE_ESHOT_M,
	SURFACE_ESHOT_L,
	SURFACE_ZAKO01,
	SURFACE_ZAKO02,
	SURFACE_ZAKO03,
	SURFACE_ZAKO04,
	SURFACE_ZAKO05,
	SURFACE_ZAKO06,
	SURFACE_ZAKO07,
	SURFACE_ZAKO08,
	SURFACE_MIDL01,
	SURFACE_MIDL02,
	SURFACE_BOSS01,
	SURFACE_ZAKO01R,
	SURFACE_ZAKO02R,
	SURFACE_ZAKO03R,
	SURFACE_ZAKO04R,
	SURFACE_ZAKO05R,
	SURFACE_ZAKO06R,
	SURFACE_ZAKO07R,
	SURFACE_ZAKO08R,
	SURFACE_MIDL01R,
	SURFACE_MIDL02R,

	SURFACE_ZAKOSTONE1,
	SURFACE_ZAKOSTONE2,
	SURFACE_ZAKOSTONE3,
	SURFACE_ZAKOSTONE4,
	SURFACE_MIDDSTONE1,
	SURFACE_MIDDSTONE2,
	SURFACE_MIDDSTONE3,
	SURFACE_MIDDSTONE4,

	SURFACE_EFFBALL01,
	SURFACE_EFFBALL02,
	SURFACE_EFFBALL03,
	SURFACE_EFFBALL04,
	SURFACE_EFFBALL05,
	SURFACE_EFFBALL06,
	SURFACE_EFFBALL07,
	SURFACE_EFFBALL08,
	SURFACE_EFFBALL09,
	SURFACE_EFFBALL10,
	SURFACE_EFFBALL11,
	SURFACE_EFFBALL12,
	SURFACE_EFFBALL13,
	SURFACE_EFFBALL14,
	SURFACE_EFFBALL15,
	SURFACE_EFFBALL16,
	SURFACE_EFFBALL17,
	SURFACE_EFFBALL18,
	SURFACE_EFFBALL19,
	SURFACE_EFFBALL20,
	SURFACE_EFFBALL21,
	SURFACE_EFFBALL22,
	SURFACE_EFFBALL23,
	SURFACE_EFFBALL24,
	SURFACE_EFFBALL25,
	SURFACE_EFFBALL26,
	SURFACE_EFFBALL27,
	SURFACE_EFFBALL28,
	SURFACE_EFFBALL29,
	SURFACE_EFFBALL30,
	SURFACE_EFFBALL31,
	SURFACE_EFFBALL32,
	SURFACE_EFFBALL33,
	SURFACE_EFFBALL34,
	SURFACE_EFFBALL35,
	SURFACE_EFFBALL36,
	SURFACE_EFFBALL37,
	SURFACE_EFFBALL38,
	SURFACE_EFFBALL39,
	SURFACE_EFFBALL40,
	SURFACE_EFFBALL41,
	SURFACE_EFFBALL42,
	SURFACE_EFFBALL43,
	SURFACE_EFFBALL44,
	SURFACE_EFFBALL45,
	SURFACE_EFFBALL46,
	SURFACE_EFFBALL47,
	SURFACE_EFFBALL48,

	SURFACE_OPENING1,
	SURFACE_OPENING2,
	SURFACE_OPENING3,
	SURFACE_OPENING4,
	SURFACE_OPENING5,
	SURFACE_OPENING6,

	SURFACE_STARTDEMO1,
	SURFACE_STARTDEMO2,
	SURFACE_STARTDEMO3,

	SURFACE_MAX
};

/* sound */
enum{
	BGM_INIT = 0,

	BGM_OPENING = BGM_INIT,
	BGM_STG01,
	BGM_STG02,
	BGM_STG03,
	BGM_BOSS,
	BGM_BOSS_LAST,
	BGM_CLEAR,
	BGM_ENDING,
	BGM_RESULT,
	BGM_GAMEOVER,

	BGM_MAX,

	SE_INIT = 0,

	SE_CURSOR = SE_INIT,
	SE_COLUMN,
	SE_CORRECT,
	SE_CANCEL,
	SE_DAMAGE,
	SE_EXPLODE_S,
	SE_EXPLODE_M,
	SE_EXPLODE_L,
	SE_SSHOT,
	SE_NSHOT,
	SE_COLLIDE,
	SE_TITLE1,
	SE_TITLE2,
	SE_SORTIE,
	SE_DEPARTURE,
	SE_WARP_IN,
	SE_EWARP_IN,

	VOICE_ROGER,
	VOICE_START,
	VOICE_EARTHOUT,
	VOICE_WARPIN,
	VOICE_WARPSTART1,
	VOICE_WARPSTART2,

	SE_MAX
};

/* button */
enum{
	BTN_DEBUG_QUIT = PAD_BUTTON6,
	BTN_PAUSE_SKIP = PAD_BUTTON7,
	BTN_PAUSE = PAD_BUTTON8,
	BTN_EXIT = PAD_BUTTON9,

	BUTTON_MAX
};

/* fade */
enum{
	SCRFADE_INIT = 0,

	SCRFADE_BIN = SCRFADE_INIT,
	SCRFADE_BOUT,
	SCRFADE_WIN,
	SCRFADE_WOUT,

	SCRFADE_MAX
};

/* game mode */
enum{
	GAMEMODE_INIT = 0,

	GAMEMODE_SUPEREASY = GAMEMODE_INIT,
	GAMEMODE_SUPERHARD,

	GAMEMODE_MAX
};

/* enemys */
enum{
	ENEMY_INIT = 0,

	ZAKO_01 = ENEMY_INIT,
	ZAKO_02,
	ZAKO_03,
	ZAKO_04,
	ZAKO_05,
	ZAKO_06,
	ZAKO_07,
	ZAKO_08,

	MIDD_01,
	MIDD_02,

	BOSS_01,

	ENEMY_MAX
};

/* bullet */
enum{
	BULLET_INIT = 0,

	BULLET_S = BULLET_INIT,
	BULLET_M,
	BULLET_L,

	BULLET_MAX
};

/* explode */
enum{
	EXPTYPE_INIT = 0,

	EXPTYPE_SMALL_RAND = EXPTYPE_INIT,
	EXPTYPE_SMALL_TYPE1,
	EXPTYPE_SMALL_TYPE2,
	EXPTYPE_LARGE_RAND,
	EXPTYPE_LARGE_TYPE1,
	EXPTYPE_LARGE_TYPE2,
};

#define ESHOT_VEL_BASE (6.0f)

/* config */
typedef struct tagCONFIG CONFIG;
struct tagCONFIG{
	int game_mode;
	int vol_bgm;
	int vol_se;
};

/* backup */
typedef struct tagBACKUP BACKUP;
struct tagBACKUP{
	Sint32 high_score;
	Uint32 total_damage;
	Uint32 play_time;
	Uint32 play_cnt;
	Uint32 clear_cnt;

	Uint32 live_total;
	Uint32 live_short;
	Uint32 live_long;
	Uint32 live_average;

	Uint32 move_total;
	Uint32 move_short;
	Uint32 move_long;
	Uint32 move_average;

	Uint32 destroy_total;
	Uint32 destroy_short;
	Uint32 destroy_long;
	Uint32 destroy_average;
};

/* stars */
typedef struct tagSTAR STAR;
struct tagSTAR{
	VEC pos;
	VEC vel;
	VEC vel_org;
	VEC acc;
	VEC cen;
	VEC rot;
	VEC rot_add;
	VEC rad;
	VEC rad_add;
	SDL_Rect rect;
	SDL_bool draw;
	int wait;
};

/* warp bg */
typedef struct tagWARPBG WARPBG;
struct tagWARPBG{
	int num;
	int lines;
	int pat[6];
	float alpha;
	VEC pos;
	VEC vel;
};

/* ship define */
#define NUCLEAR_MAX (1.0f)
#define NUCLEAR_RATE (NUCLEAR_MAX / 10.0f)

#define SHIP_PAD_DIR (PAD_UP|PAD_DOWN|PAD_LEFT|PAD_RIGHT)
#define SHIP_PAD_HOR (PAD_LEFT|PAD_RIGHT)
#define SHIP_PAD_BTN (PAD_BUTTON1|PAD_BUTTON2|PAD_BUTTON3)

/* enemy define */
#define ENEMY_BASE_RATE (25000.0f)
#define ENEMY_DEFAULT_RATE (10.0f)

/* stage sequence */
#include "stage_data.h"

/* task name */
#define NAME_TITL  MAKENAME('T', 'I', 'T', 'L')
#define NAME_MENU  MAKENAME('M', 'E', 'N', 'U')
#define NAME_RSLT  MAKENAME('R', 'S', 'L', 'T')
#define NAME_OPTN  MAKENAME('O', 'P', 'T', 'N')
#define NAME_SCEN  MAKENAME('S', 'C', 'E', 'N')
#define NAME_INFO  MAKENAME('I', 'N', 'F', 'O')
#define NAME_STAR  MAKENAME('S', 'T', 'A', 'R')
#define NAME_BGSC  MAKENAME('B', 'G', 'S', 'C')
#define NAME_BGOPN MAKENAME('B', 'G', 'O', 'P')
#define NAME_BGEAR MAKENAME('B', 'G', 'E', 'R')
#define NAME_BGJUP MAKENAME('B', 'G', 'J', 'P')
#define NAME_BGWRP MAKENAME('B', 'G', 'W', 'P')
#define NAME_BGMSK MAKENAME('B', 'G', 'M', 'K')
#define NAME_SHIP  MAKENAME('S', 'H', 'I', 'P')
#define NAME_SBNR  MAKENAME('S', 'B', 'N', 'R')
#define NAME_SOPT  MAKENAME('S', 'O', 'P', 'T')
#define NAME_SSHOT MAKENAME('S', 'S', 'H', 'T')
#define NAME_NSHOT MAKENAME('N', 'S', 'H', 'T')
#define NAME_ESHOT MAKENAME('E', 'S', 'H', 'T')
#define NAME_ZAKO  MAKENAME('Z', 'A', 'K', 'O')
#define NAME_MIDD  MAKENAME('M', 'I', 'D', 'D')
#define NAME_BOSS  MAKENAME('B', 'O', 'S', 'S')
#define NAME_CORE  MAKENAME('C', 'O', 'R', 'E')
#define NAME_EFCT  MAKENAME('E', 'F', 'C', 'T')
#define NAME_QUAK  MAKENAME('Q', 'U', 'A', 'K')
#define NAME_FADE  MAKENAME('F', 'A', 'D', 'E')

/* main.c */
extern SDL_bool is_exec;
/* game.c */
extern void game_init(SDL_bool f);
extern void game_exit(void);
extern void game_exec(void);
extern void game_fillcolor_init(void);
extern void game_config_update(void);
extern void game_backup_update(void);
/* tsk_gctrl.c */
extern void tsk_gctrl(TSK *tp);
extern void tsk_gctrl_start(TSK *tp, int num);
extern void tsk_gctrl_release(TSK *tp);
/* tsk_title.c */
extern SDL_bool logo_disp;
extern SDL_bool is_start;
extern void tsk_title(TSK *tp);
/* tsk_menu.c */
extern void tsk_menu(TSK *tp);
/* tsk_option.c */
extern void tsk_option(TSK *tp);
/* tsk_result.c */
extern void tsk_result(TSK *tp);
/* tsk_info.c */
extern void tsk_info(TSK *tp);
extern void tsk_bonus(TSK *tp);
extern void tsk_scene(TSK *tp);
extern void tsk_gameover(TSK *tp);
extern void tsk_gameclear(TSK *tp);
/* tsk_star.c */
extern void set_star_stable(int cnt);
extern void set_star_vertical(int cnt);
extern void set_star_rolling(int cnt, float rad, float rot);
extern void set_star_acc(float x, float y, int frame);
extern void set_star_vel(float x, float y, int frame);
extern void set_star_switch(int f);
/* tsk_bg??.c */
extern void tsk_bg(TSK *tp);
extern void tsk_bg01(TSK *tp);
extern void tsk_bg02(TSK *tp);
extern void tsk_bgmask(TSK *tp);
extern void set_bg01_acc(float x, float y, int frame);
extern void set_bg02_acc(float x, float y, int frame);
extern void set_bg02_alpha(int n, float a);
extern void set_bg02_fadein(int n, float f);
extern void set_bg02_fadeout(int n, float f);
extern void set_bgmask_alpha(float a);
extern void set_bgmask_fadein(float f);
extern void set_bgmask_fadeout(float f);
/* tsk_opnbg.c */
extern void tsk_opnbg(TSK *tp);
/* tsk_quake.c */
extern void tsk_quake(TSK *tp);
/* tsk_fade.c */
extern void tsk_fade(TSK *tp);
/* tsk_explode.c */
extern void tsk_explodeS1_manage(TSK *tp, int x, int y);
extern void tsk_explodeS2_manage(TSK *tp, int x, int y);
extern void tsk_explodeL1_manage(TSK *tp, int x, int y);
extern void tsk_explodeL2_manage(TSK *tp, int x, int y);
/* tsk_particle.c */
extern void set_particle(TSK *tp, int cnt, float x, float y, SDL_bool f, float ang, float ofs);
/* tsk_ship.c */
extern void tsk_ship(TSK *tp);
extern void tsk_sbanner_on(void);
extern void tsk_sbanner_off(void);
extern void tsk_sbanner_kill(void);
/* tsk_sopt.c */
extern void tsk_sopt(TSK *tp);
/* tsk_muzzle.c */
extern void tsk_muzzle(TSK *tp);
/* tsk_sshot.c */
extern void tsk_sshot(TSK *tp);
/* tsk_nshot.c */
extern void tsk_nshot(TSK *tp);
/* tsk_nuclear.c */
extern void tsk_nuclear(TSK *tp);
/* tsk_eshot.c */
extern void tsk_eshot(TSK *tp);
/* tsk_zako.c */
extern void tsk_zako01(TSK *tp);
extern void tsk_zako02(TSK *tp);
extern void tsk_zako03(TSK *tp);
extern void tsk_zako04(TSK *tp);
extern void tsk_zako05(TSK *tp);
extern void tsk_zako06(TSK *tp);
extern void tsk_zako07(TSK *tp);
extern void tsk_zako08(TSK *tp);
extern void tsk_zako01_reset(void);
extern void tsk_zako02_reset(void);
extern void tsk_zako03_reset(void);
extern void tsk_zako04_reset(void);
extern void tsk_zako05_reset(void);
/* tsk_middle.c */
extern void tsk_middle01(TSK *tp);
extern void tsk_middle02(TSK *tp);
/* tsk_boss.c */
extern void tsk_boss01(TSK *tp);
/* enemy_cmn.c */
extern void enemy_reset(void);
extern void enemy_set_small_core(TSK *tp);
extern void enemy_set_small_core_pos(TSK *tp, int x, int y);
extern void enemy_set_middle_core(TSK *tp);
extern void enemy_set_middle_core_pos(TSK *tp, int x, int y);
extern void enemy_release_core(TSK *tp);
extern void enemy_release_core_all(TSK *tp);
extern void enemy_move_vec_reset(TSK *tp);
extern void enemy_move_vec_set(TSK *tp);
extern void enemy_calc_destrate(TSK *tp, float rate);
extern void enemy_shot_1way(TSK *tp, int x, int y);
extern void enemy_shot_2way(TSK *tp, int x, int y);
extern void enemy_shot_3way(TSK *tp, int x, int y);
extern void enemy_damage(TSK *tp, TSK *etp, int score, int dest_step);
extern void enemy_explode(TSK *tp, int type, int x, int y);
extern void enemy_zako_explode(TSK *tp);
extern void enemy_score(Sint32 add_score);
extern void enemy_get_playerside(void);
extern float enemy_getaim(TSK *tp, float dir);
/* demoplay.c */
extern void demoplay_pad(TSK *tp);
extern void demoplay_draw(TSK *tp);
/* data.c */
extern int debug_mode;
extern SDL_Surface *primary;
extern SDL_Surface *font[];
extern SDL_Surface *surface[];
extern Uint8 *keys;
extern Uint32 pads;
extern Uint32 reps;
extern Uint32 trgs;
extern Uint32 shippad_dir;
extern Uint32 shippad_dat;
extern Uint32 shippad_trg;
extern SDL_bool pause_f;
extern SDL_bool pause;
extern SDL_bool pause_skip;
extern SDL_bool demo_play;
extern SDL_bool game_over;
extern Sint16 cam[];
extern float scroll[];
extern SDL_bool scroll_f;
extern int warp_wait;
extern int game_mode;
extern int game_level;
extern int opn_mode;
extern SDL_bool seq_continue;
extern int play_time;
extern int game_time;
extern SDL_bool time_exec;
extern int stg_num;
extern int scene;
extern Uint32 score;
extern Uint32 disp_score;
extern Sint32 bonus_score;
extern Sint32 total_bonus;
extern Sint32 sshot_first;
extern Sint32 sshot_wait;
extern float sshot_vel;
extern Sint32 nshot_level;
extern Sint32 nshot_wait;
extern SDL_bool nuclear_exec;
extern float nuclear_power;
extern float eshot_vel;
extern int ship_dead;
extern TSK *ship_tsk;
extern int ship_demo_fire;
extern int player_side;
extern SDL_bool player_lock;
extern FUNC enemy_adr[];
extern SDL_bool is_boss;
extern SDL_bool is_middle;
extern CONFIG config;
extern BACKUP backup;
extern Uint32 total_damage;
extern Uint32 move_total;
extern Uint32 destroy_total;

#endif	/* __GAME_H__ */
