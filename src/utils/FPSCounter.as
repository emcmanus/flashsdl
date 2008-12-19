package utils {

import flash.display.Sprite;
import flash.events.TimerEvent;
import flash.events.Event;
import flash.text.TextField;
import flash.text.TextFieldAutoSize;
import flash.text.TextFormat;
import flash.utils.Timer;

public class FPSCounter extends Sprite {

private var _tf:TextField;
private var _fmt:TextFormat;
private var _timer:Timer;
private var _frameNum:int;

public function FPSCounter() {
_fmt = new TextFormat("_sans", 11, 0xCCCCCC);
_frameNum = 0;
init();
}

private function init():void {
_tf = createText();
addChild(_tf);
_timer = new Timer(1000);
_timer.addEventListener(TimerEvent.TIMER, displayFPS, false, 0, true);
this.addEventListener(Event.ENTER_FRAME, increaseFrame, false, 0, true);
_timer.start();
}

private function displayFPS(te:TimerEvent):void {
_tf.text = "fps = " + _frameNum;
_frameNum = 0;
}

private function increaseFrame(e:Event):void {
++ _frameNum;
}

private function createText():TextField {
var t:TextField = new TextField();
t.width = 0;
t.height = 0;
t.autoSize = TextFieldAutoSize.LEFT;
t.selectable = false;
t.defaultTextFormat = _fmt;
return t;
}

public function set textColor(col:uint):void {
_tf.textColor = col;
}
}
}