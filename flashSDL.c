#include <stdio.h>

#include "SDL.h"
#include "AS3.h"


// Prototypes

AS3_Val setBitmapData(void *data, AS3_Val bitmapData);

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int run();

extern AS3_Val FLASH_DISPLAY_BITMAP_DATA;



/*
	Lib Initialization
*/

int main(int argc, char **argv){
    
	// Create callbacks
	AS3_Val runMethod = AS3_Function(NULL, run);
	AS3_Val setBitmapDataMethod = AS3_Function(NULL, setBitmapData);
	
    AS3_Val libSDL = AS3_Object("run:AS3ValType, setBitmapData:AS3ValType", runMethod, setBitmapDataMethod);
    
	AS3_Release( runMethod );
	AS3_Release( setBitmapDataMethod );
	
    AS3_LibInit(libSDL);
    return 0;
}


/*
	Associates SDL's buffer with a user's BitmapData object. This is a more
	robust solution than asking for a sprite and adding a child bitmap.
	
	TODO: Either scale the display buffer to bitmapdata's dimensions or enforce
	size requirements (ie, your bitmapdata obj must be of the same dimensions
	specified in SDL_setViewMode)
*/

AS3_Val setBitmapData(void *data, AS3_Val args) {
	
	AS3_Val bitmapDataObject;
	
	AS3_ArrayValue( args, "AS3ValType", &bitmapDataObject );
	
	FLASH_DISPLAY_BITMAP_DATA = bitmapDataObject;
	
	return AS3_Int(0);
}










/*
	Program logic
*/

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

 screen = SDL_SetVideoMode(200, 150, 32, SDL_SWSURFACE | SDL_FULLSCREEN);
 if ( screen == NULL )
 {
  fprintf(stderr, "Couldn't set 320x240x24 video mode: %s\n", SDL_GetError());
  return -2;
 }

 // Map the color yellow to this display (R=0xff, G=0xFF, B=0x00)
 yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
 
 // Make the dot at the center of the screen
 x = screen->w / 2;
 y = screen->h / 2;


 //while( !quit )
 for ( i=0; i<1; ++i )	// Flash pegging == bad
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

// Print pixel
char buf[32];
sprintf( buf, "setting Pixel %i, %i to 0x%X", x, y, pixel );
sztrace( buf );
free(buf);

 switch(bpp)
 {
  case 1:
   *p = pixel;
   break;
  case 2:
   *(Uint16 *)p = pixel;
   break;
  case 3:
   sztrace("bbp==3");
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
