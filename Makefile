CC = gcc
RM = rm -f
CP = cp
ST = strip --strip-unneeded
# Windows
EXT = .exe
# Linux
#EXT = 
# OpenPandora
#EXT = .pnd

INCFLAGS  = `sdl-config --cflags` -I/usr/include/mingw/SDL_wheel
INCFLAGS += -I/usr/local/include/SDL_wheel
DEFAULT_CFLAGS = -Wall -O2 $(INCFLAGS)
CFLAGS  = $(DEFAULT_CFLAGS)
LDFLAGS  = -L/usr/lib/mingw
LDFLAGS += -L/usr/local/lib
LDFLAGS += -lSDL_wheel
# Cygwin + MingW
LDFLAGS += -lSDL_image -lSDL_mixer -lmingw32 -lSDLmain -lSDL -mno-cygwin -mwindows
# Linux and OpenPandora
#LDFLAGS += -lSDL_image -lSDL_mixer -lSDLmain -lSDL

OBJS  = ./src/main.o
OBJS += ./src/data.o
OBJS += ./src/game.o
OBJS += ./src/stage_data.o
OBJS += ./src/stage_00.o
OBJS += ./src/stage_01.o
OBJS += ./src/stage_02.o
OBJS += ./src/stage_03.o
OBJS += ./src/stage_99.o
OBJS += ./src/enemy_cmn.o
OBJS += ./src/demoplay.o
OBJS += ./src/tsk_gctrl.o
OBJS += ./src/tsk_title.o
OBJS += ./src/tsk_menu.o
OBJS += ./src/tsk_option.o
OBJS += ./src/tsk_result.o
OBJS += ./src/tsk_info.o
OBJS += ./src/tsk_star.o
OBJS += ./src/tsk_bg.o
OBJS += ./src/tsk_bg01.o
OBJS += ./src/tsk_bg02.o
OBJS += ./src/tsk_bgmask.o
OBJS += ./src/tsk_opnbg.o
OBJS += ./src/tsk_quake.o
OBJS += ./src/tsk_fade.o
OBJS += ./src/tsk_explode.o
OBJS += ./src/tsk_particle.o
OBJS += ./src/tsk_ship.o
OBJS += ./src/tsk_sopt.o
OBJS += ./src/tsk_muzzle.o
OBJS += ./src/tsk_sshot.o
OBJS += ./src/tsk_nshot.o
OBJS += ./src/tsk_nuclear.o
OBJS += ./src/tsk_eshot.o
OBJS += ./src/tsk_zako01.o
OBJS += ./src/tsk_zako02.o
OBJS += ./src/tsk_zako03.o
OBJS += ./src/tsk_zako04.o
OBJS += ./src/tsk_zako05.o
OBJS += ./src/tsk_zako06.o
OBJS += ./src/tsk_zako07.o
OBJS += ./src/tsk_zako08.o
OBJS += ./src/tsk_middle01.o
OBJS += ./src/tsk_middle02.o
OBJS += ./src/tsk_boss01.o
OBJS += ./src/util/util_tsk.o
OBJS += ./src/util/util_func.o
OBJS += ./src/util/util_collision.o
OBJS += ./src/util/util_platinum.o
OBJS += ./src/util/util_menu.o
OBJS += ./src/debug/debug_main.o
OBJS += ./src/debug/debug_menu_top.o
OBJS += ./src/debug/debug_menu_star.o
OBJS += ./src/debug/debug_menu_ship.o
OBJS += ./src/debug/debug_menu_enemy.o
OBJS += ./src/debug/debug_menu_sequence.o
OBJS += ./src/debug/debug_menu_surface.o
OBJS += ./src/debug/debug_menu_sound.o

SRCS  = $(patsubst %.o, %.c, $(OBJS))

EXES  = shooting$(EXT)

ifeq ($(wildcard .depend),.depend)
do-it-all: $(EXES)
include .depend
else
do-it-all: depend recursive
recursive:
	make
endif

.PHONY: depend
depend: $(OBJS)
	$(CC) -MM $(CFLAGS) $(SRCS) > .depend

.PHONY: clean
clean:
	$(RM) $(OBJS) .depend $(EXES) stdout.txt stderr.txt

$(EXES): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

run: $(EXES)
	./$(EXES)
	cat stdout.txt

runf: $(EXES)
	./$(EXES) -f
	cat stdout.txt
