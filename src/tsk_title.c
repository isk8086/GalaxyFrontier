#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

#if DEBUG_MODE
#define TITLE_WAIT (60 * 15)
//#define TITLE_WAIT (60 * 30)
#else
#define TITLE_WAIT (60 * 30)
#endif

#define LOGO_ANIM_FRAME (20)

/* opening */
typedef struct tagOPENING OPENING;
struct tagOPENING{
	int start;
	int end;
};

SDL_bool logo_disp;
SDL_bool is_start;

static void tsk_title_draw(TSK *tp);
static void tsk_title_exit(TSK *tp);
static SDL_bool tsk_title_destroy(TSK *tp);

static int opening_num;
static SDL_bool opening_skip;

static Sint16 msg_rect[6][XY] = {
									{ 144,  16 },
									{ 264,  32 },
									{ 536,  48 },
									{ 608,  48 },
									{ 520,  48 },
									{ 552,  64 },
								};
static OPENING opndata[6] = {
								{    0,  210 },
								{  210,  435 },
								{  435,  780 },
								{  780, 1125 },
								{ 1125, 1435 },
								{ 1435, 1740 },
							};
static int msg_mode;
static float msg_alpha;

static float logo_pos[3][XY];
static float ship_roll;

void tsk_title(TSK *tp)
{
	if(tsk_title_destroy(tp) == SDL_TRUE) return;

	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_title_draw;
			tp->exit_adr = tsk_title_exit;
			logo_disp = SDL_FALSE;
			SDL_wheel_SetFade(FADE_OUT_BLACK, 1);
			tp->mode = -1;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			ship_roll = PI;
			tp->step = STEP_MAIN + 10;
			break;

		/* opening message */
		case STEP_MAIN+10:
			scroll_f = SDL_TRUE;
			set_star_rolling(256, 2.317215, PI / 91.12375f);
			tp->wait = 90;
			tp->step++;
			break;
		case STEP_MAIN+11:
			tp->wait--;
			if(tp->wait > 0) break;
			tp->step++;
		case STEP_MAIN+12:
			SDL_wheel_SetFade(FADE_IN_BLACK, 30);
			SDL_wheel_PlayMusic(BGM_OPENING, SDL_FALSE);
			tp->mode = 0;
			tp->cnt = 0;
			opening_num = 0;
			opening_skip = SDL_FALSE;
			msg_mode = 0;
			msg_alpha = 0.0f;
			tp->step++;
			break;
		case STEP_MAIN+13:
			if(tp->cnt < 60 || (trgs & PAD_BUTTON1) == 0){
				if(tp->cnt == opndata[opening_num].end){
					opening_num++;
				}
				if(tp->cnt == opndata[opening_num].start){
					msg_alpha = 0.0f;
					msg_mode = 0;
				}
				if(tp->cnt == (opndata[opening_num].end - 45)){
					msg_alpha = 1.0f;
					msg_mode = 1;
				}
				if(msg_mode == 0){
					msg_alpha += 1.0f / 30.0f;
				}else{
					msg_alpha -= 1.0f / 30.0f;
				}
				if(msg_alpha < 0.0f) msg_alpha = 0.0f;
				if(msg_alpha > 1.0f) msg_alpha = 1.0f;
				if(opening_num < 6) SDL_wheel_SetAlpha(surface[SURFACE_OPENING1 + opening_num], (int)(msg_alpha * 255.0f));
				tp->cnt++;
				if(opening_num < 6) break;
			}else{
				opening_skip = SDL_TRUE;
			}
			SDL_wheel_SetFade(FADE_OUT_BLACK, 45);
			tp->wait = 45;
			tp->step++;
			break;
		case STEP_MAIN+14:
			tp->wait--;
			if(tp->wait > 0) break;
			TSKclearGroupName(tp, GROUP_BG, NAME_STAR);
			if(opening_skip == SDL_FALSE){
				tp->mode = 1;
				tp->step = STEP_MAIN + 20;
			}else{
				tp->step = STEP_MAIN + 30;
			}
			break;

		/* title logo animation */
		case STEP_MAIN+20:
			set_star_vertical(256);
			SDL_wheel_SetFade(FADE_IN_BLACK, 30);
			logo_disp = SDL_TRUE;
			logo_pos[0][X] = (float)SCR_X + 320.0f;
			logo_pos[0][Y] = (float)SCR_HY;
			logo_pos[1][X] = -320.0f;
			logo_pos[1][Y] = (float)SCR_HY;
			logo_pos[2][X] = (float)SCR_X + 320.0f;
			logo_pos[2][Y] = -240.0f;
			tp->wait = 60;
			tp->step++;
			break;
		case STEP_MAIN+21:
			tp->wait--;
			if(tp->wait > 0) break;
			tp->wait = LOGO_ANIM_FRAME;
			tp->step++;
			break;
		case STEP_MAIN+22:
			logo_pos[0][X] += ((float)SCR_HX - (float)(SCR_X + 320.0f)) / (float)LOGO_ANIM_FRAME;
			tp->wait--;
			if(tp->wait > 0) break;
			SDL_wheel_PlaySe(SE_TITLE1);
			logo_pos[0][X] = (float)SCR_HX;
			tp->wait = LOGO_ANIM_FRAME;
			tp->step++;
			break;
		case STEP_MAIN+23:
			logo_pos[1][X] += ((float)SCR_HX + 320.0f) / (float)LOGO_ANIM_FRAME;
			tp->wait--;
			if(tp->wait > 0) break;
			SDL_wheel_PlaySe(SE_TITLE1);
			logo_pos[1][X] = (float)SCR_HX;
			tp->wait = LOGO_ANIM_FRAME;
			tp->step++;
			break;
		case STEP_MAIN+24:
			logo_pos[2][X] += ((float)SCR_HX - (float)(SCR_X + 320.0f)) / (float)LOGO_ANIM_FRAME;
			logo_pos[2][Y] += ((float)SCR_HY + 240.0f) / (float)LOGO_ANIM_FRAME;
			tp->wait--;
			if(tp->wait > 0) break;
			SDL_wheel_PlaySe(SE_TITLE2);
			logo_pos[2][X] = (float)SCR_HX;
			logo_pos[2][Y] = (float)SCR_HY;
			SDL_wheel_SetFade(FADE_IN_WHITE, 15);
			tp->wait = 15;
			tp->step++;
			break;
		case STEP_MAIN+25:
			tp->wait--;
			if(tp->wait > 0) break;
			tp->mode = 2;
			tp->step = STEP_MAIN + 32;
			break;

		/* title wait */
		case STEP_MAIN+30:
			set_star_vertical(256);
			SDL_wheel_SetFade(FADE_IN_BLACK, 30);
			logo_pos[0][X] = (float)SCR_HX;
			logo_pos[0][Y] = (float)SCR_HY;
			logo_pos[1][X] = (float)SCR_HX;
			logo_pos[1][Y] = (float)SCR_HY;
			logo_pos[2][X] = (float)SCR_HX;
			logo_pos[2][Y] = (float)SCR_HY;
			logo_disp = SDL_TRUE;
			tp->mode = 2;
			tp->wait = 30;
			tp->step++;
			break;
		case STEP_MAIN+31:
			tp->wait--;
			if(tp->wait > 0) break;
			tp->step++;
			break;
		case STEP_MAIN+32:
			tp->cnt = 0;
			tp->mode = 2;
			tp->step++;
			break;
		case STEP_MAIN+33:
			tp->cnt++;
			if(tp->cnt > TITLE_WAIT){
				tp->step = STEP_MAIN + 200;
				break;
			}else if(trgs & PAD_BUTTON1){
				SDL_wheel_PlaySe(SE_CORRECT);
				tp->mode = 4;
				tp->step = STEP_MAIN + 40;
				break;
			}
			break;

		/* main menu */
		case STEP_MAIN+40:
			TSKsetBtmName(tsk_menu, GROUP_INFO, NAME_MENU);
			tp->step++;
			break;
		case STEP_MAIN+41:
			if(TSKgetName(NAME_MENU) != NULL) break;
			if(is_start == SDL_FALSE){
				tp->step = STEP_MAIN + 32;
				break;
			}
			SDL_wheel_SetFadeMusic(1, 120);
			tp->step = STEP_MAIN + 100;
			break;

		/* to game start sequence */
		case STEP_MAIN+100:
			tp->cnt = 0;
			tp->step++;
			break;
		case STEP_MAIN+101:
//			tp->cnt++;
//			if(tp->cnt < 120) break;
			tp->step = STEP_EXIT;
			break;

		/* to demo start sequence */
		case STEP_MAIN+200:
			SDL_wheel_SetFade(FADE_OUT_BLACK, 60);
			if(SDL_wheel_IsPlayMusic() == SDL_TRUE) SDL_wheel_SetFadeMusic(1, 120);
			tp->mode = 3;
			tp->cnt = 0;
			tp->step++;
			break;
		case STEP_MAIN+201:
			tp->cnt++;
			if(tp->cnt < 120) break;
			TSKclearGroupName(tp, GROUP_BG, NAME_STAR);
			tp->mode = -1;
			logo_disp = SDL_FALSE;
			is_start = SDL_TRUE;
			demo_play = SDL_TRUE;
			SDL_wheel_SetFade(FADE_IN_BLACK, 1);
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclear(tp);
			break;
	}

	if(tp->step >= (STEP_MAIN + 30) && tp->step <= (STEP_MAIN + 1000)){
		ship_roll += PI / 150.0f;
		logo_pos[2][Y] = (float)(SCR_HY) + sin(ship_roll) * 8.0f;
	}
}

void tsk_title_draw(TSK *tp)
{

	int i;
	char *title_str;
	Sint16 x;

	if(tp->mode == 0){
		if(opening_num >= 0){
			SDL_wheel_DispSurface(surface[SURFACE_OPENING1 + opening_num], SCR_HX, SCR_HY, 0, 0, msg_rect[opening_num][X], msg_rect[opening_num][Y]);
		}
	}

	if(logo_disp == SDL_TRUE){
		SDL_wheel_DispSurface(surface[SURFACE_TITLE_BG], SCR_HX, SCR_HY, 0, 0, 640, 480);
		for(i = 2; i >= 0; i--){
			SDL_wheel_DispSurface(surface[SURFACE_TITLE_LOGO1 + i], (Sint16)logo_pos[i][X], (Sint16)logo_pos[i][Y], 0, 0, 640, 480);
		}
	}

	if(tp->mode >= 2){
		if(tp->mode == 2){
			if((tp->cnt & 0x20) != 0){
				title_str = "PRESS SHOT BUTTON";
				x = 320 - (SDL_wheel_GetWidthAscii_Bank(FONT_16x16, title_str) / 2);
				SDL_wheel_DispAscii_Bank(FONT_16x16, title_str, x, 288);
			}
		}
		if(logo_disp == SDL_TRUE){
			title_str = COPYRIGHT_NAME;
			x = 320 - (SDL_wheel_GetWidthAscii_Bank(FONT_16x16, title_str) / 2);
			SDL_wheel_DispAscii_Bank(FONT_16x16, title_str, x, 428);
			title_str = COOPERATION_NAME;
			SDL_wheel_DispAscii_Bank(FONT_16x16, title_str, x, 448);
			SDL_wheel_DispAscii_Bank(FONT_02, "- PROJECT RH3 -", 556, 180);
		}
	}

	if(tp->mode < 4) SDL_wheel_DrawFade();
}

void tsk_title_exit(TSK *tp)
{
}

SDL_bool tsk_title_destroy(TSK *tp)
{
	if((trgs & BTN_EXIT)){
		if(DEBUG_MENU == 0){
			is_exec = SDL_FALSE;
		}else{
			debug_mode = 1;
		}
		tp->step = STEP_EXIT;
		return SDL_TRUE;
	}

	return SDL_FALSE;
}

