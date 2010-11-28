#SDL version 1.2.0 = 1
#SDL version 1.3.0 = 2
SDL_BUILD_VERSION := 1
SDL_BUILD_MIXER := true

ifeq ($(SDL_BUILD_VERSION), 2)
SDL_INCLUDE := external/sdl/libs/sdl_1.3.0/include
else
SDL_INCLUDE := external/sdl/libs/sdl_1.2.0/include
endif