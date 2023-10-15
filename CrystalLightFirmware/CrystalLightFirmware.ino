//Crystal light firmware
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#if ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) || defined(ARDUINO_AVR_FEATHER328P) || \
        defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || defined(ARDUINO_AVR_PROTRINKET5FTDI) || \
        defined(ARDUINO_AVR_PROTRINKET3FTDI) )
  #define USE_TIMER_1     true
  #warning Using Timer1
#else          
  #define USE_TIMER_3     true
  #warning Using Timer3
#endif

#include "TimerInterrupt.h"

#define dataPin       9  // пин подключен к входу DS (14)
#define latchPin      10  // пин подключен к входу ST_CP (12)
#define clockPin      11  // пин подключен к входу SH_CP (11)
#define interruptPin  2
#define brightness0   6
#define brightness1   5
#define brightness2   3


#define USE_TIMER_1     true
#define TIMER_INTERVAL_MS    1
#define LED_TIMER_INTERVAL_MS  500
#define BRIGHT_TIMER_INTERVAL_MS  10

int ledTimer = 0;
int brightTimer = 0;

byte data0 = 0x32;
byte data1 = 0x01;
byte colors[] = { 
    //0x00, //off
    0x01, //red
    0x02, //green
    0x03, //yellow
    0x04, //blue
    0x05, //magenta
    0x06, //cyan
    0x07  //white
  };

byte colorIndices[] = { 1, 3, 5 };
int brightnessValues[] = { 0, 173, 169 };
int brightnessChangeDirection[] = { 0, 0, 1 };
int sinusBrightness[] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 13, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 22, 23, 24, 25, 26, 26, 27, 28, 29, 30, 31, 32, 34, 35, 36, 37, 38, 39, 40, 41, 42, 44, 45, 46, 47, 48, 50, 51, 52, 53, 55, 56, 57, 59, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 80, 81, 83, 84, 85, 87, 88, 90, 91, 93, 94, 96, 97, 99, 101, 102, 104, 105, 107, 108, 110, 111, 113, 114, 116, 118, 119, 121, 122, 124, 125, 127, 127, 129, 130, 132, 133, 135, 136, 138, 140, 141, 143, 144, 146, 147, 149, 150, 152, 153, 155, 157, 158, 160, 161, 163, 164, 166, 167, 169, 170, 171, 173, 174, 176, 177, 179, 180, 182, 183, 184, 186, 187, 189, 190, 191, 193, 194, 195, 197, 198, 199, 201, 202, 203, 204, 206, 207, 208, 209, 210, 212, 213, 214, 215, 216, 217, 218, 219, 220, 222, 223, 224, 225, 226, 227, 228, 228, 229, 230, 231, 232, 233, 234, 235, 236, 236, 237, 238, 239, 239, 240, 241, 241, 242, 243, 243, 244, 245, 245, 246, 246, 247, 247, 248, 248, 249, 249, 250, 250, 250, 251, 251, 251, 252, 252, 252, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 255, 255, 255 };

void TimerHandler(void)
{
  ledTimer++;
  if (ledTimer == LED_TIMER_INTERVAL_MS){
    LedTimerTick();
    ledTimer = 0;
  }
  brightTimer++;
  if (brightTimer == BRIGHT_TIMER_INTERVAL_MS){
    BrightTimerTick();
    brightTimer = 0;
  }

  // Serial.println(brightnessValues[0]);
}

void LedTimerTick(void){
  //Colors cycle
  // for (byte i = 0; i < 3; i++){
  //   ShiftColor(i);
  // };
}

void ShiftColor(int i){  
  colorIndices[i]++;
  if(colorIndices[i] > 6) colorIndices[i] = 0;
  setColor(i, colorIndices[i]);
}

void RandomColor(int i){
  colorIndices[i] == random(6);
  setColor(i, colorIndices[i]);
}

void BrightTimerTick(void){
  //Brightness cycle
  // Serial.println(brightnessValues[0]);
  for (byte i = 0; i < 3; i++){
    if (brightnessChangeDirection[i] == 0){
      brightnessValues[i]++;
      if (brightnessValues[i] == 255){
        brightnessChangeDirection[i] = 1;
        ShiftColor(i);
      }
    }
    else{
      brightnessValues[i]--;
      if (brightnessValues[i] == 0){
        brightnessChangeDirection[i] = 0;        
      }
    }
  }
}

void setup() {
  Serial.begin(9600);

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  // randomSeed(analogRead(0));

  // устанавливаем режим работы пинов
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(brightness0, OUTPUT);
  pinMode(brightness1, OUTPUT);
  pinMode(brightness2, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);

  ITimer1.init();

  ITimer1.attachInterruptInterval(TIMER_INTERVAL_MS, TimerHandler);

  attachInterrupt(digitalPinToInterrupt(interruptPin), button, FALLING);

  // ставим HIGH на "защёлку", чтобы регистр не принимал сигнал
  digitalWrite(latchPin, HIGH);

  for (byte i = 0; i < 3; i++){
    setColor(i, colorIndices[i]);
  };

}

void loop() {

  displayColors(); 
  // delay(500);
}

void setColor(byte ledIndex, byte colorIndex){
  byte color = colors[colorIndex];
  switch (ledIndex){
    case 0:
      data0 = data0 & 0xF0;
      data0 = data0 | color;
      break;
    case 1:
      data0 = data0 & 0x0F;
      color = color << 4;
      data0 = data0 | color;
      break;
    case 2:
      data1 = data1 & 0xF0;
      data1 = data1 | color;
      break;
  }
}

void displayColors(){
  
  //Brightness from 255 - off to 0 - max brightness
  analogWrite(brightness0, sinusBrightness[brightnessValues[0]]);
  analogWrite(brightness1, sinusBrightness[brightnessValues[1]]);
  analogWrite(brightness2, sinusBrightness[brightnessValues[2]]);

  digitalWrite(latchPin, LOW);                                        // ставим LOW на "защёлку"
  shiftOut(dataPin, clockPin, MSBFIRST, data1); // отправляем байт в двоичном виде
  shiftOut(dataPin, clockPin, MSBFIRST, data0); // отправляем байт в двоичном виде
  digitalWrite(latchPin, HIGH);                                       // ставим HIGH на "защёлку"
}

void button(){
  // Serial.print(0);
}