#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "global.h"
#include "game.h"

static void tsk_result_draw(TSK *tp);
static void tsk_result_exit(TSK *tp);
static void tsk_result_init(TSK *tp);
static void tsk_result_move(TSK *tp);

static int get_maxwidth(void);

static float trg_x;
static float item_pos[16];
static int item_wait[16];

void tsk_result(TSK *tp)
{
	switch(tp->step){
		case STEP_INIT:
			tp->draw_adr = tsk_result_draw;
			tp->exit_adr = tsk_result_exit;
			tsk_result_init(tp);
			set_star_rolling(256, 2.317215, PI / 91.12375f);
			SDL_wheel_SetFade(FADE_IN_BLACK, 30);
			tp->cnt = 0;
			tp->step = STEP_MAIN;
			break;

		case STEP_MAIN+0:
			tp->cnt++;
			if(tp->cnt < 60) break;
			SDL_wheel_PlayMusic(BGM_RESULT, SDL_FALSE);
			tp->step++;
			break;
		case STEP_MAIN+1:
			tsk_result_move(tp);
			if(trgs & PAD_BUTTON1){
				SDL_wheel_SetFadeMusic(1, 60);
				SDL_wheel_SetFade(FADE_OUT_BLACK, 60);
				tp->cnt = 0;
				tp->step++;
			}
			break;
		case STEP_MAIN+2:
			tsk_result_move(tp);
			tp->cnt++;
			if(tp->cnt < 60) break;
			SDL_wheel_StopMusic();
			tp->step = STEP_EXIT;
			break;

		default:
			TSKclearGroupName(tp, GROUP_BG, NAME_STAR);
			TSKclear(tp);
			break;
	}
}

void tsk_result_draw(TSK *tp)
{
	char result_str[40];
	Sint16 y;
	int hour, min, sec, msec;

	strcpy(result_str, "==== RESULT INFORMATION ====");
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, 320 - (SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str) / 2), 48);

	y = 88;

	sprintf(result_str, "           HIGH SCORE : %d", backup.high_score);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 0], y);
	y += 18;
	sprintf(result_str, "         TOTAL DAMAGE : %d", backup.total_damage / 1000);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 1], y);
	y += 18;
	convert_time(backup.play_time, hour, min, sec, msec);
	sprintf(result_str, "            PLAY TIME : %02d:%02d:%02d:%02d", hour, min, sec, msec);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 2], y);
	y += 18;
	sprintf(result_str, "           PLAY COUNT : %d", backup.play_cnt);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 3], y);
	y += 18;
	sprintf(result_str, "          CLEAR COUNT : %d", backup.clear_cnt);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 3], y);
	y += 18;
	y += 18;

	convert_time(backup.live_total, hour, min, sec, msec);
	sprintf(result_str, "  TOTAL SURVIVAL TIME : %02d:%02d:%02d:%02d", hour, min, sec, msec);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 4], y);
	y += 18;
	convert_time(backup.live_short, hour, min, sec, msec);
	sprintf(result_str, "MINIMUM SURVIVAL TIME : %02d:%02d:%02d", min, sec, msec);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 5], y);
	y += 18;
	convert_time(backup.live_long, hour, min, sec, msec);
	sprintf(result_str, "MAXIMUM SURVIVAL TIME : %02d:%02d:%02d", min, sec, msec);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 6], y);
	y += 18;
	convert_time(backup.live_average, hour, min, sec, msec);
	sprintf(result_str, "AVERAGE SURVIVAL TIME : %02d:%02d:%02d", min, sec, msec);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 7], y);
	y += 18;
	y += 18;

	sprintf(result_str, "       TOTAL DISTANCE : %d", backup.move_total / 1000);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 8], y);
	y += 18;
	sprintf(result_str, "     MINIMUM DISTANCE : %d", backup.move_short / 1000);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[ 9], y);
	y += 18;
	sprintf(result_str, "     MAXIMUM DISTANCE : %d", backup.move_long / 1000);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[10], y);
	y += 18;
	sprintf(result_str, "     AVERAGE DISTANCE : %d", backup.move_average / 1000);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[11], y);
	y += 18;
	y += 18;

	sprintf(result_str, "    TOTAL DESTRUCTION : %d", backup.destroy_total);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[12], y);
	y += 18;
	sprintf(result_str, "  MINIMUM DESTRUCTION : %d", backup.destroy_short);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[13], y);
	y += 18;
	sprintf(result_str, "  MAXIMUM DESTRUCTION : %d", backup.destroy_long);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[14], y);
	y += 18;
	sprintf(result_str, "  AVERAGE DESTRUCTION : %d", backup.destroy_average);
	SDL_wheel_DispAscii_Bank(FONT_16x16, result_str, (Sint16)item_pos[15], y);
	y += 18;
}

void tsk_result_exit(TSK *tp)
{
}

void tsk_result_init(TSK *tp)
{
	int i;

	trg_x = (float)get_maxwidth();
	trg_x = 320.0f - (trg_x / 2.0f);	

	for(i = 0; i < 16; i++){
		item_pos[i] = 640.0f;
		item_wait[i] = i * 4;
	}
}

void tsk_result_move(TSK *tp)
{
	int i;

	for(i = 0; i < 16; i++){
		if(item_wait[i] != 0){
			item_wait[i]--;
		}else{
			item_pos[i] += (trg_x - item_pos[i]) / 10.0f;
		}
	}
}

int get_maxwidth(void)
{
	char result_str[40];
	int max = 0;
	int hour, min, sec, msec;

	sprintf(result_str, "           HIGH SCORE : %d", backup.high_score);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	sprintf(result_str, "         TOTAL DAMAGE : %d", backup.total_damage / 1000);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	convert_time(backup.play_time, hour, min, sec, msec);
	sprintf(result_str, "            PLAY TIME : %02d:%02d:%02d:%02d", hour, min, sec, msec);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	sprintf(result_str, "           PLAY COUNT : %d", backup.play_cnt);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);

	convert_time(backup.live_total, hour, min, sec, msec);
	sprintf(result_str, "  TOTAL SURVIVAL TIME : %02d:%02d:%02d:%02d", hour, min, sec, msec);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	convert_time(backup.live_short, hour, min, sec, msec);
	sprintf(result_str, "MINIMUM SURVIVAL TIME : %02d:%02d:%02d", min, sec, msec);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	convert_time(backup.live_long, hour, min, sec, msec);
	sprintf(result_str, "MAXIMUM SURVIVAL TIME : %02d:%02d:%02d", min, sec, msec);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	convert_time(backup.live_average, hour, min, sec, msec);
	sprintf(result_str, "AVERAGE SURVIVAL TIME : %02d:%02d:%02d", min, sec, msec);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);

	sprintf(result_str, "       TOTAL DISTANCE : %d", backup.move_total / 1000);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	sprintf(result_str, "     MINIMUM DISTANCE : %d", backup.move_short / 1000);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	sprintf(result_str, "     MAXIMUM DISTANCE : %d", backup.move_long / 1000);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	sprintf(result_str, "     AVERAGE DISTANCE : %d", backup.move_average / 1000);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	sprintf(result_str, "    TOTAL DESTRUCTION : %d", backup.destroy_total);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	sprintf(result_str, "  MINIMUM DESTRUCTION : %d", backup.destroy_short);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	sprintf(result_str, "  MAXIMUM DESTRUCTION : %d", backup.destroy_long);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);
	sprintf(result_str, "  AVERAGE DESTRUCTION : %d", backup.destroy_average);
	if(max < SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str)) max = SDL_wheel_GetWidthAscii_Bank(FONT_16x16, result_str);

	return max;
}
