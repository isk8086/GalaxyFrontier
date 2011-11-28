
#include <stdio.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "../game.h"

#include "util_menu.h"

char *value_int[] = {
	"INT",
};

char *value_float[] = {
	"FLOAT",
};

static int font_num;
static int fontg_num;

void MENUinit(int font_w, int font_g)
{
	font_num = font_w;
	fontg_num = font_g;
}

int MENUexec(MENUdata *menu, int *items, int items_max)
{
	int ret = 0;
	MENUdata *p = &menu[*items + 1];
	SDL_bool cur_move = SDL_FALSE;

	if(reps & PAD_UP){
		(*items)--;
		cur_move = SDL_TRUE;
	}else if(reps & PAD_DOWN){
		(*items)++;
		cur_move = SDL_TRUE;
	}

	if(cur_move == SDL_TRUE){
		if(*items < 0)				 *items = items_max  - 1;
		else if(*items >= items_max) *items = 0;
		return ret;
	}

	if(p->select == SDL_TRUE){
		if(p->min != -1){
			if(reps & PAD_LEFT){
				p->now -= p->add;
				cur_move = SDL_TRUE;
			}else if(reps & PAD_RIGHT){
				p->now += p->add;
				cur_move = SDL_TRUE;
			}
		}
		if(cur_move == SDL_TRUE){
			if(p->now < p->min)		   p->now = p->max  - 1;
			else if(p->now >= p->max) p->now = p->min;
			return ret;
		}
	}

	if(trgs & PAD_BUTTON1){
		if(p->select == SDL_TRUE){
			ret = 1;
		}
	}

	return ret;
}

void MENUdraw(MENUdata *menu, int items, int items_max)
{
	int i, n;
	char str[256];
	Sint16 x = 16;
	Sint16 y = 16;
	Sint16 cursor = y + 24 + (items * 16);

	for(i = 0; i < (items_max + 1); i++){
		if(i == 0){		/* caption */
			SDL_wheel_DispAscii_Bank(font_num, menu[i].caption, x, y);
			y += 24;
		}else{			/* items */
			if(menu[i].min != -1){
				n = menu[i].now;
				if(menu[i].item == value_int){
					sprintf(str, "%s %d", menu[i].caption, n);
				}else if(menu[i].item == value_float){
					sprintf(str, "%s %f", menu[i].caption, (float)n / 10000.0f);
				}else{
					sprintf(str, "%s %s", menu[i].caption, menu[i].item[n]);
				}
			}else{
				strcpy(str, menu[i].caption);
			}
			if(menu[i].select == SDL_TRUE){
				SDL_wheel_DispAscii_Bank(font_num, str, x, y);
			}else{
				SDL_wheel_DispAscii_Bank(fontg_num, str, x, y);
			}
			y += 16;
		}
	}

	SDL_wheel_DispAscii_Bank(font_num, "->", x, cursor);
}
