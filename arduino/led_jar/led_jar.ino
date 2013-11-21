#include <Adafruit_NeoPixel.h>
 
int leds = 74;
int beatPixels = 40;
byte inByte = 0;
int bytesread = 0;
char code[36];

Adafruit_NeoPixel strips[8] = {
  Adafruit_NeoPixel(leds, 8, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(leds, 7, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(leds, 6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(leds, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(leds, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(leds, 21, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(leds, 20, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(leds, 14, NEO_GRB + NEO_KHZ800)
};

Adafruit_NeoPixel beat = Adafruit_NeoPixel(beatPixels, 22, NEO_GRB + NEO_KHZ800);

byte lastformat[72];
boolean gettingPattern = false;
byte patternCount = 0;
byte patternLength = 16;


void setup() {    
 // memset(lastformat, 0, sizeof(lastformat));  
  Serial.begin(57600);

  for(byte i = 0; i < 8; i++) {
    strips[i].begin();
    strips[i].show();
  }
  
  beat.begin();
  beat.show();
}

void loop() {
 // setColor(255,0,0,10);
  readData();
  //writeOut(); 
  //colorWipe(dummyStrip.Color(255, 0, 0), 50); // Red
  //colorWipe(dummyStrip.Color(0, 255, 0), 50); // Green
  //colorWipe(dummyStrip.Color(0, 0, 255), 50); // Blue
  //colorWipe(dummyStrip.Color(255, 255, 255), 50);
 // rainbow(20);
  //rainbowCycle(20);  
}

void updateAll(){
  for(byte i = 0; i < 8; i++) { 
    strips[i].show();   
  }  
}


void oneDirection(int r, int g, int b, int percent, int strip){
   
  int leds = 72 * (percent/100.0);
  strips[strip].setBrightness(100 * (percent/100.0));
  for(int i=0; i < 72; i++) {
     if(i <= leds){
       strips[strip].setPixelColor(i, r, g, b); 
       //Serial.println("collor");
       
     }
     else{
       strips[strip].setPixelColor(i, 0,0,0);
     }  
  }
}

void biDirectional(int r, int g, int b, int percent, int strip){
   
  int leds = 36 * (percent/100.0);
  strips[strip].setBrightness(255 * (percent/100.0));
  
  for(int i=0; i < 36; i++) {
     if(i <= leds){
       strips[strip].setPixelColor(36 + i, r, g, b);
       strips[strip].setPixelColor(35 - i, r, g, b);  
       
       
     }
     else{
       strips[strip].setPixelColor(36 + i, 0,0,0);
       strips[strip].setPixelColor(35 - i, 0,0,0);
     }  
  }
}

void setColor(int r, int g, int b, int percent, int strip, int mod){
  
  if(mod == 4)
    oneDirection(r, g, b, percent, strip);
  else if(mod == 5)
    biDirectional(r, g, b, percent, strip);
}

void setBeat(int r, int g, int b, int percent){
  beat.setBrightness(255 * (percent/100.0));
  for(int i=0; i < beatPixels; i++) {    
       beat.setPixelColor(i, r, g, b); 
  }
  beat.show();
}

int mod = 4;

void readData() {
  if (Serial.available() > 0) {
       //Serial.println("data");
       
         inByte = Serial.read();
         //Serial.println(inByte);
         if(inByte == 4 || inByte == 5){
           mod = inByte;
           //Serial.println("geting data");
            bytesread = 0;
            while(bytesread <= 36) {   
               //Serial.println(bytesread);           
              if( Serial.available() > 0) { 
                  inByte = Serial.read(); 
                  
                  code[bytesread] = inByte;  
                  //Serial.print('r');
                  //Serial.println(bytesread);              
                  bytesread++; 
                  if(bytesread >= 36 ) { 
                    break;                       
                  }                   
                } 
            } 
            //Serial.println(bytesread);
            if(bytesread == 36) { 
              
              setColor(code[0],code[1],code[2], code[3], 0, mod);
              setColor(code[4],code[5],code[6], code[7], 1, mod);
              setColor(code[8],code[9],code[10], code[11], 2, mod);
              setColor(code[12],code[13],code[14], code[15], 3, mod);
              setColor(code[16],code[17],code[18], code[19], 4, mod);
              setColor(code[20],code[21],code[22], code[23], 5, mod);
              setColor(code[24],code[25],code[26], code[27], 6, mod);
              setColor(code[28],code[29],code[30], code[31], 7, mod);
              setBeat(code[32],code[33],code[34], code[35]); 
              updateAll();
              Serial.flush();   
            }else{
              Serial.println("miss");
            } 
            bytesread = 0;
            Serial.flush();
            //timeLastCommand = millis();
       }
   }
}

void writeOut() {
}
