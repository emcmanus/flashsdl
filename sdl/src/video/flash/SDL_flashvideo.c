/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_flashvideo.h"
#include "SDL_flashevents_c.h"
#include "SDL_flashmouse_c.h"


#define FLASHVID_DRIVER_NAME "flash"


/* Initialization/Query functions */
static int FLASH_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **FLASH_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *FLASH_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int FLASH_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void FLASH_VideoQuit(_THIS);

/* Hardware surface functions */
static int FLASH_AllocHWSurface(_THIS, SDL_Surface *surface);
static int FLASH_LockHWSurface(_THIS, SDL_Surface *surface);
static void FLASH_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void FLASH_FreeHWSurface(_THIS, SDL_Surface *surface);

/* etc. */
static void FLASH_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

/* FLASH driver bootstrap functions */

static int FLASH_Available(void)
{
	const char *envr = SDL_getenv("SDL_VIDEODRIVER");
	
	if ((envr) && (SDL_strcmp(envr, FLASHVID_DRIVER_NAME) == 0)) {
		return(1);
	}

	return(0);
}

static void FLASH_DeleteDevice(SDL_VideoDevice *device)
{
	sztrace("FLASH_DeleteDevice(SDL_VideoDevice *device)\n");

	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *FLASH_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	sztrace("FLASH_CreateDevice(int devindex)\n");

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}
	SDL_memset(device->hidden, 0, (sizeof *device->hidden));

	/* Set the function pointers */
	device->VideoInit = FLASH_VideoInit;
	device->ListModes = FLASH_ListModes;
	device->SetVideoMode = FLASH_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = FLASH_SetColors;
	device->UpdateRects = FLASH_UpdateRects;
	device->VideoQuit = FLASH_VideoQuit;
	device->AllocHWSurface = FLASH_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = FLASH_LockHWSurface;
	device->UnlockHWSurface = FLASH_UnlockHWSurface;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = FLASH_FreeHWSurface;
	device->SetCaption = NULL;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;
	device->InitOSKeymap = FLASH_InitOSKeymap;
	device->PumpEvents = FLASH_PumpEvents;

	device->free = FLASH_DeleteDevice;


	return device;
}

VideoBootStrap FLASH_bootstrap = {
	FLASHVID_DRIVER_NAME, "SDL Flash video driver",
	FLASH_Available, FLASH_CreateDevice
};


int FLASH_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	sztrace("FLASH_VideoInit(_THIS, SDL_PixelFormat *vformat)\n");

	/* Determine the screen depth (use default 8-bit depth) */
	/* we change this during the SDL_SetVideoMode implementation... */
	vformat->BitsPerPixel = 8;
	vformat->BytesPerPixel = 1;

	/* We're done! */
	return(0);
}

SDL_Rect **FLASH_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
   	 return (SDL_Rect **) -1;
}

SDL_Surface *FLASH_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{

	sztrace("FLASH_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags)\n");
		
	if ( this->hidden->buffer ) {
		SDL_free( this->hidden->buffer );
	}
	
	this->hidden->buffer = SDL_malloc(width * height * (bpp / 8));
	if ( ! this->hidden->buffer ) {
		SDL_SetError("Couldn't allocate buffer for requested mode");
		return(NULL);
	}
	
/* 	printf("Setting mode %dx%d\n", width, height); */
	
	SDL_memset(this->hidden->buffer, 0, width * height * (bpp / 8));
	
	/* Allocate the new pixel format for the screen */
	if ( ! SDL_ReallocFormat(current, bpp, 0, 0, 0, 0) ) {
		SDL_free(this->hidden->buffer);
		this->hidden->buffer = NULL;
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return(NULL);
	}
	
	/* Set up the new mode framebuffer */
	current->flags = flags & SDL_FULLSCREEN;
	this->hidden->w = current->w = width;
	this->hidden->h = current->h = height;
	current->pitch = current->w * (bpp / 8);
	current->pixels = this->hidden->buffer;
	
	/* We're done */
	return(current);
}

/* We don't actually allow hardware surfaces other than the main one */
static int FLASH_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	sztrace("FLASH_AllocHWSurface(_THIS, SDL_Surface *surface)\n");
	return(-1);
}
static void FLASH_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	sztrace("FLASH_FreeHWSurface(_THIS, SDL_Surface *surface)\n");
	return;
}

/* We need to wait for vertical retrace on page flipped displays */
static int FLASH_LockHWSurface(_THIS, SDL_Surface *surface)
{
	sztrace("FLASH_LockHWSurface(_THIS, SDL_Surface *surface)\n");
	return(0);
}

static void FLASH_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	sztrace("FLASH_UnlockHWSurface(_THIS, SDL_Surface *surface)\n");
	return;
}


static void FLASH_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	//sztrace("FLASH_UpdateRects(_THIS, int numrects, SDL_Rect *rects)\n");
	
	// Print endian
	/*
	char str_endian[24];
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
		sprintf(str_endian, "\n%s", "big endian");
	} else {
		sprintf(str_endian, "\n%s", "little endian");
	}
	*/
	
	
	/*
		Draw method
	*
	
	// Create new byteArray
	AS3_Val buffer_NS = AS3_String("flash.utils");
	AS3_Val buffer_class = AS3_NSGetS(buffer_NS, "ByteArray");
	AS3_Val buffer_array = AS3_New(buffer_class, AS3_Array(""));
	AS3_Release(buffer_NS);
	AS3_Release(buffer_class);
	
	int bpp = SDL_VideoSurface->format->BytesPerPixel;
	int buffer_length = SDL_VideoSurface->pitch * this->hidden->h * bpp;
	
	// Get Ram, note current position
	AS3_Val flash_ram = AS3_Ram();
	AS3_Val old_ram_position = AS3_GetS( flash_ram, "position" );
	int buffer_position = (int) &SDL_VideoSurface->pixels;
	
	// Move head to buffer position
	AS3_SetS( flash_ram, "position", AS3_Int(buffer_position) );
	
	// Debug
	sztrace("buffer address:");
	AS3_Trace( AS3_Int(buffer_position) );
	
	sztrace("current ram position:");
	AS3_Trace( AS3_GetS(flash_ram, "position") );
	
	sztrace("ram length:");
	AS3_Trace( AS3_GetS(flash_ram, "length") );
	
	
	// Copy buffer into new byte array
	AS3_ByteArray_readBytes( &buffer_array, flash_ram, buffer_length );
	
	// call setPixels with the buffer byteArray
	AS3_Val rect_NS = AS3_String("flash.geom");
	AS3_Val rect_Class = AS3_NSGetS(rect_NS, "Rectangle");
	AS3_Val setPixels_Rect = AS3_New(rect_Class, AS3_Array("IntType, IntType, IntType, IntType", 
				0, 0, SDL_VideoSurface->pitch, this->hidden->h));
	AS3_Release(rect_NS);
	AS3_Release(rect_Class);
	
	sztrace("buffer array length:");
	AS3_Trace( AS3_GetS(buffer_array, "length") );
	
	AS3_Val setPixels_Params = AS3_Array("AS3ValType, AS3ValType", setPixels_Rect, buffer_array);
	AS3_CallS("setPixels", this->hidden->bitmap_data, setPixels_Params);
	
	// reset ram position
	AS3_SetS( flash_ram, "position", old_ram_position );
	
	*/
	
//	// Debug -- are we actually copying any data?
//	int i;
//	int checksum=0;
//	for(i=0; i<(bufferLength/4); i++){
//		checksum += AS3_IntValue(AS3_CallS("readUnsignedInt", bufferArray, AS3_Array("")));
//	}
//	
//	sprintf( buf, "Checksum = %i", checksum );
//	sztrace(buf);
//	free(buf);
	
	
	// New Attempt -- Copy Buffer from RAM into a new ByteArray
	/*
	
	char buf[56];
	sprintf(buf, "width: %d, height: %d", this->hidden->w, this->hidden->h);
	sztrace(strcat(buf, str_endian));
	
	free(buf);
	free(str_endian);
	*/
	
//	AS3_Val ram = AS3_Ram();
//	
//	AS3_Val bufferNS = AS3_String("flash.utils");
//	AS3_Val bufferClass = AS3_NSGetS(bufferNS, "ByteArray");
//	AS3_Release(bufferNS);
//	
//	AS3_Val bufferArray = AS3_New(bufferClass, AS3_Array(""));
	
	// Get Buffer address (ram offset)
//	AS3_Val bufPosition = AS3_Ptr(SDL_VideoSurface->pixels);	// I hope this handles uints properly
//	int bpp = SDL_VideoSurface->format->BytesPerPixel;
	
	
//	int bufferLength = SDL_VideoSurface->pitch * this->hidden->h * bpp;
	
	
	// Store old ram position
	//AS3_Val oldRamPosition;
	//oldRamPosition = AS3_GetS(ram, "position");
	
	// Move ram head to buffer -- *none* of these work!
	//AS3_Set( ram, AS3_String("position"), bufPosition );
//	AS3_ByteArray_seek( ram, AS3_IntValue(bufPosition), SEEK_SET );
	//AS3_CallS( "position", ram, AS3_Array("IntType", bufPosition) );
	
	//AS3_SetS(bufferArray, "position", AS3_Int(0));
	//AS3_ByteArray_writeBytes( bufferArray, , bufferLength );
	
	//AS3_SetS(bufferArray, "position", AS3_Int(0));
	
	// Next step -- are we actually copying anything from the display buffer?
//	int i;
//	int checksum=0;
//	for(i=0; i<(bufferLength/4); i++){
//		checksum += AS3_IntValue(AS3_CallS("readUnsignedInt", bufferArray, AS3_Array("")));
//	}
//	
//	sprintf( buf, "Checksum = %i", checksum );
//	sztrace(buf);
//	free(buf);
	
//	AS3_Val firstPx = AS3_CallS( "readUnsignedInt", bufferArray, AS3_Array("") );
//	sztrace("First Pixel:");
//	AS3_Trace(firstPx);
	
	
	// Create new rect(0, 0, w, h);
//	AS3_Val rectNs = AS3_String("flash.geom");
//	AS3_Val rectClass = AS3_NSGetS(rectNs, "Rectangle");
//	AS3_Val setPixels_Rect = AS3_New(rectClass, AS3_Array("IntType, IntType, IntType, IntType", 
//				0, 0, this->hidden->w, this->hidden->h));
//	AS3_Release(rectNs);
//	AS3_Release(rectClass);
	
	/*
	sztrace("Buffer length:");
	AS3_Trace(AS3_GetS(bufferArray, "length"));
	*/
	
	//sztrace("new2");
	
//	AS3_Val setPixels_Params = AS3_Array("AS3ValType, AS3ValType", setPixels_Rect, ram);
//	AS3_CallS("setPixels", this->hidden->bitmap_data, setPixels_Params);
	
	/*
	AS3_Val _displayBuffer = FLASH_newByteArrayFromMalloc( SDL_VideoSurface->pixels, bufferLength );
	
	// Parameters for setPixels call
	AS3_Val setPixels_Params = AS3_Array("AS3ValType, AS3ValType", setPixels_Rect, _displayBuffer);
	sztrace("display buffer:");
	AS3_Trace(_displayBuffer);
	AS3_CallS("setPixels", this->hidden->bitmap_data, setPixels_Params);
	
	
	// Reset ram head to old position
	//AS3_SetS( ram, "position", oldRamPosition );
	*/
	
	
	// AS3 Callback drawing method. This renders the whole frame, not just the appropriate Rect.
//	
//	AS3_Val toolsNS = AS3_String("tools");
//	AS3_Val staticCallbacks = AS3_NSGetS(toolsNS, "StaticCallbacks");
//	
//	AS3_CallS( "printBuffer", staticCallbacks, AS3_Array("PtrType", SDL_VideoSurface->pixels));
//	
//	AS3_Release( toolsNS );
//	AS3_Release( staticCallbacks );
	
	
	
//	// First true drawing attempt -- For width->for height, setPixel
//	int i, j;
//	
//
//	
//	
//	// Here p is the address to the pixel we want to set
//	Uint8 r, g, b;
//	Uint32 pixel;
//	int offset;
//	
//	AS3_Val setPixel_params;
//	
//	// Lock surfaces
//	AS3_CallS("lock", this->hidden->bitmap_data, NULL);
//	SDL_LockSurface(SDL_VideoSurface);
//	
//	// i=column, j=row
//	for ( i=0; i<SDL_VideoSurface->pitch; i++ ){
//		for ( j=0; j<this->hidden->h; j++ ){
//			offset = (SDL_VideoSurface->pitch/bpp * j + i);
//			
//			pixel = ((Uint32 *)SDL_VideoSurface->pixels)[offset];
//			pixel |= 0xFF000000;
//			
//			setPixel_params = AS3_Array("IntType, IntType, IntType", i, j, pixel);
//			AS3_CallS("setPixel32", this->hidden->bitmap_data, setPixel_params);
//		}
//	}
	
	// Unlock surfaces
	//AS3_CallS("unlock", this->hidden->bitmap_data, NULL);
	//SDL_UnlockSurface(SDL_VideoSurface);
}

int FLASH_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	sztrace("FLASH_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)\n");
	/* do nothing of note. */
	return(1);
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
void FLASH_VideoQuit(_THIS)
{
	sztrace("FLASH_VideoQuit(_THIS)\n");

	if (this->screen->pixels != NULL)
	{
		SDL_free(this->screen->pixels);
		this->screen->pixels = NULL;
	}
}

