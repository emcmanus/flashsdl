// gcc flashSDL.c -DFLASH -Isdl/include -Lsdl -lSDL -swc -O3 -o libSDL.swc
// OLD -> mxmlc -library-path+=libSDL.swc --target-player=10.0.0 flashsdl.as

#include "SDL.h"
#include "AS3.h"
#include <stdio.h>

extern AS3_Val FLASH_DISPLAY_BITMAP_DATA;

// Prototypes
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
AS3_Val initDisplay(AS3_Val parent, int width, int height);
AS3_Val config(void* self, AS3_Val args);
int run();


/*
Lib entry
*/
int main(int argc, char **argv){
    
	// Create callbacks
	AS3_Val runMethod = AS3_Function(NULL, run);
	AS3_Val configMethod = AS3_Function(NULL, config);
	
    AS3_Val libSDL = AS3_Object("run:AS3ValType, config:AS3ValType", runMethod, configMethod);
    
    AS3_LibInit(libSDL);
    return 0;
}



/*
Takes an AS3_Val parent, adds a new bitmap object of the specified width/height,
and returns a reference to the its bitmapdata, which we then use internally
for drawing/updating the display.
*/
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


/*
Call this before actually running the method containing your main loop. This accepts a
reference to a sprite. We'll later add a bitmap object as a child on that sprite which
we can draw to.
*/
AS3_Val config( void* self, AS3_Val args ){
	
	AS3_Val parent;
	AS3_ArrayValue( args, "AS3ValType", &parent );
	
	FLASH_DISPLAY_BITMAP_DATA = initDisplay(parent, 800, 600);
	
	return AS3_Int(0);
}



int run() {
 SDL_Surface *screen;
 int quit = 0;
 SDL_Event event;
 int x, y;
 int i;	// main loop
 Uint32 yellow;

 // Initialize defaults, Video and Audio
 if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1))
 {
  printf("Could not initialize SDL: %s.\n", SDL_GetError());
  return -1;
 }

 screen = SDL_SetVideoMode(800, 600, 24, SDL_SWSURFACE | SDL_FULLSCREEN);
 if ( screen == NULL )
 {
  fprintf(stderr, "Couldn't set 800x600x24 video mode: %s\n", SDL_GetError());
  return -2;
 }

 // Map the color yellow to this display (R=0xff, G=0xFF, B=0x00)
 yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
 
 // Make the dot at the center of the screen
 x = screen->w / 2;
 y = screen->h / 2;


 //while( !quit )
 for ( i=0; i<5; ++i )	// Flash pegging == bad
 {
  // Poll for events
  while( SDL_PollEvent( &event ) )
  {
   switch( event.type )
   {
    case SDL_KEYUP:
     if(event.key.keysym.sym == SDLK_ESCAPE)
      quit = 1;
      break;
     if(event.key.keysym.sym == SDLK_F1)
      SDL_WM_ToggleFullScreen(screen); // Only on X11
      break;
    case SDL_QUIT:
     quit = 1;
     break;
    default:
     break;
   }
  }

  // Lock the screen for direct access to the pixels
  if ( SDL_MUSTLOCK(screen) )
  {
   if ( SDL_LockSurface(screen) < 0 )
   {
    fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
    return -3;
   }
  }

  // Plot the Pixel
  putpixel(screen, x, y, yellow);

  // Unlock Surface if necessary
  if ( SDL_MUSTLOCK(screen) )
  {
   SDL_UnlockSurface(screen);
  }

  // Update just the part of the display that we've changed
  SDL_UpdateRect(screen, x, y, 1, 1);
 }

 SDL_Quit();
 
 return 0;
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
 int bpp = surface->format->BytesPerPixel;
 // Here p is the address to the pixel we want to set
 Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

 switch(bpp)
 {
  case 1:
   *p = pixel;
   break;
  case 2:
   *(Uint16 *)p = pixel;
   break;
  case 3:
   if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
   {
    p[0] = (pixel >> 16) & 0xff;
    p[1] = (pixel >> 8) & 0xff;
    p[2] = pixel & 0xff;
   } else {
    p[0] = pixel & 0xff;
    p[1] = (pixel >> 8) & 0xff;
    p[2] = (pixel >> 16) & 0xff;
   }
   break;
  case 4:
   *(Uint32 *)p = pixel;
   break;
 }
}
