#include "global.h"
#include "game.h"

/* global data */

SDL_Surface *primary;
SDL_Surface *font[FONT_MAX];
SDL_Surface *surface[SURFACE_MAX];

int debug_mode;

Uint8 *keys;
Uint32 pads;
Uint32 reps;
Uint32 trgs;

Uint32 shippad_dir;
Uint32 shippad_dat;
Uint32 shippad_trg;

SDL_bool pause_f;
SDL_bool pause;
SDL_bool pause_skip;
SDL_bool demo_play;
SDL_bool game_over;

Sint16 cam[XY];
float scroll[XY];
SDL_bool scroll_f;
int warp_wait;

int game_mode;
int game_level;

int opn_mode;

SDL_bool seq_continue;

int play_time;
int game_time;
SDL_bool time_exec;
int stg_num;
int scene;
Uint32 score;
Uint32 disp_score;
Sint32 bonus_score;
Sint32 total_bonus;

Sint32 sshot_first;
Sint32 sshot_wait;
float sshot_vel;

Sint32 nshot_level;
Sint32 nshot_wait;
SDL_bool nuclear_exec;

float nuclear_power;

float eshot_vel = ESHOT_VEL_BASE;

int ship_dead;
TSK *ship_tsk;
int ship_demo_fire;
int player_side;
SDL_bool player_lock;

FUNC enemy_adr[] = {
	tsk_zako01,
	tsk_zako02,
	tsk_zako03,
	tsk_zako04,
	tsk_zako05,
	tsk_zako06,
	tsk_zako07,
	tsk_zako08,

	tsk_middle01,
	tsk_middle02,

	tsk_boss01,

	NULL,
};

SDL_bool is_boss;
SDL_bool is_middle;

CONFIG config;
BACKUP backup;

Uint32 total_damage;
Uint32 move_total;
Uint32 destroy_total;
