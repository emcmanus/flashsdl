
libSDL.swc: libSDL.a
	gcc flashSDL.c -DFLASH -Isdl/include -Lsdl -lSDL -swc -O3 -o libSDL.swc

libSDL.a:
	cd sdl; make -f Makefile.flash clean all; cd ..

clean:
	cd sdl; make -f Makefile.flash clean; cd ..
	rm FlashDisplay.swf
	rm libSDL.swc
	rm sdl/SDL.l.bc
