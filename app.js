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
var numberOfLEDs = 72;

var lightsOn = function(percentage, rgb){
	/*var i = 0;
	var ledArray = [];
	var ledsOn = ~~((numberOfLEDs/2) * (percentage / 100));
	var ledsOff = half - ledsOn;
	

	var ts = ledsOff <= tailSize ? ledsOff  : tailSize;

	ledsOff = Math.max(ledsOff - tailSize, 0);

	for( i = 0; i < ledsOff; i++){
		ledArray.push([0,0,0]);
	}
	for( i = ts; i > 0; i--){
		//var level = ((tailSize - i) / tailSize) * fade;
		var level =((tailSize - i) / tailSize) * fade;
		ledArray.push([rgb[0] * level, rgb[1] * level, rgb[2] * level]);
	}
	for( i = 0; i < ledsOn; i++){
		ledArray.push(rgb);
	}

	var revercedLedArray = ledArray.slice(0).reverse();

	var full = ledArray.concat(revercedLedArray);
	*/
	
	var leds = ~~((numberOfLEDs) * (percentage / 100));
	//console.log(leds);
	serialPort.write(new Buffer([5, rgb[0], rgb[1], rgb[2], leds]), function(err, results) {
		if(err){
			console.log(err);
		}
    	readyToSending = true;
  	});  
	
};


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
   // console.log(buffer.length);
    //readyToSending = true;
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