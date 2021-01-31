OBJS = src/main.cc \
	src/Vec2f.cc \
	src/Entity.cc \
	src/Text.cc \
	src/MainGameScene.cc

CC = clang++
WEBCC = em++
WINCC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
WEB_FLAGS = -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 --preload-file assets/

OBJ_NAME = build/linux/ggj2021
WEB_NAME = build/web/ggj2021.html
WIN_NAME = ggj2021.exe


desktop: $(OBJS) 
		$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
web: $(OBJS) 
		$(WEBCC) $(OBJS) $(COMPILER_FLAGS) -o $(WEB_NAME) $(WEB_FLAGS)
win:	
	$(WINCC) -w -std=c++14 -Wfatal-errors \
	./src/*.cc \
	-o $(WIN_NAME) \
	-I"C:\SDL2LIB\include" \
	-L"C:\SDL2LIB\lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer \
	-lSDL2
