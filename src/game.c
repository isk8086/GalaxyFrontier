#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "global.h"
#include "game.h"

static void backup_init(void);

static clock_t clocks;
static int rand_seed;

void game_init(SDL_bool f)
{
	Uint32 video_mode = SDL_SWSURFACE;
	Uint32 pixcel;
	int i;

	/* surface init */
	for(i = FONT_INIT; i < FONT_MAX; i++){
		font[i] = NULL;
	}

	for(i = SURFACE_INIT; i < SURFACE_MAX; i++){
		surface[i] = NULL;
	}

	if(f == SDL_TRUE) video_mode |= SDL_FULLSCREEN;

	/* SDL init */
	SDL_wheel_InitAll(SCREEN_SX, SCREEN_SY, video_mode);
	SDL_WM_SetCaption(APP_NAME, NULL);

	/* random init */
	clocks = clock();
	rand_seed = (int)clocks;
	printf("random seed %d\n", rand_seed);
	srand(rand_seed);

	/* font init */
	font[FONT_01] = SDL_wheel_LoadBmp("./resource/font/font.bmp", SDL_TRUE);
	font[FONT_02] = SDL_wheel_LoadBmp("./resource/font/font4x8.bmp", SDL_TRUE);
	font[FONT_16x16] = SDL_wheel_LoadBmp("./resource/font/font16x16.bmp", SDL_TRUE);
	font[FONT_16x16G] = SDL_wheel_LoadBmp("./resource/font/font16x16_gray.bmp", SDL_TRUE);
	SDL_wheel_InitAscii(font[FONT_01], 16, 3, 0, 10, 14);
	SDL_wheel_InitAscii_Bank(FONT_02, font[FONT_02], 8, 0, 0, 4, 8);
	SDL_wheel_InitAscii_Bank(FONT_16x16, font[FONT_16x16], 16, 0, 0, 16, 16);
	SDL_wheel_InitAscii_Bank(FONT_16x16G, font[FONT_16x16G], 16, 0, 0, 16, 16);
	SDL_wheel_ResetFontSize(FONT_02, 4, 8);
	SDL_wheel_ResetFontSize(FONT_16x16, 16, 16);
	SDL_wheel_ResetFontSize(FONT_16x16G, 16, 16);

	/* star graphic init */
	surface[SURFACE_STAR] = SDL_wheel_CreateSurface(16, 16, SDL_SWSURFACE);
	SDL_wheel_FillSurface(surface[SURFACE_STAR],  0, 0,  2, 2, SDL_wheel_MapRgba(128,   0,   0, 255));
	SDL_wheel_FillSurface(surface[SURFACE_STAR],  2, 0,  4, 2, SDL_wheel_MapRgba(  0, 128,   0, 255));
	SDL_wheel_FillSurface(surface[SURFACE_STAR],  4, 0,  6, 2, SDL_wheel_MapRgba(  0,   0, 128, 255));
	SDL_wheel_FillSurface(surface[SURFACE_STAR],  6, 0,  8, 2, SDL_wheel_MapRgba(128, 128,   0, 255));
	SDL_wheel_FillSurface(surface[SURFACE_STAR],  8, 0, 10, 2, SDL_wheel_MapRgba(128,   0, 128, 255));
	SDL_wheel_FillSurface(surface[SURFACE_STAR], 10, 0, 12, 2, SDL_wheel_MapRgba(  0, 128, 128, 255));
	SDL_wheel_FillSurface(surface[SURFACE_STAR], 12, 0, 14, 2, SDL_wheel_MapRgba(128, 128, 128, 255));

	/* particle graphic init */
	surface[SURFACE_PARTICLE] = SDL_wheel_CreateSurface(16, 16, SDL_SWSURFACE);
	SDL_wheel_FillSurface(surface[SURFACE_PARTICLE],  0, 0,  4, 4, SDL_wheel_MapRgba(192,  96,  64, 255));
	SDL_wheel_FillSurface(surface[SURFACE_PARTICLE],  4, 0,  8, 4, SDL_wheel_MapRgba(192, 128,  64, 255));
	SDL_wheel_FillSurface(surface[SURFACE_PARTICLE],  8, 0, 12, 4, SDL_wheel_MapRgba(192, 160,  64, 255));

	/* screen mask graphic init */
	surface[SURFACE_MASK] = SDL_wheel_CreateSurface(SCR_X, SCR_Y, SDL_SWSURFACE);
	SDL_wheel_FillSurface(surface[SURFACE_MASK], 0, 0, SCR_X, SCR_Y, SDL_wheel_MapRgba(0, 0, 0, 255));

	/* other graphics load */
	surface[SURFACE_SYSTEM]      = SDL_wheel_LoadBmp("./resource/graphics/system.bmp", SDL_TRUE);
	surface[SURFACE_ENERGY]      = SDL_wheel_LoadBmp("./resource/graphics/energy.bmp", SDL_TRUE);
	surface[SURFACE_TITLE_BG]    = SDL_wheel_LoadBmp("./resource/graphics/title/title_bg1.bmp", SDL_TRUE);
	surface[SURFACE_TITLE_LOGO1] = SDL_wheel_LoadBmp("./resource/graphics/title/title_logo_1.bmp", SDL_TRUE);
	surface[SURFACE_TITLE_LOGO2] = SDL_wheel_LoadBmp("./resource/graphics/title/title_logo_2.bmp", SDL_TRUE);
	surface[SURFACE_TITLE_SHIP]  = SDL_wheel_LoadBmp("./resource/graphics/title/title_ziki.bmp", SDL_TRUE);
	surface[SURFACE_BG00]        = SDL_wheel_LoadBmp("./resource/graphics/bg/bg.bmp", SDL_TRUE);
	surface[SURFACE_BG01]        = SDL_wheel_LoadBmp("./resource/graphics/bg/bg01.bmp", SDL_TRUE);
	surface[SURFACE_BG02]        = SDL_wheel_LoadBmp("./resource/graphics/bg/bg02.bmp", SDL_TRUE);
	surface[SURFACE_WARP01]      = SDL_wheel_LoadBmp("./resource/graphics/bg/warp01.bmp", SDL_TRUE);
	surface[SURFACE_WARP02]      = SDL_wheel_LoadBmp("./resource/graphics/bg/warp02.bmp", SDL_TRUE);
	surface[SURFACE_WARP03]      = SDL_wheel_LoadBmp("./resource/graphics/bg/warp03.bmp", SDL_TRUE);
	surface[SURFACE_SHIP]        = SDL_wheel_LoadBmp("./resource/graphics/ship/ship.bmp", SDL_TRUE);
	surface[SURFACE_SBANNER]     = SDL_wheel_LoadBmp("./resource/graphics/ship/sbanner.bmp", SDL_TRUE);
	surface[SURFACE_SSHOT]       = SDL_wheel_LoadBmp("./resource/graphics/ship/sshot.bmp", SDL_TRUE);
	surface[SURFACE_SSHOT2]      = SDL_wheel_LoadBmp("./resource/graphics/ship/sshot2.bmp", SDL_TRUE);
	surface[SURFACE_SSHOT_M]     = SDL_wheel_LoadBmp("./resource/graphics/ship/shot_mazlle.bmp", SDL_TRUE);
	surface[SURFACE_SSHOT_T]     = SDL_wheel_LoadBmp("./resource/graphics/ship/shot_touch.bmp", SDL_TRUE);
	surface[SURFACE_NSHOT]       = SDL_wheel_LoadBmp("./resource/graphics/ship/nshot.bmp", SDL_TRUE);
	surface[SURFACE_NUCLEAR]     = SDL_wheel_LoadBmp("./resource/graphics/ship/nuclear.bmp", SDL_TRUE);
	surface[SURFACE_ESHOT]       = SDL_wheel_LoadBmp("./resource/graphics/enemy/eshot.bmp", SDL_TRUE);
	surface[SURFACE_ESHOT_S]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/eshot_s.bmp", SDL_TRUE);
	surface[SURFACE_ESHOT_M]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/eshot_m.bmp", SDL_TRUE);
	surface[SURFACE_ESHOT_L]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/eshot_l.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO01]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako01.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO02]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako02.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO03]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako03.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO04]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako04.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO05]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako05.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO06]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako06.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO07]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako07.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO08]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako08.bmp", SDL_TRUE);
	surface[SURFACE_MIDL01]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/mzako01.bmp", SDL_TRUE);
	surface[SURFACE_MIDL02]      = SDL_wheel_LoadBmp("./resource/graphics/enemy/mzako02.bmp", SDL_TRUE);
	surface[SURFACE_BOSS01]      = SDL_wheel_LoadBmp("./resource/graphics/boss01.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO01R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako01r.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO02R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako02r.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO03R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako03r.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO04R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako04r.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO05R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako05r.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO06R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako06r.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO07R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako07r.bmp", SDL_TRUE);
	surface[SURFACE_ZAKO08R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako08r.bmp", SDL_TRUE);
	surface[SURFACE_MIDL01R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/mzako01r.bmp", SDL_TRUE);
	surface[SURFACE_MIDL02R]     = SDL_wheel_LoadBmp("./resource/graphics/enemy/mzako02r.bmp", SDL_TRUE);
	surface[SURFACE_EXPLODE_S1]  = SDL_wheel_LoadBmp("./resource/graphics/effect/explode/exp_05.bmp", SDL_TRUE);
	surface[SURFACE_EXPLODE_S2]  = SDL_wheel_LoadBmp("./resource/graphics/effect/explode/exp_06.bmp", SDL_TRUE);
	surface[SURFACE_EXPLODE_L1]  = SDL_wheel_LoadBmp("./resource/graphics/effect/explode/exp_07.bmp", SDL_TRUE);
	surface[SURFACE_EXPLODE_L2]  = SDL_wheel_LoadBmp("./resource/graphics/effect/explode/exp_08.bmp", SDL_TRUE);
	surface[SURFACE_OPENING1]    = SDL_wheel_LoadBmp("./resource/graphics/opening/opening1.bmp", SDL_TRUE);
	surface[SURFACE_OPENING2]    = SDL_wheel_LoadBmp("./resource/graphics/opening/opening2.bmp", SDL_TRUE);
	surface[SURFACE_OPENING3]    = SDL_wheel_LoadBmp("./resource/graphics/opening/opening3.bmp", SDL_TRUE);
	surface[SURFACE_OPENING4]    = SDL_wheel_LoadBmp("./resource/graphics/opening/opening4.bmp", SDL_TRUE);
	surface[SURFACE_OPENING5]    = SDL_wheel_LoadBmp("./resource/graphics/opening/opening5.bmp", SDL_TRUE);
	surface[SURFACE_OPENING6]    = SDL_wheel_LoadBmp("./resource/graphics/opening/opening6.bmp", SDL_TRUE);
	surface[SURFACE_STARTDEMO1]  = SDL_wheel_LoadBmp("./resource/graphics/start_demo/start_demo01.bmp", SDL_TRUE);
	surface[SURFACE_STARTDEMO2]  = SDL_wheel_LoadBmp("./resource/graphics/start_demo/start_demo02.bmp", SDL_TRUE);
	surface[SURFACE_STARTDEMO3]  = SDL_wheel_LoadBmp("./resource/graphics/start_demo/start_demo03.bmp", SDL_TRUE);

	surface[SURFACE_ZAKOSTONE1]  = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako_stone_anime_01.bmp", SDL_TRUE);
	surface[SURFACE_ZAKOSTONE2]  = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako_stone_anime_02.bmp", SDL_TRUE);
	surface[SURFACE_ZAKOSTONE3]  = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako_stone_anime_03.bmp", SDL_TRUE);
	surface[SURFACE_ZAKOSTONE4]  = SDL_wheel_LoadBmp("./resource/graphics/enemy/szako_stone_anime_04.bmp", SDL_TRUE);
	surface[SURFACE_MIDDSTONE1]  = SDL_wheel_LoadBmp("./resource/graphics/enemy/mzako_stone_anime_01.bmp", SDL_TRUE);
	surface[SURFACE_MIDDSTONE2]  = SDL_wheel_LoadBmp("./resource/graphics/enemy/mzako_stone_anime_02.bmp", SDL_TRUE);
	surface[SURFACE_MIDDSTONE3]  = SDL_wheel_LoadBmp("./resource/graphics/enemy/mzako_stone_anime_03.bmp", SDL_TRUE);
	surface[SURFACE_MIDDSTONE4]  = SDL_wheel_LoadBmp("./resource/graphics/enemy/mzako_stone_anime_04.bmp", SDL_TRUE);

	surface[SURFACE_EFFBALL01]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p000_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL02]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p001_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL03]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p002_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL04]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p003_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL05]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p004_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL06]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p005_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL07]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p006_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL08]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p007_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL09]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p008_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL10]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p009_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL11]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p010_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL12]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p011_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL13]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p012_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL14]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p013_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL15]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p014_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL16]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p015_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL17]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p016_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL18]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p017_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL19]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p018_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL20]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p019_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL21]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p020_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL22]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p021_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL23]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p022_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL24]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p023_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL25]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p024_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL26]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p025_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL27]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p026_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL28]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p027_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL29]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p028_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL30]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p029_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL31]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p030_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL32]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p031_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL33]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p032_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL34]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p033_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL35]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p034_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL36]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p035_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL37]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p036_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL38]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p037_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL39]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p038_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL40]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p039_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL41]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p040_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL42]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p041_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL43]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p042_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL44]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p043_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL45]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p044_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL46]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p045_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL47]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p046_00.bmp", SDL_TRUE);
	surface[SURFACE_EFFBALL48]   = SDL_wheel_LoadBmp("./resource/graphics/ef_ball/ef_ball_p047_00.bmp", SDL_TRUE);

	pixcel = SDL_wheel_GetPixel(surface[SURFACE_TITLE_LOGO1], 0, 0);
	SDL_wheel_SetColorkey(surface[SURFACE_TITLE_BG], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_TITLE_LOGO1], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_TITLE_LOGO2], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_TITLE_SHIP], pixcel);

	pixcel = SDL_wheel_GetPixel(surface[SURFACE_WARP01], 0, 0);
	SDL_wheel_SetColorkey(surface[SURFACE_WARP01], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_WARP02], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_WARP03], pixcel);

	pixcel = SDL_wheel_GetPixel(surface[SURFACE_SSHOT_M], 0, 0);
	SDL_wheel_SetColorkey(surface[SURFACE_SSHOT_M], pixcel);

	pixcel = SDL_wheel_GetPixel(surface[SURFACE_SSHOT_T], 0, 0);
	SDL_wheel_SetColorkey(surface[SURFACE_SSHOT_T], pixcel);

	pixcel = SDL_wheel_GetPixel(surface[SURFACE_SSHOT_T], 0, 0);
	SDL_wheel_SetColorkey(surface[SURFACE_SSHOT_T], pixcel);

	pixcel = SDL_wheel_GetPixel(surface[SURFACE_STARTDEMO3], 0, 0);
	SDL_wheel_SetColorkey(surface[SURFACE_STARTDEMO1], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_STARTDEMO2], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_STARTDEMO3], pixcel);

	pixcel = SDL_wheel_GetPixel(surface[SURFACE_EFFBALL01], 0, 0);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL01], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL02], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL03], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL04], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL05], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL06], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL07], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL08], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL09], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL10], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL11], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL12], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL13], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL14], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL15], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL16], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL17], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL18], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL19], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL20], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL21], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL22], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL23], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL24], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL25], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL26], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL27], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL28], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL29], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL30], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL31], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL32], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL33], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL34], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL35], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL36], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL37], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL38], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL39], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL40], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL41], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL42], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL43], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL44], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL45], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL46], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL47], pixcel);
	SDL_wheel_SetColorkey(surface[SURFACE_EFFBALL48], pixcel);

	primary = SDL_wheel_GetPrimarySurface();
	SDL_wheel_InitFade();

	/* bgm int */
	SDL_wheel_LoadMusic("./resource/bgm/opening.ogg", BGM_OPENING, 1);
	SDL_wheel_LoadMusic("./resource/bgm/bgm01.ogg", BGM_STG01, 1);
	SDL_wheel_LoadMusic("./resource/bgm/bgm02.ogg", BGM_STG02, 1);
	SDL_wheel_LoadMusic("./resource/bgm/bgm03.ogg", BGM_STG03, 1);
	SDL_wheel_LoadMusic("./resource/bgm/boss.ogg", BGM_BOSS, 1);
	SDL_wheel_LoadMusic("./resource/bgm/boss_last.ogg", BGM_BOSS_LAST, 1);
	SDL_wheel_LoadMusic("./resource/bgm/clear.ogg", BGM_CLEAR, 1);
	SDL_wheel_LoadMusic("./resource/bgm/ending.ogg", BGM_ENDING, 1);
	SDL_wheel_LoadMusic("./resource/bgm/result.ogg", BGM_RESULT, 1);
	SDL_wheel_LoadMusic("./resource/bgm/gameover.ogg", BGM_GAMEOVER, 1);

	/* sfx init */
	SDL_wheel_LoadSe("./resource/sfx/title1.wav", SE_TITLE1, 0);
	SDL_wheel_LoadSe("./resource/sfx/title2.wav", SE_TITLE2, 0);
	SDL_wheel_LoadSe("./resource/sfx/cursor.wav", SE_CURSOR, 0);
	SDL_wheel_LoadSe("./resource/sfx/column.wav", SE_COLUMN, 0);
	SDL_wheel_LoadSe("./resource/sfx/correct.wav", SE_CORRECT, 0);
	SDL_wheel_LoadSe("./resource/sfx/cancel.wav", SE_CANCEL, 0);
	SDL_wheel_LoadSe("./resource/sfx/damage.wav", SE_DAMAGE, 1);
	SDL_wheel_LoadSe("./resource/sfx/explode_s.wav", SE_EXPLODE_S, 2);
	SDL_wheel_LoadSe("./resource/sfx/explode_m.wav", SE_EXPLODE_M, 3);
	SDL_wheel_LoadSe("./resource/sfx/explode_l.wav", SE_EXPLODE_L, 4);
	SDL_wheel_LoadSe("./resource/sfx/sshot.wav", SE_SSHOT, 5);
	SDL_wheel_LoadSe("./resource/sfx/nshot.wav", SE_NSHOT, 5);
	SDL_wheel_LoadSe("./resource/sfx/collide.wav", SE_COLLIDE, 6);
	SDL_wheel_LoadSe("./resource/sfx/sortie.wav", SE_SORTIE, 0);
	SDL_wheel_LoadSe("./resource/sfx/departure.wav", SE_DEPARTURE, 1);
	SDL_wheel_LoadSe("./resource/sfx/warp-in.wav", SE_WARP_IN, 0);
	SDL_wheel_LoadSe("./resource/sfx/ewarp-in.wav", SE_EWARP_IN, 0);

	/* voice init */
	SDL_wheel_LoadSe("./resource/voice/voice_roger.wav", VOICE_ROGER, 7);
	SDL_wheel_LoadSe("./resource/voice/voice_start.wav", VOICE_START, 7);
	SDL_wheel_LoadSe("./resource/voice/voice_earthout.wav", VOICE_EARTHOUT, 7);
	SDL_wheel_LoadSe("./resource/voice/voice_warpin.wav", VOICE_WARPIN, 7);
	SDL_wheel_LoadSe("./resource/voice/voice_warpstart1.wav", VOICE_WARPSTART1, 7);
	SDL_wheel_LoadSe("./resource/voice/voice_warpstart2.wav", VOICE_WARPSTART2, 7);

	/* key config */
	if(PANDORA_MODE == 0){
		SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON2, SDLK_x);
		SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON3, SDLK_c);
	}else{
		SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON1, SDLK_HOME);
		SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON2, SDLK_PAGEDOWN);
		SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON3, SDLK_END);
		SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON4, SDLK_PAGEUP);
	}
	SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON6, SDLK_q);
	SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON7, SDLK_o);
	SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON8, SDLK_p);
	SDL_wheel_SetKeyConf(WHPAD_PLAYER, WHPAD_BUTTON9, SDLK_ESCAPE);

	/* scroll init */
	cam[X] = 0;
	cam[Y] = 0;

	/* task init */
	TSKinit();
	if(DEBUG_MENU == 0){
		TSKsetTop(tsk_gctrl, GROUP_SYSTEM);
	}


	/* debug menu init */
	if(DEBUG_MENU != 0){
		MENUinit(FONT_16x16, FONT_16x16G);
	}

	/* others */
	is_exec = SDL_TRUE;
	pause_f = SDL_FALSE;
	pause = SDL_FALSE;
	pause_skip = SDL_FALSE;
	demo_play = SDL_FALSE;

	game_fillcolor_init();
	backup_init();

	/* game init */
	ship_tsk = NULL;
	player_side = 0;
	game_mode = config.game_mode;
}

void game_exit(void)
{
	int i;

	for(i = FONT_INIT; i < FONT_MAX; i++){
		if(font[i] != NULL) SDL_FreeSurface(font[i]);
	}

	for(i = SURFACE_INIT; i < SURFACE_MAX; i++){
		if(surface[i] != NULL) SDL_FreeSurface(surface[i]);
	}

	for(i = BGM_INIT; i < BGM_MAX; i++){
		SDL_wheel_FreeMusic(i);
	}

	for(i = SE_INIT; i < SE_MAX; i++){
		SDL_wheel_FreeSe(i);
	}
}

void game_exec(void)
{
	keys = SDL_wheel_Keys();
	pads = SDL_wheel_Pads(0);
	reps = SDL_wheel_Reps(0);
	trgs = SDL_wheel_Trgs(0);

	pause_skip = SDL_FALSE;

	if(pause_f == SDL_TRUE){
		if((trgs & BTN_PAUSE)){
			if(pause == SDL_FALSE){
				SDL_wheel_PauseMusic(1);
				SDL_wheel_PauseSe(1);
				pause = SDL_TRUE;
			}else{
				SDL_wheel_PauseMusic(0);
				SDL_wheel_PauseSe(0);
				pause = SDL_FALSE;
			}
		}
		if(pause == SDL_TRUE && (reps & BTN_PAUSE_SKIP)) pause_skip = SDL_TRUE;
	}

	if(pause_skip == SDL_TRUE || pause == SDL_FALSE){
		TSKexec();
		SDL_wheel_ExecFade();
		COLLISIONexec();
	}
	TSKdraw();
}

void game_fillcolor_init(void)
{
	SDL_wheel_SetFillColor(FILL_R, FILL_G, FILL_B, FILL_A);
}

void game_config_update(void)
{
	game_mode = config.game_mode;
	SDL_wheel_WriteFileMode(FILENAME_CONFIG, &config, sizeof(CONFIG), 1, "wb");
}

void game_backup_update(void)
{
	if(game_mode == GAMEMODE_SUPERHARD){
		if(score > backup.high_score) backup.high_score = score;
		backup.total_damage += total_damage;
		backup.play_time += play_time;
		backup.play_cnt++;

		backup.live_total += game_time;
		if(backup.live_short == 0 || backup.live_short > game_time) backup.live_short = game_time;
		if(backup.live_long < game_time) backup.live_long = game_time;
		backup.live_average = backup.live_total / backup.play_cnt;

		backup.move_total += move_total;
		if(backup.move_short == 0 || backup.move_short > move_total) backup.move_short = move_total;
		if(backup.move_long < move_total) backup.move_long = move_total;
		backup.move_average = backup.move_total / backup.play_cnt;

		backup.destroy_total += destroy_total;
		if(backup.destroy_short == 0 || backup.destroy_short > destroy_total) backup.destroy_short = destroy_total;
		if(backup.destroy_long < destroy_total) backup.destroy_long = destroy_total;
		backup.destroy_average = backup.destroy_total / backup.play_cnt;

		SDL_wheel_WriteFileMode(FILENAME_RESULT, &backup, sizeof(BACKUP), 1, "wb");
	}
}

void backup_init(void)
{
	if(SDL_wheel_ExistFile(FILENAME_CONFIG) == 0){
		printf("init backup data (config data).\n");
		config.game_mode = GAMEMODE_SUPERHARD;
		config.vol_bgm = SND_VOL_RATE;
		config.vol_se = SND_VOL_RATE;
		SDL_wheel_WriteFileMode(FILENAME_CONFIG, &config, sizeof(CONFIG), 1, "wb");
	}else{
		printf("read backup data (config data).\n");
		SDL_wheel_ReadFileMode(FILENAME_CONFIG, &config, sizeof(CONFIG), 1, "rb");
	}

	SDL_wheel_SetVolMusic(config.vol_bgm);
	SDL_wheel_SetVolSe(config.vol_se);

	if(SDL_wheel_ExistFile(FILENAME_RESULT) == 0){
		printf("init backup data (result data).\n");
		backup.high_score = 0;
		backup.total_damage = 0;
		backup.play_time = 0;
		backup.play_cnt = 0;
		backup.clear_cnt = 0;
		backup.live_total = 0;
		backup.live_short = 0;
		backup.live_long = 0;
		backup.live_average = 0;
		backup.move_total = 0;
		backup.move_short = 0;
		backup.move_long = 0;
		backup.move_average = 0;
		backup.destroy_total = 0;
		backup.destroy_short = 0;
		backup.destroy_long = 0;
		backup.destroy_average = 0;
		SDL_wheel_WriteFileMode(FILENAME_RESULT, &backup, sizeof(BACKUP), 1, "wb");
	}else{
		printf("read backup data (result data).\n");
		SDL_wheel_ReadFileMode(FILENAME_RESULT, &backup, sizeof(BACKUP), 1, "rb");
	}
}

