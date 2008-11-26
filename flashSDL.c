#include "AS3.h"
#include <stdio.h>
#include "SDL.h"

extern AS3_Val FLASH_DISPLAY_BITMAP_DATA;

AS3_Val initDisplay(AS3_Val parent, int width, int height)
{
	AS3_Val Bitmap_class;
	AS3_Val BitmapData_class;
	AS3_Val bitmapData;
	AS3_Val bitmap;

	Bitmap_class = AS3_NSGetS(AS3_String("flash.display"), "Bitmap");
	BitmapData_class = AS3_NSGetS(AS3_String("flash.display"), "BitmapData");

	bitmapData = AS3_New(BitmapData_class, 
		AS3_Array("AS3ValType, AS3ValType, AS3ValType, AS3ValType",
			   AS3_Int(500), AS3_Int(500), AS3_False(), AS3_Int(0x000000)));

	bitmap = AS3_New(Bitmap_class, AS3_Array("AS3ValType", bitmapData));

	AS3_CallS("addChild", parent, AS3_Array("AS3ValType", bitmap));

	return bitmapData;
}

AS3_Val run(void *data, AS3_Val args)
{
	AS3_Val parent;
	AS3_Val bitmapData;

	AS3_ArrayValue(args, "AS3ValType", &parent);

	bitmapData = initDisplay(parent, 320, 240);
	FLASH_DISPLAY_BITMAP_DATA = bitmapData;

	SDL_Init(SDL_INIT_VIDEO);

	return bitmapData;
}

int main(int argc, char **argv)
{
	AS3_Val runFunction = AS3_Function(NULL, run);

	AS3_Val libSDL = AS3_Object("run:AS3ValType", runFunction);

	AS3_LibInit(libSDL);

	return 0;
}
