#ifndef	__UTIL__PLATINUM__
#define	__UTIL__PLATINUM__

#include "SDL.h"

#include "util.h"

/* FMF header (20 bytes) */
typedef struct tagFMFHeader FMFHeader;
struct tagFMFHeader
{
	Uint32 dwIdentifier;	/* identifer 'FMF__' */
	Uint32 dwSize;			/* map data saize */
	Uint32 dwWidth;			/* map width */
	Uint32 dwHeight;		/* map height */
	Uint8 byChipWidth;		/* chip width(pixels) */
	Uint8 byChipHeight;		/* chip height(pixels)*/
	Uint8 byLayerCount;		/* layer num */
	Uint8 byBitCount;		/* layer bit count */
};

#define CHIP_WCOUNT (16)	/* chip count */

extern void PLATINUMinit(Uint16 sx, Uint16 sy, Uint16 hx, Uint16 hy);
extern FMFHeader *PLATINMopen(const char *fname);
extern void PLATINMclose(FMFHeader *fmf);
extern SDL_bool PLATINMmapDraw(FMFHeader *fmf, SDL_Surface *surface, float loc_x, float loc_y, int space);

#endif	/* __UTIL__PLATINUM__ */
