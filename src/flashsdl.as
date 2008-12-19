package {
	import flash.display.Sprite;
	
	import sdl.LibSDL;
	import sdl.video.VideoSurface;
	
	import utils.FPSCounter;
	
	[SWF(width=800,height=600,frameRate=60)]
	public class flashsdl extends Sprite {
		
		private var surface:VideoSurface;
		private var surfaceContainer:Sprite;
		
		private var libSDL:LibSDL;
		
		public function flashsdl() {
			
			// C Library setup
			this.libSDL = new LibSDL();
			this.surface = libSDL.getSurface( 800, 600 );
			
			// Build container for input events
			surfaceContainer = new Sprite();
			surfaceContainer.addChild( surface );
			addChild(surfaceContainer);
			
			libSDL.setEventTarget( surfaceContainer );
			
			// Monitor FPS performance
			var fps:FPSCounter = new FPSCounter();
			addChild(fps);
		}
		
	}
}
