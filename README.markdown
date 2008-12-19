##FlashSDL - A Simple Flash Port of SDL##


*This codebase is published as a proof-of-concept more than anything else.* I haven't taken the time to add proper documentation so proceed with caution!

####General Pointers####

  - Most bridging logic is contained in the AS3 portion of the project *(/src)*. I'd prefer to build the flash driver in C, rather than a mix of C and AS3, so the code can be contributed to the SDL trunk. But at this point it's alot easier to just stub in some C methods and write the core control logic in AS3.
  - Keyboard and Mouse events are (more or less) supported.
  - The display buffer is simply mapped to a BitmapData object and updates every time the playhead enters a new frame.
  - This port does not support sound! (But I've added the stubs so *you* can contribute that functionality!)


####On Building####

Start by building libSDL.swc *(there's a Make target in /Makefile to do just this!)*. Then link libSDL.swc to your AS3 project and build */src/flashsdl.as*.


####Using FlashSDL in your own project#####
Start by examining */flashsdl.c*. Most immediately you'll have to refactor your C application's main loop to run iteratively in the tick() method.

Don't ask me how to compile your C application. So far that's an unsupported use case ;)
