var five = require("johnny-five");
var Raspi = require("raspi-io");
var board = new five.Board({
  io: new Raspi()
});

board.on("ready", function() {
  console.log("Ready!");
  var led = new five.Led("P1-13");
  led.blink();
});
