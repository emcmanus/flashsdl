package {
	import flash.display.*;
	import cmodule.libSDL.CLibInit;

	public class FlashDisplay extends Sprite {
		public function FlashDisplay() {
			var loader:CLibInit = new CLibInit;
		//	loader.putEnv("SDL_VIDEODRIVER", "flash");
			var lib:Object = loader.init();
			lib.run(this);
		}
	}
}
