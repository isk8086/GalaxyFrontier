#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "game.h"

#define SHIP_NAN (0.0f)
#define SHIP_MX1 (8.0f / 4.0f * 4.0f)
#define SHIP_MY1 (8.0f / 4.0f * 3.0f)
#define SHIP_MX2 (SHIP_MX1 / SQRT)
#define SHIP_MY2 (SHIP_MY1 / SQRT)

#define SHIP_MINX ((float)SCR_LEFT   + 24.0f)
#define SHIP_MAXX ((float)SCR_RIGHT  - 24.0f)
#define SHIP_MINY ((float)SCR_TOP    + 24.0f)
#define SHIP_MAXY ((float)SCR_BOTTOM - 24.0f)

#define SHIP_PAT_WAIT (4)

#define SSHOT_ROT (PI / 180.0f * 30.0f)
#define SSHOT_ROT_RATE (SSHOT_ROT / 30.0f)

static void tsk_ship_exit(TSK *tp);
static void tsk_ship_intr(TSK *tp);
static void tsk_ship_draw(TSK *tp);
static void tsk_ship_init(TSK *tp);
static void tsk_ship_move(TSK *tp);
static void tsk_ship_shot(TSK *tp);
static void tsk_ship_nuclear(TSK *tp);
static void tsk_ship_shot_fire(TSK *tp, float x, float y, float mx, float my, float dir, SDL_bool f);
static void tsk_ship_anim(TSK *tp);
static void tsk_ship_check_gameover(TSK *tp);

static void tsk_sbanner(TSK *tp);
static void tsk_sbanner_exit(TSK *tp);
static void tsk_sbanner_draw(TSK *tp);
static void tsk_sbanner_init(TSK *tp);
static void tsk_sbanner_move(TSK *tp);

static SDL_Rect anim_rect_dir[] = {
	{   0,   0,  48,  48 },		/* left pattern 2 */
	{  48,   0,  96,  48 },		/* left pattern 1 */
	{  96,   0, 144,  48 },		/* center pattern */
	{ 144,   0, 192,  48 },		/* right pattern 1 */
	{ 192,   0, 240,  48 },		/* right pattern 2 */
};

static SDL_Rect anim_rect_banner[] = {
	{  0,   0,  8,  16 },
	{  8,   0, 16,  16 },
	{ 16,   0, 24,  16 },
	{ 24,   0, 32,  16 },
};

static COLLISION ship_collision[] = {
	{ {   +0.0f,   +0.0f }, {   +2.0f,   +2.0f } },
};

static const float ship_move[][2] = {
										{ +SHIP_NAN, +SHIP_NAN },	/* 0x00 none */
										{ +SHIP_NAN, -SHIP_MY1 },	/* 0x01 UP */
										{ +SHIP_NAN, +SHIP_MY1 },	/* 0x02 DOWN */
										{ +SHIP_NAN, +SHIP_NAN },	/* 0x03 UP+DOWN */
										{ -SHIP_MX1, +SHIP_NAN },	/* 0x04 LEFT */
										{ -SHIP_MX2, -SHIP_MY2 },	/* 0x05 UP+LEFT */
										{ -SHIP_MX2, +SHIP_MY2 },	/* 0x06 DOWN+LEFT */
										{ +SHIP_NAN, +SHIP_NAN },	/* 0x07 UP+DOWN+LEFT */
										{ +SHIP_MX1, +SHIP_NAN },	/* 0x08 RIGHT */
										{ +SHIP_MX2, -SHIP_MY2 },	/* 0x09 UP+RIGHT */
										{ +SHIP_MX2, +SHIP_MY2 },	/* 0x0A DOWN+RIGHT */
										{ +SHIP_NAN, +SHIP_NAN },	/* 0x0B UP+DOWN+RIGHT */
										{ +SHIP_NAN, +SHIP_NAN },	/* 0x0C LEFT+RIGHT */
										{ +SHIP_NAN, +SHIP_NAN },	/* 0x0D UP+LEFT+RIGHT */
										{ +SHIP_NAN, +SHIP_NAN },	/* 0x0E DOWN+LEFT+RIGHT */
										{ +SHIP_NAN, +SHIP_NAN },	/* 0x0F UP+DOWN+LEFT+RIGHT */
									};

static const float nuclear_cost = NUCLEAR_MAX / (float)(60 * 5);

static TSK *opt_l;
static TSK *opt_r;

static float sshot_rot;

static int sbanner_num[2];
static int sbanner_f;

void tsk_ship(TSK *tp)
{
	if(demo_play == SDL_TRUE){
		demoplay_pad(tp);
	}else{
		shippad_dir = pads & SHIP_PAD_DIR;
		shippad_dat = pads & SHIP_PAD_BTN;
		shippad_trg = trgs & SHIP_PAD_BTN;
	}

	switch(tp->step){
		case STEP_INIT:
			ship_tsk = tp;
			tp->exit_adr = tsk_ship_exit;
			tp->intr_adr = tsk_ship_intr;
			tp->draw_adr = tsk_ship_draw;
			tp->pos[X] = SCR_HX;
			tp->pos[Y] = SCR_HY * 1.5f;
			TSKsetCollision(tp, ship_collision);
			tp->pat = 2;
			tp->pat_wait = 0;
			ship_dead = 0;
			ship_demo_fire = 0;
			sshot_first = 5;
			sshot_rot = 0.0f;
			player_lock = SDL_FALSE;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tsk_ship_init(tp);
			tp->step++;
			break;
		case STEP_MAIN+1:
			if(player_lock == SDL_FALSE){
				tsk_ship_move(tp);
				tsk_ship_shot(tp);
				tsk_ship_nuclear(tp);
				tsk_ship_anim(tp);
			}else{
				if(opn_mode == 1 || opn_mode == 2){
					set_particle(tp, 1, -8.0f, +8.0f, SDL_TRUE, 0.0f, PI / 3.0f);
					set_particle(tp, 1, +8.0f, +8.0f, SDL_TRUE, 0.0f, PI / 3.0f);
				}
			}
			break;

		/* game over */
		case STEP_MAIN+10:
			break;

		default:
			TSKclear(tp);
			break;
	}

	tp->cnt++;
}

void tsk_ship_exit(TSK *tp)
{
	ship_tsk = NULL;
}

void tsk_ship_intr(TSK *tp)
{
	TSK *etp;

	return;

	if((tp->tskid & TSKID_NODMG) != 0) return;

	SDL_wheel_PlaySe(SE_COLLIDE);
	TSKclearGroupName(tp, GROUP_EFFECT, NAME_QUAK);
	etp = TSKsetTopName(tsk_quake, GROUP_EFFECT, NAME_EFCT);
	if(etp != NULL){
		TSKexecDirect(etp);
	}

	ship_dead++;
	tsk_ship_check_gameover(tp);

	etp = tp->tsk_collision;
	if(etp) return;
}

void tsk_ship_draw(TSK *tp)
{
	SDL_Rect *p;

	p = &anim_rect_dir[tp->pat];
	SDL_wheel_DispSurface(surface[SURFACE_SHIP], (Sint16)tp->pos[X] + cam[X], (Sint16)tp->pos[Y] + cam[Y], p->x, p->y, p->w, p->h);

	if(demo_play == SDL_TRUE){
		demoplay_draw(tp);
	}
}

void tsk_ship_init(TSK *tp)
{
	TSK *etp;

	tp->cnt = 0;

	sshot_wait = 0;
	sshot_vel = 21.73125f;
	nshot_level = 0;
	nshot_wait = 0;

	nuclear_power = NUCLEAR_MAX;
	nuclear_exec = SDL_FALSE;

	etp = TSKsetTopName(tsk_sopt, GROUP_SHIP, NAME_EFCT);
	if(etp != NULL){
		opt_l = etp;
		etp->tsk_parent = tp;
		etp->mode = 0;
		etp->pos[X] = tp->pos[X];
		etp->pos[Y] = tp->pos[Y];
		TSKexecDirect(etp);
	}
	etp = TSKsetTopName(tsk_sopt, GROUP_SHIP, NAME_EFCT);
	if(etp != NULL){
		opt_r = etp;
		etp->tsk_parent = tp;
		etp->mode = 1;
		etp->pos[X] = tp->pos[X];
		etp->pos[Y] = tp->pos[Y];
		TSKexecDirect(etp);
	}

	tsk_sbanner_off();
	etp = TSKsetBtmName(tsk_sbanner, GROUP_SHIP, NAME_SBNR);
	if(etp != NULL){
		etp->tsk_parent = tp;
		etp->pos[X] = tp->pos[X];
		etp->pos[Y] = tp->pos[Y];
		TSKexecDirect(etp);
	}
}

void tsk_ship_move(TSK *tp)
{
	tp->vec[X] = tp->pos[X];
	tp->vec[Y] = tp->pos[Y];
	tp->pos[X] += ship_move[shippad_dir][X];
	tp->pos[Y] += ship_move[shippad_dir][Y];
	tp->vec[X] = tp->pos[X] - tp->vec[X];
	tp->vec[Y] = tp->pos[Y] - tp->vec[Y];

	if(tp->vec[X] < 0.0f){
		sshot_rot -= SSHOT_ROT_RATE; 
		if(sshot_rot < -SSHOT_ROT) sshot_rot = -SSHOT_ROT;
	}else if(tp->vec[X] > 0.0f){
		sshot_rot += SSHOT_ROT_RATE; 
		if(sshot_rot > +SSHOT_ROT) sshot_rot = +SSHOT_ROT;
	}else{
		if(sshot_rot < 0.0f){
			sshot_rot += SSHOT_ROT_RATE * 1.5f;
			if(sshot_rot > 0.0f) sshot_rot = 0.0f;
		}else if(sshot_rot > 0.0f){
			sshot_rot -= SSHOT_ROT_RATE * 1.5f;
			if(sshot_rot < 0.0f) sshot_rot = 0.0f;
		}
	}

	if(tp->pos[X] < SHIP_MINX) tp->pos[X] = SHIP_MINX;
	if(tp->pos[X] > SHIP_MAXX) tp->pos[X] = SHIP_MAXX;
	if(tp->pos[Y] < SHIP_MINY) tp->pos[Y] = SHIP_MINY;
	if(tp->pos[Y] > SHIP_MAXY) tp->pos[Y] = SHIP_MAXY;
}

void tsk_ship_shot(TSK *tp)
{
	if(nuclear_exec == SDL_TRUE) return;

	if((shippad_dat & PAD_BUTTON1) == 0){
		sshot_first = 10;
		sshot_wait = 0;
		return;
	}

	if(sshot_wait){
		sshot_wait--;
		return;
	}

	SDL_wheel_PlaySe(SE_SSHOT);

	if(sshot_first != 0){
		tsk_ship_shot_fire(tp, 0.0f, -44.0f, 0.0f, -20.0f, 0.0f, SDL_TRUE);
		sshot_first--;
	}else{
		tsk_ship_shot_fire(opt_l, -10.0f, -34.0f, -4.0f, -14.0f, -PI / 12.5f + sshot_rot, SDL_FALSE);
		tsk_ship_shot_fire(opt_l, -10.0f, -34.0f, -4.0f, -14.0f, -PI /  5.0f + sshot_rot, SDL_FALSE);
		tsk_ship_shot_fire(tp,      0.0f, -44.0f,  0.0f, -20.0f,       +0.0f + sshot_rot, SDL_FALSE);
		tsk_ship_shot_fire(opt_r, +10.0f, -34.0f, +4.0f, -14.0f, +PI /  5.0f + sshot_rot, SDL_FALSE);
		tsk_ship_shot_fire(opt_r, +10.0f, -34.0f, +4.0f, -14.0f, +PI / 12.5f + sshot_rot, SDL_FALSE);
	}

	sshot_wait = 2;
}

void tsk_ship_shot_fire(TSK *tp, float x, float y, float mx, float my, float dir, SDL_bool f)
{
	TSK *etp;

	etp = TSKsetTopName(tsk_sshot, GROUP_SSHOT, NAME_SSHOT);
	if(etp != NULL){
		etp->pos[X] = tp->pos[X] + x;
		etp->pos[Y] = tp->pos[Y] + y;
		etp->rot[X] = (PI * 2.0f / 8.0f) * dir;
		etp->rot[Y] = (PI * 2.0f / 8.0f) * dir;
		if(f == SDL_TRUE) etp->mode = 1;
		else			  etp->mode = 0;
		TSKexecDirect(etp);
		sshot_wait = 1;
		etp = TSKsetBtmName(tsk_muzzle, GROUP_SHIP, NAME_EFCT);
		if(etp != NULL){
			etp->tsk_parent = tp;
			etp->ofs[X] = mx;
			etp->ofs[Y] = my;
			TSKexecDirect(etp);
		}
	}
}

void tsk_ship_nuclear(TSK *tp)
{
	TSK *etp;
	TSK *eetp;
	TSK *stp = NULL;
	int num;

	if(nuclear_exec == SDL_FALSE){
		if((total_bonus != 0 || bonus_score != 0) && TSKgetGroupName(GROUP_SHIP, NAME_NSHOT) == NULL){
			bonus_score = 0;
			total_bonus = 0;
			return;
		}
		if(nuclear_power < NUCLEAR_RATE) return;
		if((shippad_trg & PAD_BUTTON2) == 0){
			return;
		}
		if(nuclear_power <= (NUCLEAR_MAX / 3.0f))			  nshot_level = 1;
		else if(nuclear_power <= (NUCLEAR_MAX / 3.0f * 2.0f)) nshot_level = 2;
		else 												  nshot_level = 3;
		nuclear_exec = SDL_TRUE;
	}

	nuclear_power -= nuclear_cost;

	if(nuclear_power <= 0.0f){
		nshot_level = 0;
		nuclear_power = 0.0f;
		nuclear_exec = SDL_FALSE;
		return;
	}

	if(nshot_wait){
		nshot_wait--;
		return;
	}

	if((shippad_dat & PAD_BUTTON1) == 0) return;

	etp = TSKsetTopName(tsk_nshot, GROUP_NSHOT, NAME_NSHOT);
	if(etp != NULL){
		SDL_wheel_PlaySe(SE_NSHOT);
		num = rand() % 3;
		if(num == 0){
			stp = opt_l;
		}else if(num == 1){
			stp= tp;
		}else if(num == 2){
			stp = opt_r;
		}
		if(stp != NULL){
			etp->pos[X] = stp->pos[X] +  0.0f;
			if(nshot_level == 1)	  etp->pos[Y] = stp->pos[Y] - 26.0f;
			else if(nshot_level == 2) etp->pos[Y] = stp->pos[Y] - 30.0f;
			else 					  etp->pos[Y] = stp->pos[Y] - 34.0f;
			TSKexecDirect(etp);
			nshot_wait = 1;
			eetp = TSKsetBtmName(tsk_muzzle, GROUP_SHIP, NAME_EFCT);
			if(eetp != NULL){
				eetp->tsk_parent = stp;
				eetp->ofs[X] = etp->pos[X] - stp->pos[X];
				eetp->ofs[Y] = etp->pos[Y] - stp->pos[Y] + 10.0f;
				if(nshot_level == 1)	  eetp->ofs[Y] = etp->pos[Y] - stp->pos[Y] + 12.0f;
				else if(nshot_level == 2) eetp->ofs[Y] = etp->pos[Y] - stp->pos[Y] + 16.0f;
				else 					  eetp->ofs[Y] = etp->pos[Y] - stp->pos[Y] + 20.0f;
				TSKexecDirect(eetp);
			}
		}
	}
}

void tsk_ship_anim(TSK *tp)
{
	if(tp->pat_wait != 0){
		tp->pat_wait--;
		return;
	}else{
		tp->pat_wait = SHIP_PAT_WAIT;
	}

	if(shippad_dir & PAD_LEFT){
		if(tp->pat != 0) tp->pat--;
	}else if(shippad_dir & PAD_RIGHT){
		if(tp->pat != 4) tp->pat++;
	}else{
		if(tp->pat < 2) tp->pat++;
		if(tp->pat > 2) tp->pat--;
	}
}

void tsk_ship_check_gameover(TSK *tp)
{
}

void tsk_sbanner_on(void)
{
	sbanner_f = 1;
}

void tsk_sbanner_off(void)
{
	sbanner_f = 0;
}

void tsk_sbanner_kill(void)
{
	sbanner_f = -1;
}

void tsk_sbanner(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->exit_adr = tsk_sbanner_exit;
			tp->draw_adr = tsk_sbanner_draw;
			tp->pat = 0;
			tp->step = STEP_MAIN;
			tsk_sbanner_init(tp);
			break;

		case STEP_MAIN+0:
			if(sbanner_f == 1){
				tsk_sbanner_move(tp);
			}else if(sbanner_f == -1){
				tp->step = STEP_EXIT;
			}
			break;

		default:
			TSKclear(tp);
			break;
	}

	tp->cnt++;
}

void tsk_sbanner_exit(TSK *tp)
{
}

void tsk_sbanner_draw(TSK *tp)
{
	int r_mode;
	SDL_Rect *p;

	if(sbanner_f == 1){
		r_mode = SDL_wheel_SetRanderMode(RENDER_LEFTTOP);
		p = &anim_rect_banner[tp->pat + sbanner_num[0]];
		SDL_wheel_DispSurface(surface[SURFACE_SBANNER], (Sint16)tp->pos[X] -14.0f + cam[X], (Sint16)tp->pos[Y] + 12.0f + cam[Y], p->x, p->y, p->w, p->h);
		p = &anim_rect_banner[tp->pat + sbanner_num[1]];
		SDL_wheel_DispSurface(surface[SURFACE_SBANNER], (Sint16)tp->pos[X] + 6.0f + cam[X], (Sint16)tp->pos[Y] + 12.0f + cam[Y], p->x, p->y, p->w, p->h);
		SDL_wheel_SetRanderMode(r_mode);
	}
}

void tsk_sbanner_init(TSK *tp)
{
	sbanner_num[0] = 0;
	sbanner_num[1] = 0;
}

void tsk_sbanner_move(TSK *tp)
{
	TSK *etp = tp->tsk_parent;

	sbanner_num[0] = (rand() % 4);
	sbanner_num[1] = (rand() % 4);

	if(etp != NULL){
		tp->pos[X] = etp->pos[X];
		tp->pos[Y] = etp->pos[Y];
	}
}

