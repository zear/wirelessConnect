#PLATFORM = linux_x86
#PLATFORM = linux_x64
#PLATFORM = mingw32	# win32
#PLATFORM = cegcc	# wince (arm)
#PLATFORM = gizmondo
#PLATFORM = gp2x
#PLATFORM = wiz
#PLATFORM = caanoo
#PLATFORM = dingux
#PLATFORM = gcw

FPS_MACRO ?= 60
PLATFORM ?= linux_x86

### Dingoo/Dingux
ifeq ($(PLATFORM), dingux)
	CC = mipsel-linux-gcc
	STRIP = mipsel-linux-strip
	CFLAGS = -mips32 -mtune=mips32 -G0 -fomit-frame-pointer -ffunction-sections -ffast-math -fsingle-precision-constant -mbranch-likely
	INCLUDE = -I/opt/opendingux-toolchain/usr/include/SDL
	LIB = -lSDL_mixer -lSDL -lm -lpthread -lSDL_gfx
endif

### GCW Zero
ifeq ($(PLATFORM), gcw)
	CC = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc
	STRIP = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-strip
	CFLAGS = -mips32 -mtune=mips32 -G0 -fomit-frame-pointer -ffunction-sections -ffast-math -fsingle-precision-constant -mbranch-likely
	LDFLAGS = -L/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/lib/
	INCLUDE = -I/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include/ -I/opt/opendingux-toolchain/usr/include/SDL
	LIB = -lSDL_mixer -lSDL -lm -lpthread
endif

### GP2X/Wiz
ifeq ($(PLATFORM), gp2x)
	CC = arm-open2x-linux-gcc
	STRIP = arm-open2x-linux-strip
	CFLAGS =
	LDFLAGS = -static # GP2X requires static linking, Wiz does not
	LDFLAGS += -L/opt/open2x/gcc-4.1.1-glibc-2.3.6/lib/
	INCLUDE = -I/opt/open2x/gcc-4.1.1-glibc-2.3.6/include/
	LIB = -lSDL_mixer -lsmpeg -lSDL -lm -lpthread -lstdc++
endif
ifeq ($(PLATFORM), wiz)
	CC = arm-open2x-linux-gcc
	STRIP = arm-open2x-linux-strip
	CFLAGS =
	LDFLAGS += -L/opt/open2x/gcc-4.1.1-glibc-2.3.6/lib/
	INCLUDE = -I/opt/open2x/gcc-4.1.1-glibc-2.3.6/include/
	LIB = -lSDL_mixer -lsmpeg -lSDL -lm -lpthread -lstdc++
endif

### Caanoo
ifeq ($(PLATFORM), caanoo)
	CC = arm-gph-linux-gnueabi-gcc
	STRIP = arm-gph-linux-gnueabi-strip
	CFLAGS =
	LIB = -lSDL_mixer -lSDL -lm -lpthread
endif

### x86 linux
ifeq ($(PLATFORM), linux_x86)
	CC = gcc
	STRIP = strip
	CFLAGS =
	INCLUDE = -I/usr/include/ -I/usr/include/SDL
	LIB = -lSDL_mixer -lSDL -lSDL_gfx -lm -lpthread
endif

### win32
ifeq ($(PLATFORM), mingw32)
	CFLAGS =
	INCLUDE = -I/usr/i486-mingw32/include
	CC = i486-mingw32-gcc
	STRIP = i486-mingw32-strip
	LIB = -lmingw32 -lSDLmain -lSDL_mixer -lSDL -lm -lpthread -mwindows
endif

### wince (arm)
ifeq ($(PLATFORM), cegcc)
	CFLAGS = -D_WIN32_IE=0x0400
	INCLUDE = -I/opt/cegcc/arm-cegcc/include/
	LDFLAGS = -static -L/opt/cegcc/arm-cegcc/lib/
	CC = arm-cegcc-gcc
	STRIP = arm-cegcc-strip
	LIB = -lSDL_mixer -lSDL -lm -lpthread -mwindows
endif

### Gizmondo
ifeq ($(PLATFORM), gizmondo)
	CFLAGS = -Wall -ffast-math -funroll-loops -mcpu=arm920 -mtune=arm920t -fstrict-aliasing -fexpensive-optimizations -falign-functions -fweb -frename-registers -fomit-frame-pointer -finline -finline-functions -fno-builtin -fno-common -mstructure-size-boundary=8
	INCLUDE = -I/opt/cegcc/arm-cegcc/gizmondo/include/
	LDFLAGS = -static -L/opt/cegcc/arm-cegcc/gizmondo/lib/
	CC = arm-cegcc-gcc
	STRIP = arm-cegcc-strip
	LIB = /opt/cegcc/arm-cegcc/gizmondo/lib/SDL_mixer.dll /opt/cegcc/arm-cegcc/gizmondo/lib/SDL.dll -lm -lpthread
endif

###
SVNREV = -DVERSION_REVISION="$(shell svnversion -n -c . | cut -s -d: -f2 | tr -d MS)" # calculates the svn revision number

ifdef DEBUG
	CFLAGS += -Wextra -Wall -ggdb3 -c -O2 $(SVNREV)
else
	CFLAGS += -c -O2 $(SVNREV)
endif

ifdef CHEATS
	CFLAGS += -DWITH_CHEATS
endif

ifdef NOFPSLIMIT
	CFLAGS += -DNO_FPS_LIMIT
endif

ifdef DUMA
	LIB += -lduma
endif

CFLAGS += -DFPS_MACRO=$(FPS_MACRO)
	

SRC =	src/main.c		\
	src/draw.c  		\
	src/fileio.c		\
	src/font.c		\
	src/input.c 		\
	src/logic.c 		\
	src/menu.c		\
	src/network.c		\
	src/onScreenKeyboard.c	\
	src/SDLmain.c 		\
	src/SDLgfx.c 		\
	src/timer.c
OBJ = $(SRC:.c=.o)
EXE = wirelessConnect.bin

all : $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIB) -o $@
ifndef DEBUG
	$(STRIP) $(EXE)
endif

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf src/*.o src/game/*.o src/int/*.o $(EXE)
