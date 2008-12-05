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
	
	--------------------------------------------
	
	This is just a stub. The following code doesn't do anything yet, so feel
	free to help port this driver!
	
	--------------------------------------------
*/
#include "SDL_config.h"

#include "SDL_rwops.h"
#include "SDL_timer.h"
#include "SDL_audio.h"
#include "../SDL_audiomem.h"
#include "../SDL_audio_c.h"
#include "../SDL_audiodev_c.h"
#include "SDL_flashaudio.h"

/* The tag name used by flash audio */
#define FLASHAUD_DRIVER_NAME         "flash"

/* Audio driver functions */
static int FLASHAUD_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void FLASHAUD_WaitAudio(_THIS);
static void FLASHAUD_PlayAudio(_THIS);
static Uint8 *FLASHAUD_GetAudioBuf(_THIS);
static void FLASHAUD_CloseAudio(_THIS);

/* Audio driver bootstrap functions */
static int FLASHAUD_Available(void)
{
	const char *envr = SDL_getenv("SDL_AUDIODRIVER");
	if (envr && (SDL_strcmp(envr, FLASHAUD_DRIVER_NAME) == 0)) {
		return(1);
	}
	return(0);
}

static void FLASHAUD_DeleteDevice(SDL_AudioDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_AudioDevice *FLASHAUD_CreateDevice(int devindex)
{
	SDL_AudioDevice *this;

	/* Initialize all variables that we clean on shutdown */
	this = (SDL_AudioDevice *)SDL_malloc(sizeof(SDL_AudioDevice));
	if ( this ) {
		SDL_memset(this, 0, (sizeof *this));
		this->hidden = (struct SDL_PrivateAudioData *)
				SDL_malloc((sizeof *this->hidden));
	}
	if ( (this == NULL) || (this->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( this ) {
			SDL_free(this);
		}
		return(0);
	}
	SDL_memset(this->hidden, 0, (sizeof *this->hidden));

	/* Set the function pointers */
	this->OpenAudio = FLASHAUD_OpenAudio;
	this->WaitAudio = FLASHAUD_WaitAudio;
	this->PlayAudio = FLASHAUD_PlayAudio;
	this->GetAudioBuf = FLASHAUD_GetAudioBuf;
	this->CloseAudio = FLASHAUD_CloseAudio;

	this->free = FLASHAUD_DeleteDevice;

	return this;
}

AudioBootStrap FLASHAUD_bootstrap = {
	FLASHAUD_DRIVER_NAME, "SDL flash audio driver",
	FLASHAUD_Available, FLASHAUD_CreateDevice
};

/* This function waits until it is possible to write a full sound buffer */
static void FLASHAUD_WaitAudio(_THIS)
{
	/* Don't block on first calls to simulate initial fragment filling. */
	if (this->hidden->initial_calls)
		this->hidden->initial_calls--;
	else
		SDL_Delay(this->hidden->write_delay);
}

static void FLASHAUD_PlayAudio(_THIS)
{
	/* no-op...this is a null driver. */
}

static Uint8 *FLASHAUD_GetAudioBuf(_THIS)
{
	return(this->hidden->mixbuf);
}

static void FLASHAUD_CloseAudio(_THIS)
{
	if ( this->hidden->mixbuf != NULL ) {
		SDL_FreeAudioMem(this->hidden->mixbuf);
		this->hidden->mixbuf = NULL;
	}
}

static int FLASHAUD_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
	float bytes_per_sec = 0.0f;

	/* Allocate mixing buffer */
	this->hidden->mixlen = spec->size;
	this->hidden->mixbuf = (Uint8 *) SDL_AllocAudioMem(this->hidden->mixlen);
	if ( this->hidden->mixbuf == NULL ) {
		return(-1);
	}
	SDL_memset(this->hidden->mixbuf, spec->silence, spec->size);

	bytes_per_sec = (float) (((spec->format & 0xFF) / 8) *
	                   spec->channels * spec->freq);

	/*
	 * We try to make this request more audio at the correct rate for
	 *  a given audio spec, so timing stays fairly faithful.
	 * Also, we have it not block at all for the first two calls, so
	 *  it seems like we're filling two audio fragments right out of the
	 *  gate, like other SDL drivers tend to do.
	 */
	this->hidden->initial_calls = 2;
	this->hidden->write_delay =
	               (Uint32) ((((float) spec->size) / bytes_per_sec) * 1000.0f);

	/* We're ready to rock and roll. :-) */
	return(0);
}

