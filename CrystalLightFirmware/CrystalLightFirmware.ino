//Crystal light firmware
#define dataPin  9  // пин подключен к входу DS (14)
#define latchPin 10  // пин подключен к входу ST_CP (12)
#define clockPin 11  // пин подключен к входу SH_CP (11)
#define interruptPin 2

byte data0 = 0x32;
byte data1 = 0x01;
byte colors[] = { 
    0x00, //off
    0x01, //red
    0x02, //green
    0x03, //yellow
    0x04, //blue
    0x05, //magenta
    0x06, //cyan
    0x07  //white
  };

byte colorIndices[] = { 0, 1, 2 };

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  // устанавливаем режим работы пинов OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin), button, FALLING);

   // ставим HIGH на "защёлку", чтобы регистр не принимал сигнал
   digitalWrite(latchPin, HIGH); 

  //Bring the darkness
  setColor(0, 0);
  setColor(1, 0);
  setColor(2, 0);
  displayColors();

  

  Serial.println("Started!");

}

void loop() {
  // put your main code here, to run repeatedly:

  for (byte i = 0; i < 3; i++){
    setColor(i, colorIndices[i]);
    colorIndices[i]++;
    if(colorIndices[i] > 7) colorIndices[i] = 0;
  };

  displayColors(); 
  delay(500);
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
  digitalWrite(latchPin, LOW);                                        // ставим LOW на "защёлку"
  shiftOut(dataPin, clockPin, MSBFIRST, data1); // отправляем байт в двоичном виде
  shiftOut(dataPin, clockPin, MSBFIRST, data0); // отправляем байт в двоичном виде
  digitalWrite(latchPin, HIGH);                                       // ставим HIGH на "защёлку"
}

void button(){
  Serial.print(0);
}