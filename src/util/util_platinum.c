
#include <stdio.h>

#include "SDL.h"
#include "SDL_wheel.h"

#include "util_platinum.h"

#define FMF_INFO_DEBUG (0)

static Uint16 scr_x = SCREEN_X;
static Uint16 scr_y = SCREEN_Y;
static Uint16 scr_hx = SCREEN_X / 2;
static Uint16 scr_hy = SCREEN_Y / 2;

void PLATINUMinit(Uint16 sx, Uint16 sy, Uint16 hx, Uint16 hy)
{
	scr_x = sx;
	scr_y = sy;
	scr_hx = hx;
	scr_hy = hy;
}

FMFHeader *PLATINUMopen(const char *fname)
{
	FMFHeader *fmf = NULL;
	Uint32 size;

	if(!SDL_wheel_ExistFile(fname)) return NULL;

	size = SDL_wheel_GetFileSize(fname);
	if(size){
		fmf = malloc(size);
		if(fmf){
			SDL_wheel_ReadFile(fname, (void *)fmf, (size_t)size, 1);
			if(FMF_INFO_DEBUG){
				printf("dwIdentifier : %d\n", fmf->dwIdentifier);
				printf("dwSize       : %d\n", fmf->dwSize);
				printf("dwWidth      : %d\n", fmf->dwWidth);
				printf("dwHeight     : %d\n", fmf->dwHeight);
				printf("byChipWidth  : %d\n", fmf->byChipWidth);
				printf("byChipHeight : %d\n", fmf->byChipHeight);
				printf("byLayerCount : %d\n", fmf->byLayerCount);
				printf("byBitCount   : %d\n", fmf->byBitCount);
			}
		}
	}

	return fmf;
}

void PLATINUMclose(FMFHeader *fmf)
{
	if(fmf){
		free(fmf);
	}
}

SDL_bool PLATINUMmapDraw(FMFHeader *fmf, SDL_Surface *surface, float loc_x, float loc_y, int space)
{
	SDL_bool ret = SDL_FALSE;
	Uint32 r_mode;
	int layer;
	int l, x, y;
	Uint32 size;
	Uint32 mw, mh;
	float cw, ch;
	float mx, my;
	Sint16 sx, sy;
	Uint8 *map_p;
	Uint8 *p;

	if(!fmf || !surface) return SDL_FALSE;

	loc_x -= (float)(scr_hx + (fmf->dwWidth * fmf->byChipWidth / 2 - scr_hx));
	loc_y -= (float)(scr_hy + (fmf->dwHeight * fmf->byChipHeight / 2 - scr_hy));

	if(loc_x < (float)(-scr_x) || loc_x >= (float)(+scr_x)) return SDL_FALSE;
	if(loc_y < (float)(-scr_y) || loc_y >= (float)(+scr_y)) return SDL_FALSE;

	r_mode = SDL_wheel_SetRanderMode(RENDER_LEFTTOP);

	map_p = (Uint8 *)fmf + sizeof(FMFHeader);
	size = fmf->dwSize / 2;
	mw = fmf->dwWidth;
	mh = fmf->dwHeight;
	cw = (float)fmf->byChipWidth;
	ch = (float)fmf->byChipHeight;
	layer = fmf->byLayerCount;

	for(l = 0; l < layer; l++){
		my = loc_y;
		for(y = 0; y < mh; y++){
			if((my + ch) >= 0.0f && mh < (float)scr_y){
				mx = loc_x;
				for(x = 0; x < mw; x++){
					if((mx + cw) >= 0.0f && mx < (float)scr_x){
						p = map_p + x + (y * mw) + (l * size);
						if(*p != (Uint8)space){
							ret = SDL_TRUE;
							sx = ((Sint16)(*p) % CHIP_WCOUNT) * (Sint16)cw;
							sy = ((Sint16)(*p) / CHIP_WCOUNT) * (Sint16)ch;
							SDL_wheel_DispSurface(surface, mx, my, sx, sy, sx + (Sint16)cw, sy + (Sint16)ch);
						}
					}
					mx += cw;
				}
			}
			my += ch;
		}
	}

	SDL_wheel_SetRanderMode(r_mode);

	return ret;
}
