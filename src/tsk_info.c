#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

#define BONUS_INIT_POS (-(16.0f * 2.0f))
#define BONUS_INIT_TRG (16.0f)

static void tsk_info_draw(TSK *tp);
static void tsk_info_exit(TSK *tp);
static void tsk_info_score(TSK *tp);
static void tsk_bonus_draw(TSK *tp);
static void tsk_bonus_exit(TSK *tp);
static void tsk_scene_draw(TSK *tp);
static void tsk_scene_exit(TSK *tp);
static void tsk_gameover_draw(TSK *tp);
static void tsk_gameover_exit(TSK *tp);
static void tsk_gameclear_draw(TSK *tp);
static void tsk_gameclear_exit(TSK *tp);

static char scene_str[10];

static int disp_bonus;
static int max_bonus;
static int info_frame;
static float bonus_pos;
static float bonus_trg;
static SDL_bool old_nuclear;

void tsk_info(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_info_draw;
			tp->exit_adr = tsk_info_exit;
			tp->cnt = 0;
			tp->wait = 0;
			info_frame = 0;
			bonus_trg = BONUS_INIT_POS;
			bonus_pos = bonus_trg;
			disp_bonus = 0;
			max_bonus = 0;
			old_nuclear = SDL_FALSE;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tsk_info_score(tp);
			info_frame++;
			if(nuclear_exec == SDL_FALSE){
				if(old_nuclear != nuclear_exec && old_nuclear == SDL_TRUE){
					bonus_pos = BONUS_INIT_TRG;
					bonus_trg = BONUS_INIT_TRG;
					tp->wait = 120;
				}
				disp_bonus = total_bonus != 0 ? total_bonus : disp_bonus;
				max_bonus = bonus_score != 0 ? bonus_score : max_bonus;
				if(tp->wait != 0){
					bonus_pos += (bonus_trg - bonus_pos) / 5.0f;
					tp->wait--;
					if(tp->wait == 0){
						disp_bonus = 0;
						max_bonus = 0;
					}
				}
			}else{
				bonus_pos += (bonus_trg - bonus_pos) / 5.0f;
				if(old_nuclear != nuclear_exec && old_nuclear == SDL_FALSE) tp->wait = 0;
				if(disp_bonus < total_bonus){
					disp_bonus = total_bonus;
				}
				if(max_bonus < bonus_score){
					bonus_pos = BONUS_INIT_POS;
					bonus_trg = BONUS_INIT_TRG;
					max_bonus = bonus_score;
				}
			}
			old_nuclear = nuclear_exec;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_info_draw(TSK *tp)
{
	int hour, min, sec, msec;
	char disp_str[20];
	int x = 16, y = 16;
	Uint32 mode;
	SDL_bool disp = SDL_TRUE;

	sprintf(disp_str, "SC %d", disp_score);
	SDL_wheel_DispAscii_Bank(FONT_16x16, disp_str, x + cam[X], y + cam[Y]);
	y += 18;

	mode = SDL_wheel_SetRanderMode(RENDER_LEFTTOP);
	SDL_wheel_DispSurface(surface[SURFACE_SYSTEM], x + cam[X], y + cam[Y], 0, 0, 136, 40);
	SDL_wheel_DispSurface(surface[SURFACE_SYSTEM], x + 32 + cam[X], y + 4 + cam[Y], 0, 40, (Sint16)(96 * nuclear_power), 52);
	if(nuclear_power == NUCLEAR_MAX){
		if((info_frame & 0x01) == 0) SDL_wheel_DispSurface(surface[SURFACE_SYSTEM], x + 32 + cam[X], y + 4 + cam[Y], 0, 56, 96, 68);
	}

	if(tp->wait != 0){
		if((info_frame & 0x01) == 0){
			disp = SDL_FALSE;
		}
	}

	if(disp == SDL_TRUE){
		sprintf(disp_str, "%6d", disp_bonus);
		SDL_wheel_DispAscii_Bank(FONT_16x16, disp_str, x + 8 + cam[X], y + 16 + cam[Y]);
		if(max_bonus != 0){
			SDL_wheel_DispSurface(surface[SURFACE_SYSTEM], x + 84 + cam[X], y + 44 + cam[Y], 0, 72, 48, 92);
			sprintf(disp_str, "%5d", max_bonus);
			SDL_wheel_DispAscii_Bank(FONT_16x16, disp_str, (Sint16)bonus_pos + cam[X], y + 48 + cam[Y]);
		}
	}

	SDL_wheel_SetRanderMode(mode);
	convert_time(game_time, hour, min, sec, msec);
	sprintf(disp_str, "%02d:%02d:%02d", min, sec, msec);
	SDL_wheel_DispAscii_Bank(FONT_16x16, disp_str, SCR_X - SDL_wheel_GetWidthAscii_Bank(FONT_16x16, disp_str) - 16 - cam[X], 16 - cam[Y]);

	if(demo_play == SDL_TRUE){
		if((play_time & 0x20) != 0){
			if((play_time % 0x20) == 0) tp->cnt++;
			if((tp->cnt & 0x01) == 1){
				strcpy(disp_str, "DEMO PLAY");
				x = 320 - SDL_wheel_GetWidthAscii_Bank(FONT_16x16, disp_str) / 2;
				SDL_wheel_DispAscii_Bank(FONT_16x16, disp_str, x, 192);
			}else{
				strcpy(disp_str, "PRESS SHOT BUTTON");
				x = 320 - SDL_wheel_GetWidthAscii_Bank(FONT_16x16, disp_str) / 2;
				SDL_wheel_DispAscii_Bank(FONT_16x16, disp_str, x, 192);
			}
		}
	}
}

void tsk_info_exit(TSK *tp)
{
}

void tsk_info_score(TSK *tp)
{
	int calc_score;

	if(score == disp_score) return;

	if((score - disp_score) > 50000){
		calc_score = 10000;
	}else{
		calc_score = 1000;
	}

	while(calc_score > 0){
		if((score - disp_score) > calc_score){
			disp_score += calc_score;
			return;
		}
		calc_score /= 10;
	}

	if((score - disp_score) == 1) disp_score = score;
}

void tsk_bonus(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_bonus_draw;
			tp->exit_adr = tsk_bonus_exit;
			tp->cnt = 0;
			tp->ftmp = 0.0f;
			tp->acc[Y] = 0.0f;
			tp->wait = 120;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tp->wait >= (120 - 30)) tp->ftmp += (float)tp->num / 30.0f;
			else tp->ftmp = (float)tp->num;
			tp->cnt++;
			tp->pos[Y] += 1.0f;
			tp->pos[Y] += tp->acc[Y];
			if(tp->wait >= (120 - 60)) tp->acc[Y] += 0.0237125f;
			else tp->acc[Y] += 0.13271325f;
			if(tp->wait){
				tp->wait--;
				break;
			}
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_bonus_draw(TSK *tp)
{
	char disp_str[10];
	int x;

	if((tp->cnt & 0x01) == 0){
		sprintf(disp_str, "+%d", (int)tp->ftmp);
		x = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, disp_str) / 2;
		SDL_wheel_DispAscii_Bank(FONT_16x16, disp_str, (Sint16)tp->pos[X] - x, (Sint16)tp->pos[Y] - 8);
	}
}

void tsk_bonus_exit(TSK *tp)
{
}

void tsk_scene(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_scene_draw;
			tp->exit_adr = tsk_scene_exit;
			sprintf(scene_str, "SCENE %d", scene);
			tp->pos[X] = 640.0f;
			tp->pos[Y] = 640.0f;
			tp->trg[X] = 320.0f - (float)(SDL_wheel_GetWidthAscii_Bank(FONT_16x16, scene_str) / 2.0f);
			tp->trg[Y] = 320.0f - (float)(SDL_wheel_GetWidthAscii_Bank(FONT_16x16, "START!") / 2.0f);
			tp->vel[X] = 0.0f;
			tp->vel[Y] = 0.0f;
			tp->acc[X] = 0.0f;
			tp->acc[Y] = 0.0f;
			tp->cnt = 0;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			if(tp->cnt < 30){
				tp->pos[X] += (tp->trg[X] - tp->pos[X]) / 15;
			}else{
				tp->pos[X] += (tp->trg[X] - tp->pos[X]) / 15;
				tp->pos[Y] += (tp->trg[Y] - tp->pos[Y]) / 15;
			}
			tp->cnt++;
			if(tp->cnt < 90) break;
			tp->cnt = 0;
			tp->step++;
			break;
		case STEP_MAIN+1:
			if(tp->cnt < 10){
				tp->pos[X] -= tp->vel[X];
				tp->vel[X] += tp->acc[X];
				tp->acc[X] += 0.1f;
			}else{
				tp->pos[X] -= tp->vel[X];
				tp->vel[X] += tp->acc[X];
				tp->acc[X] += 0.1f;
				tp->pos[Y] -= tp->vel[Y];
				tp->vel[Y] += tp->acc[Y];
				tp->acc[Y] += 0.1f;
			}
			tp->cnt++;
			if(tp->cnt < 60) break;
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_scene_draw(TSK *tp)
{
	SDL_wheel_DispAscii_Bank(FONT_16x16, scene_str, (Sint16)tp->pos[X], 240 - 10);
	SDL_wheel_DispAscii_Bank(FONT_16x16, "START!", (Sint16)tp->pos[Y], 240 + 10);
}

void tsk_scene_exit(TSK *tp)
{
}

void tsk_gameover(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_gameover_draw;
			tp->exit_adr = tsk_gameover_exit;
			tp->pos[X] = 320.0f - (float)(SDL_wheel_GetWidthAscii_Bank(FONT_16x16, "GAME OVER!") / 2.0f);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_gameover_draw(TSK *tp)
{
	SDL_wheel_DispAscii_Bank(FONT_16x16, "GAME OVER!", (Sint16)tp->pos[X], 192);
}

void tsk_gameover_exit(TSK *tp)
{
}

void tsk_gameclear(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_gameclear_draw;
			tp->exit_adr = tsk_gameclear_exit;
			tp->pos[X] = 320.0f - (float)(SDL_wheel_GetWidthAscii_Bank(FONT_16x16, "MISSON COMPLETE!") / 2.0f);
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			break;

		default:
			TSKclear(tp);
			break;
	}
}

void tsk_gameclear_draw(TSK *tp)
{
	SDL_wheel_DispAscii_Bank(FONT_16x16, "MISSION COMPLETE!", (Sint16)tp->pos[X], 192);
}

void tsk_gameclear_exit(TSK *tp)
{
}

