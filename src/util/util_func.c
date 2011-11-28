
#include "SDL.h"
#include "SDL_wheel.h"

void FUNCinitTbl(int *tbl, int size, int start)
{
	int i;

	for(i = 0; i < size; i++){
		tbl[i] = start + i;
	}
}

void FUNCshuffleTbl(int *tbl, int max)
{
	int i, seed, old, tmp;

	old = seed = 0;
	for(i = max - 1; i > 1; i--){
		seed = rand() % i;
		old = seed;
		tmp = tbl[i];
		tbl[i] = tbl[seed];
		tbl[seed] = tmp;
	}
}

float FUNCrandFloat(float rnd)
{
	return (float)(rand() % 10000) / 10000.0f * rnd;
}
