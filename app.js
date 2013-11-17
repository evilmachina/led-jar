"use strict";
var io = require('socket.io').listen(1339);

var xbeeSerialPort = "/dev/tty.usbmodem16651";

var serialport = require('serialport');
var serialPort = new serialport.SerialPort(xbeeSerialPort,
{//Listening on the serial port for data coming from Arduino over USB
	baudRate: 57600,
	parser: serialport.parsers.readline('\n')
});

serialPort.on('data', function(data) {
    console.log('data received: ' + data);
  });  
var readyToSending = true;

var showAmplitude = function(data){
	var buffer = [];
	buffer.push(4);

	for(var i = 0; i < 9; i++){
	
    	var level = data[i].percentage / 100;
            buffer.push(Math.round(data[i].rgb[0]));
            buffer.push(Math.round(data[i].rgb[1]));
            buffer.push(Math.round(data[i].rgb[2]));

        var leds = ~~((100) * (level));
        buffer.push(leds);
        

    }
	serialPort.write(new Buffer(buffer), function(err, results) {
		if(err){
			console.log(err);
		}
    	readyToSending = true;
  	});  
};


io.sockets.on('connection', function (socket) {
	socket.on('volumes', function (data) {
		//console.log(data);
		if(readyToSending){
			readyToSending = false;
			showAmplitude(data.data);
		}else{
			console.log("nopp");
		}
  	});
      /*  socket.on('beat', function (msg) {
                //console.log(data);
		var data = msg.data;
		var level = data.percentage / 100;
               	var rgb = [Math.round(data.rgb[0] * level),
			   Math.round(data.rgb[1] * level),
			   Math.round(data.rgb[2] * level)];
        	
        	try{
                     //  serialPort.write(new Buffer([4, rgb[0], rgb[1], rgb[2], 10]));
        	}catch(e){
                	console.log(e);
        	}
                
        });*/

	socket.on('disconnect', function () {
   		 //off();
 	 });
});
//off();