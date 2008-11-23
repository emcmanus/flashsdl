
FlashDisplay.swf: libSDL.swc
	mxmlc -library-path+=libSDL.swc --target-player=10.0.0 FlashDisplay.as

libSDL.swc: libSDL.a
	gcc flashSDL.c -DFLASHVM -Isdl/include -Lsdl -lSDL -swc -O3 -o libSDL.swc

libSDL.a:
	cd sdl; make -f Makefile.flashvm clean all; cd ..
