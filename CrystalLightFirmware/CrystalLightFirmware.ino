//Crystal light firmware
#define dataPin  2  // пин подключен к входу DS
#define latchPin 3  // пин подключен к входу ST_CP
#define clockPin 4  // пин подключен к входу SH_CP

byte data0 = 0x32;
byte data1 = 0x01;

void setup() {
  // put your setup code here, to run once:
  // устанавливаем режим работы пинов OUTPUT
   pinMode(latchPin, OUTPUT);
   pinMode(clockPin, OUTPUT);
   pinMode(dataPin, OUTPUT);
   // ставим HIGH на "защёлку", чтобы регистр не принимал сигнал
   digitalWrite(latchPin, HIGH);  

}

void loop() {
  // digitalWrite(8, HIGH);
  // delay(500);
  // digitalWrite(8, LOW);
  // delay(500);
  // put your main code here, to run repeatedly:
  digitalWrite(latchPin, LOW);                                        // ставим LOW на "защёлку"
  shiftOut(dataPin, clockPin, MSBFIRST, data1); // отправляем байт в двоичном виде
  shiftOut(dataPin, clockPin, MSBFIRST, data0); // отправляем байт в двоичном виде
  digitalWrite(latchPin, HIGH);                                       // ставим HIGH на "защёлку"


}
