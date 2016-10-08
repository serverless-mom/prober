var five = require("johnny-five");
var Raspi = require("raspi-io");
var board = new five.Board({
  io: new Raspi()
});

board.on("ready", function() {
  var timer = 0;
  var led = new five.Led("P1-13");
  while(timer<50){
    led.toggle();
    timer++;
  }
});
