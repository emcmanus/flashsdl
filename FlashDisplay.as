package {
	import flash.display.*;
	import cmodule.libSDL.CLibInit;

	public class FlashDisplay extends Sprite {
		public function FlashDisplay() {
			var loader:CLibInit = new CLibInit;
			var lib:Object = loader.init();
			lib.run(this);
		}
	}
}
