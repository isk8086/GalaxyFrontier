#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"
#include "debug/debug.h"

static void tsk_gctrl_exit(TSK *tp);
static SDL_bool tsk_gctrl_destroy(TSK *tp);

void tsk_gctrl(TSK *tp)
{
	if(SDL_wheel_GetEvent()->type == SDL_QUIT){
		is_exec = SDL_FALSE;
		tp->step = STEP_EXIT;
		return;
	}

	switch(tp->step){
		case STEP_INIT:
			if(DEBUG_MODE == 0) pause_f = SDL_FALSE;
			else pause_f = SDL_TRUE;
			tp->exit_adr = tsk_gctrl_exit;
			tp->step = STEP_MAIN;
			break;

		/* title */
		case STEP_MAIN+0:
			demo_play = SDL_FALSE;
			game_over = SDL_FALSE;
			TSKsetTopName(tsk_title, GROUP_INFO, NAME_TITL);
			tp->step++;
			break;
		case STEP_MAIN+1:
			if(TSKgetName(NAME_TITL) != NULL) break;
			if(is_exec == SDL_FALSE || debug_mode == DEBUG_MENU || debug_mode == DEBUG_MENU_EXEC){
				tp->step = STEP_EXIT;
				break;
			}
			TSKclearGroupName(tp, GROUP_BG, NAME_STAR);
			SDL_wheel_StopMusic();
			if(demo_play == SDL_TRUE){
				tp->step = STEP_MAIN + 20;
				break;
			}
			tp->step = STEP_MAIN + 10;
			break;

		/* GAME PLAY */
		case STEP_MAIN+10:
			srand(19721020);
			tsk_gctrl_start(tp, STG_00);
			seq_continue = SDL_TRUE;
			tp->step++;
			break;
		case STEP_MAIN+11:
			if(game_over == SDL_TRUE){
				tp->step = STEP_MAIN + 100;
			}else if(STAGEisExec() == SDL_TRUE){
				if(time_exec == SDL_TRUE) game_time++;
				STAGEexec();
			}else{
				if(game_mode == GAMEMODE_SUPERHARD){
					backup.clear_cnt++;
				}
				tp->cnt = 300;
				SDL_wheel_SetFadeMusic(1, 120);
				TSKsetBtmName(tsk_gameclear, GROUP_INFO, NAME_INFO);
				tp->step = STEP_MAIN + 200;
			}
			if(tsk_gctrl_destroy(tp) == SDL_TRUE) return;
#if DEBUG_MODE
			if(trgs & PAD_BUTTON4){
				tsk_gctrl_release(tp);
				break;
			}
#endif	/* DEBUG_MODE */
			break;

		/* DEMO PLAY */
		case STEP_MAIN+20:
			tsk_gctrl_start(tp, STG_01);
			seq_continue = SDL_FALSE;
			tp->step++;
			break;
		case STEP_MAIN+21:
#if DEBUG_MODE
			if((trgs & PAD_BUTTON1) != 0 || game_over == SDL_TRUE || STAGEisExec() == SDL_FALSE){
#else
			if((trgs & PAD_BUTTON1) != 0 || game_over == SDL_TRUE || play_time >= (60 * 60)){
#endif
				tsk_gctrl_release(tp);
				break;
			}else{
				if(time_exec == SDL_TRUE) game_time++;
				STAGEexec();
			}
			if(tsk_gctrl_destroy(tp) == SDL_TRUE) return;
#if DEBUG_MODE
			if(trgs & PAD_BUTTON4){
				tsk_gctrl_release(tp);
				break;
			}
#endif	/* DEBUG_MODE */
			break;

		/* game over */
		case STEP_MAIN+100:
			STAGEexec();
			SDL_wheel_SetFadeMusic(1, 60);
			tp->cnt = 0;
			tp->step++;
			break;
		case STEP_MAIN+101:
			STAGEexec();
			tp->cnt++;
			if(tp->cnt < 120) break;
			TSKsetBtmName(tsk_gameover, GROUP_INFO, NAME_INFO);
			SDL_wheel_PlayMusic(BGM_GAMEOVER, SDL_FALSE);
			tp->step++;
			break;
		case STEP_MAIN+102:
			STAGEexec();
			if(trgs & PAD_BUTTON1){
				tp->step++;
			}
			if(SDL_wheel_IsPlayMusic() == SDL_TRUE) break;
			if(tsk_gctrl_destroy(tp) == SDL_TRUE) return;
			tp->step++;
			break;
		case STEP_MAIN+103:
			tsk_gctrl_release(tp);
			game_backup_update();
			break;

		/* game clear */
		case STEP_MAIN+200:
			play_time++;
			tp->cnt--;
			if(trgs & PAD_BUTTON1){
				SDL_wheel_StopMusic();
				tp->cnt = 0;
			}
			if(tp->cnt > 0) break;
			tsk_gctrl_release(tp);
			game_backup_update();
			break;

		default:
			enemy_release_core(tp);
			TSKclear(tp);
			break;
	}
}

void tsk_gctrl_exit(TSK *tp)
{
	TSKclearAll(tp);
	SDL_wheel_StopSeAll();
	SDL_wheel_StopMusic();
}

void tsk_gctrl_start(TSK *tp, int num)
{
	set_star_vertical(256);
	TSKsetBtmName(tsk_bg, GROUP_BG, NAME_BGSC);
	TSKsetTopName(tsk_info, GROUP_INFO, NAME_INFO);
	TSKsetTopName(tsk_ship, GROUP_SHIP, NAME_SHIP);
	enemy_reset();
	STAGEinit();
	STAGEcompile(num);
	STAGEexec();
	pause_f = SDL_TRUE;
	game_level = 0;
	opn_mode = 0;
	play_time = 0;
	game_time = 0;
	time_exec = SDL_FALSE;
	is_boss = SDL_FALSE;
	is_middle = SDL_FALSE;
	score = 0;
	nuclear_exec = SDL_FALSE;
	disp_score = score;
	bonus_score = 0;
	total_bonus = 0;
	total_damage = 0;
	move_total = 0;
	destroy_total = 0;
	cam[X] = 0;
	cam[Y] = 0;
	scroll[X] = 0.0f;
	scroll[Y] = 0.0f;
	scroll_f = SDL_TRUE;
	warp_wait = 0;
	eshot_vel = ESHOT_VEL_BASE;
	player_side = 0;
}

void tsk_gctrl_release(TSK *tp)
{
	TSKclearGroupName(tp, GROUP_SYSTEM, NAME_SCEN);
	TSKclearGroupName(tp, GROUP_BG, NAME_STAR);
	TSKclearGroupName(tp, GROUP_BG, NAME_BGSC);
	TSKclearGroupName(tp, GROUP_BG2, NAME_BGOPN);
	TSKclearGroupName(tp, GROUP_BG, NAME_BGEAR);
	TSKclearGroupName(tp, GROUP_BG, NAME_BGJUP);
	TSKclearGroupName(tp, GROUP_BG, NAME_BGWRP);
	TSKclearGroupName(tp, GROUP_BG, NAME_BGMSK);
	TSKclearGroupName(tp, GROUP_INFO, NAME_INFO);
	TSKclearGroupName(tp, GROUP_SHIP, NAME_SHIP);
	TSKclearGroupName(tp, GROUP_SHIP, NAME_SBNR);
	TSKclearGroupName(tp, GROUP_SHIP, NAME_SOPT);
	TSKclearGroupName(tp, GROUP_SHIP, NAME_EFCT);
	TSKclearGroupName(tp, GROUP_SSHOT, NAME_SSHOT);
	TSKclearGroupName(tp, GROUP_NSHOT, NAME_NSHOT);
	TSKclearGroupName(tp, GROUP_ENEMY, NAME_ZAKO);
	TSKclearGroupName(tp, GROUP_ENEMY, NAME_MIDD);
	TSKclearGroupName(tp, GROUP_ENEMY, NAME_BOSS);
	TSKclearGroupName(tp, GROUP_ENEMY, NAME_CORE);
	TSKclearGroupName(tp, GROUP_ESHOT, NAME_ESHOT);
	TSKclearGroupName(tp, GROUP_EFFECT, NAME_EFCT);
	TSKclearGroupName(tp, GROUP_EFFECT, NAME_QUAK);
	SDL_wheel_StopSeAll();
	SDL_wheel_StopMusic();
	pause = SDL_FALSE;
	if(tp != NULL) tp->step = STEP_MAIN + 0;
}

SDL_bool tsk_gctrl_destroy(TSK *tp)
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
