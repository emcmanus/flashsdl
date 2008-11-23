#include "AS3.h"
#include <stdio.h>
#include "SDL.h"

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

AS3_Val _fillRect(AS3_Val bitmapData, int x, int y, int w, int h, int c)
{
	AS3_Val Rectangle_class;
	AS3_Val rectangle;
	AS3_Val xloc = AS3_Int(x);
	AS3_Val yloc = AS3_Int(y);
	AS3_Val width = AS3_Int(w);
	AS3_Val height = AS3_Int(h);
	AS3_Val color = AS3_Int(c);
	AS3_Val rectangle_params = AS3_Array("AS3ValType, AS3ValType, AS3ValType, AS3ValType", xloc, yloc, width, height);
	AS3_Val fillRect_params;

	Rectangle_class = AS3_NSGetS(AS3_String("flash.geom"), "Rectangle");

	rectangle = AS3_New(Rectangle_class, rectangle_params);

	fillRect_params = AS3_Array("AS3ValType, AS3ValType", rectangle, color);

	AS3_CallS("fillRect", bitmapData, fillRect_params);

	return AS3_True();
}

AS3_Val run(void *data, AS3_Val args)
{
	AS3_Val parent;
	AS3_Val bitmapData;

	AS3_ArrayValue(args, "AS3ValType", &parent);

	bitmapData = initDisplay(parent, 320, 240);

	SDL_Init(SDL_INIT_VIDEO);

	_fillRect(bitmapData, 50, 50, 50, 50, 0xFF00FF);

	return bitmapData;
}

int main(int argc, char **argv)
{
	AS3_Val runFunction = AS3_Function(NULL, run);

	AS3_Val libSDL = AS3_Object("run:AS3ValType", runFunction);

	AS3_LibInit(libSDL);
}
