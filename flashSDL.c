#include "AS3.h"
#include <stdio.h>
#include "SDL.h"

extern AS3_Val FLASH_DISPLAY_PARENT_SPRITE;

AS3_Val run(void *data, AS3_Val args)
{
	AS3_Val parent;
	SDL_Surface *screen;

	AS3_ArrayValue(args, "AS3ValType", &parent);

	FLASH_DISPLAY_PARENT_SPRITE = parent;

	SDL_Init(SDL_INIT_VIDEO);

	screen = SDL_SetVideoMode(320, 240, 8, SDL_ANYFORMAT);

	return parent;
}

int main(int argc, char **argv)
{
	AS3_Val runFunction = AS3_Function(NULL, run);

	AS3_Val libSDL = AS3_Object("run:AS3ValType", runFunction);

	AS3_LibInit(libSDL);

	return 0;
}
