#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "global.h"
#include "game.h"
#include "debug/debug.h"

static void app_init(int argc, char *argv[]);
static void app_close(void);
static void app_exec(void);

SDL_bool is_exec;

static SDL_bool fullscreen = SDL_FALSE;

int main(int argc, char *argv[])
{
	app_init(argc, argv);

	while(1){
		SDL_wheel_pre();
		if(debug_mode == -1 || is_exec == SDL_FALSE) break;
		app_exec();
		SDL_wheel_post();
	}

	return 0;
}

void app_init(int argc, char *argv[])
{
	int i;

	for(i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'f':
				case 'F':
					fullscreen = SDL_TRUE;
					break;
				default:
					printf("invalid paramater is '%c'(%d)\n", argv[i][1], i);
					break;
			}
		}
	}

	atexit(&app_close);
 	game_init(fullscreen);
 	debug_mode = DEBUG_MENU;
}

void app_close(void)
{
	game_exit();
	TSKclearAll(NULL);
	SDL_wheel_CloseAscii();
	SDL_wheel_CloseAll();
}

void app_exec(void)
{
	game_exec();
	if(DEBUG_MENU != 0){
		debug_mode = debug_main(debug_mode);
	}
}

