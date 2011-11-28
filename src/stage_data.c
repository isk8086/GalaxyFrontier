#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "global.h"
#include "game.h"

static SEQUENCE *stg_data[] = {
	stage00,
	stage01,
	stage02,
	stage03,

	stage99,

	NULL,
};

static int seq_p;
static SDL_bool seq_compiled;
static SDL_bool seq_exec;
static int seq_wait;
static int seq_step;
static int seq_intr;

static LABEL *label = NULL;

static int enemy_serial = 0;

void STAGEinit(void)
{
	puts("sequence init.");
	stg_num = 0;
	seq_p = 0;
	seq_wait = 0;
	seq_step = 0;
	seq_intr = -1;
	seq_compiled = SDL_FALSE;
	seq_exec = SDL_FALSE;
	if(label != NULL) free(label);
	label = NULL;
	enemy_serial = 0;
}

void STAGEreset(void)
{
	puts("sequence reset.");
	seq_p = 0;
	seq_wait = 0;
	seq_step = 0;
	seq_intr = -1;
	seq_compiled = SDL_FALSE;
	seq_exec = SDL_FALSE;
	if(label != NULL) free(label);
	label = NULL;
	enemy_serial = 0;
}

void STAGEcompile(int num)
{
	SEQUENCE *stg_p;
	SEQUENCE *p;
	LABEL *tmp;
	int i, j, n;
	SDL_bool f;

	printf("sequence compile. stage-%d\n", num);

	stg_num = num;
	stg_p = stg_data[stg_num];

	// label detect;
	i = 0;
	n = 0;
	f = SDL_TRUE;
	while(f == SDL_TRUE){
		p = &stg_p[i];
		if(p == NULL){
			return;
		}
		switch(p->command){
			case CMD_LABEL:
				tmp = realloc(label, sizeof(LABEL) * (n + 1));
				if(tmp == NULL){
					if(label != NULL) free(label);
					puts("memory not enough phase-1.");
					return;
				}
				label = tmp;
				label[n].line = i;
				label[n].name = p->param_str1;
				n++;
				break;
			case CMD_TERMINATE:
				f = SDL_FALSE;
				break;
			default:
				break;
		}
		i++;
	}

	if(n != 0){
		// jump label set;
		i = 0;
		f = SDL_TRUE;
		while(f == SDL_TRUE){
			p = &stg_p[i];
			switch(p->command){
				case CMD_JUMP:
				case CMD_OPNBG_SKIP:
					for(j = 0; j < n; j++){
						if(strcmp(p->param_str1, label[j].name) == 0) break;
					}
					if(j < n){
						p->param_val1 = label[j].line + 1;
					}else{
						printf("sequence error : not jump label is [%s]\n", p->param_str1);
						if(label != NULL) free(label);
						puts("memory not enough phase-2.");
						return;
					}
					break;
				case CMD_TERMINATE:
					f = SDL_FALSE;
					break;
				default:
					break;
			}
			i++;
		}
	}

	seq_compiled = SDL_TRUE;
	puts("sequence compile complete.");
}

void STAGEexec(void)
{
	SEQUENCE *stg_p;
	SEQUENCE *p;
	TSK *tp;
	int n;
	int ene_num;

	play_time++;

	if(seq_p == 0) printf("sequence start. stage-%d\n", stg_num);

	if(seq_compiled == SDL_FALSE || seq_p == -1) return;

	if(seq_intr >= 0){
		if((trgs & SHIP_PAD_BTN) != 0x00){
			seq_p = seq_intr;
			seq_intr = -1;
			seq_wait = 0;
		}
	}

	if(seq_wait != 0){
		seq_wait--;
		return;
	}

	stg_p = stg_data[stg_num];

	seq_exec = SDL_TRUE;

	while(seq_exec == SDL_TRUE){
		p = &stg_p[seq_p];
		if(p == NULL){
			printf("sequence terminated. stage-%d\n", stg_num);
			seq_p = -1;
			return;
		}
		switch(p->command){
			case CMD_WAIT:
				seq_wait = p->param_val1;
				seq_p++;
				break;
			case CMD_LABEL:
				seq_p++;
				break;
			case CMD_JUMP:
				//printf("jump line is %d(%s)\n", p->param_val1, p->param_str1);
				seq_p = p->param_val1;
				break;

			case CMD_BGM_REQUEST:
				if(game_over == SDL_FALSE) SDL_wheel_PlayMusic(p->param_val1, SDL_FALSE);
				seq_p++;
				break;
			case CMD_BGM_FADE:
				if(game_over == SDL_FALSE) SDL_wheel_SetFadeMusic(1, p->param_val1);
				seq_p++;
				break;

			case CMD_SE_STOP:
				SDL_wheel_StopSeAll();
				seq_p++;
				break;

			case CMD_SURFACE_COLOR:
				SDL_wheel_FillSurface(surface[p->param_val1], 0, 0, SDL_wheel_GetSurfaceW(surface[p->param_val1]), SDL_wheel_GetSurfaceH(surface[p->param_val1]), SDL_wheel_MapRgba(p->param_val2, p->param_val3, p->param_val4, 255));
				seq_p++;
				break;

			case CMD_SET_SCENE:
				tp = NULL;
				switch(p->param_val1){
					case STG_01: tp = TSKsetTopName(tsk_scene01, GROUP_SYSTEM, NAME_SCEN); break;
					case STG_02: tp = TSKsetTopName(tsk_scene02, GROUP_SYSTEM, NAME_SCEN); break;
					case STG_03: break;
					default: break;
				}
				if(tp != NULL){
					TSKexecDirect(tp);
				}
				seq_p++;
				break;

			case CMD_SET_ENEMY:
				ene_num = p->param_val1;
				n = p->param_val2;
				if(n != 0){
					if(p->param_val4 == 0) p->param_val4 = n;
					if(ene_num == MIDD_01){
						tp = TSKsetTopName(enemy_adr[ene_num], GROUP_ENEMY, NAME_MIDD);
					}else if(ene_num == BOSS_01){
						tp = TSKsetBtmName(enemy_adr[ene_num], GROUP_ENEMY, NAME_BOSS);
					}else{
						tp = TSKsetTopName(enemy_adr[ene_num], GROUP_ENEMY, NAME_ZAKO);
					}
					if(tp != NULL){
						tp->serial = enemy_serial;
						enemy_serial++;
						TSKexecDirect(tp);
						if(p->param_val3 != 0){
							seq_wait = p->param_val3;
						}
					}
					p->param_val4--;
					if(p->param_val4 != 0) break;
				}
				enemy_serial = 0;
				seq_p++;
				break;
			case CMD_IS_MIDDLE:
				if(is_middle == SDL_FALSE){
					seq_p++;
				}
				seq_exec = SDL_FALSE;
				break;
			case CMD_IS_BOSS:
				if(is_boss == SDL_FALSE){
					seq_p++;
				}
				seq_exec = SDL_FALSE;
				break;

			case CMD_TIME_START:
				time_exec = SDL_TRUE;
				seq_p++;
				break;
			case CMD_TIME_STOP:
				time_exec = SDL_FALSE;
				seq_p++;
				break;

			case CMD_SCROLL_VEL:
				set_star_vel((float)p->param_val1 / 1000.0f, (float)p->param_val2 / 1000.0f, p->param_val3);
				seq_p++;
				break;
			case CMD_SCROLL_SWITCH:
				set_star_switch(p->param_val1);
				seq_p++;
				break;

			case CMD_WARP_SET:
				warp_wait = p->param_val1;
				seq_p++;
				break;
			case CMD_WARP_WAIT:
				if(warp_wait == p->param_val1){
					seq_p++;
				}
				seq_exec = SDL_FALSE;
				break;

			case CMD_BG00_SET:
				seq_p++;
				break;
			case CMD_BG01_SET:
				if(TSKgetGroupName(p->param_val1, p->param_val2) == NULL){
					tp = TSKsetBtmName(tsk_bg01, p->param_val1, p->param_val2);
					if(tp == NULL){
						seq_exec = SDL_FALSE;
						break;
					}
					TSKexecDirect(tp);
				}
				seq_p++;
				break;
			case CMD_BG02_SET:
				if(TSKgetGroupName(p->param_val1, p->param_val2) == NULL){
					tp = TSKsetBtmName(tsk_bg02, p->param_val1, p->param_val2);
					if(tp == NULL){
						seq_exec = SDL_FALSE;
						break;
					}
					TSKexecDirect(tp);
				}
				seq_p++;
				break;
			case CMD_BG02_ALPHA:
				set_bg02_alpha(p->param_val1, (float)p->param_val2 / 1000.0f);
				seq_p++;
				break;
			case CMD_BGMASK_SET:
				if(TSKgetGroupName(p->param_val1, p->param_val2) == NULL){
					tp = TSKsetBtmName(tsk_bgmask, p->param_val1, p->param_val2);
					if(tp == NULL){
						seq_exec = SDL_FALSE;
						break;
					}
					TSKexecDirect(tp);
				}
				seq_p++;
				break;
			case CMD_BGMASK_ALPHA:
				set_bgmask_alpha((float)p->param_val1 / 1000.0f);
				seq_p++;
				break;
			case CMD_BG_STEP:
				tp = TSKgetGroupName(p->param_val1, p->param_val2);
				if(tp != NULL){
					tp->step = p->param_val3;
				}else{
					seq_exec = SDL_FALSE;
				}
				seq_p++;
				break;

			case CMD_FADE:
				tp = TSKsetTopName(tsk_fade, p->param_val1, NAME_FADE);
				if(tp == NULL){
					seq_exec = SDL_FALSE;
					break;
				}
				tp->mode = p->param_val2;
				tp->wait = p->param_val3;
				seq_p++;
				break;

			case CMD_PLAYER_SIDE:
				enemy_get_playerside();
				seq_p++;
				break;
			case CMD_PLAYER_POS:
				if(ship_tsk == NULL){
					seq_exec = SDL_FALSE;
					break;
				}
				ship_tsk->pos[X] = (float)p->param_val1;
				ship_tsk->pos[Y] = (float)p->param_val2;
				seq_p++;
				break;
			case CMD_PLAYER_LOCK:
				player_lock = SDL_TRUE;
				seq_p++;
				break;
			case CMD_PLAYER_UNLOCK:
				player_lock = SDL_FALSE;
				seq_p++;
				break;
			case CMD_PLAYER_BNON:
				tsk_sbanner_on();
				seq_p++;
				break;
			case CMD_PLAYER_BNOFF:
				tsk_sbanner_off();
				seq_p++;
				break;
			case CMD_PLAYER_BNKILL:
				tsk_sbanner_kill();
				seq_p++;
				break;

			case CMD_OPNBG_SET:
				opn_mode = 0;
				tp = TSKsetBtmName(tsk_opnbg, p->param_val1, p->param_val2);
				if(tp == NULL){
					seq_exec = SDL_FALSE;
					break;
				}
				TSKexecDirect(tp);
				seq_p++;
				break;
			case CMD_OPNBG_KILL:
				TSKclearGroupName(NULL, p->param_val1, p->param_val2);
				seq_p++;
				break;
			case CMD_OPNBG_WAIT1:
				if(opn_mode != 1){
					seq_exec = SDL_FALSE;
					break;
				}
				seq_p++;
				break;
			case CMD_OPNBG_WAIT2:
				if(opn_mode != 2){
					seq_exec = SDL_FALSE;
					break;
				}
				seq_p++;
				break;
			case CMD_OPNBG_WAIT3:
				if(opn_mode != 3){
					seq_exec = SDL_FALSE;
					break;
				}
				seq_p++;
				break;
			case CMD_OPNBG_SKIP:
				if(p->param_val2 == 1){
					seq_intr = p->param_val1;
				}else{
					seq_intr = -1;
				}
				seq_p++;
				break;

			case CMD_SEQ_NEXT:
				if(seq_continue == SDL_TRUE){
					STAGEreset();
					STAGEcompile(p->param_val1);
					printf("next sequence. stage-%d\n", stg_num);
				}else{
					seq_p++;
				}
				break;

			case CMD_PRINT:
				if(p->param_str1 != NULL){
					puts(p->param_str1);
				}
				seq_p++;
				break;

			/* sequence end */
			default:
				printf("sequence complete. stage-%d\n", stg_num);
				seq_p = -1;
				seq_exec = SDL_FALSE;
				break;
		}
		if(seq_wait != 0){
			seq_exec = SDL_FALSE;
		}
	}
}

SDL_bool STAGEisExec(void)
{
	return (seq_compiled == SDL_FALSE || seq_p == -1) ? SDL_FALSE : SDL_TRUE;
}
