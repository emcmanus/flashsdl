package {
	import cmodule.libSDL.CLibInit;
	
	import flash.display.*;

	public class flashsdl extends Sprite {
		
		private var canvas:Bitmap;
		private var canvasData:BitmapData;
		
		public function flashsdl() {
			
			var loader:CLibInit;
			var lib:Object;
			
			// Setup Display
			canvasData = new BitmapData( 200, 150, false, 0x0 );	// This must be the same resolution given in SDL_setVideoMode (For now)
			canvas = new Bitmap( canvasData );
			canvas.width = canvasData.width*2;
			canvas.height = canvasData.height*2;
			
			canvas.x = (stage.stageWidth-canvas.width)/2;
			canvas.y = (stage.stageHeight-canvas.height)/2;
			
			this.addChild( canvas );
			
			
			// Lib Init
			loader = new CLibInit;
			loader.putEnv("SDL_VIDEODRIVER", "flash");
			
			lib = loader.init();
			lib.setBitmapData( canvasData );
			lib.run();
		}
		
	}
}
