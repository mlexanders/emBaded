/*
  MASTER_TEST_2
*/

const int PIN = 4;

uint data = 0;

void setup() {
    Serial.begin(250000);
    pinMode(PIN, OUTPUT);
}

uint8_t inData[8] = {
  0,0,0,0,0,0,0,0
};

void loop() {
  if(!reset()) return;
  
  Serial.println("");

  for(int i = 7; i >= 0; i--){
    inData[i] = readBit();
    delayMicroseconds(3);
  }

  for(int i = 0; i < 8; i++){
    Serial.print(inData[i]);
  }
  
  Serial.println("");
  Serial.println("-----");
}

uint8_t readBit(){
  uint8_t r;

  noInterrupts();
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
  delayMicroseconds(5);

  pinMode(PIN, INPUT);
  delayMicroseconds(10);
  r = digitalRead(PIN);
  delayMicroseconds(53);

  interrupts();
  return r;
}

void DS18B20_Write(byte data)
{
  pinMode(PIN, OUTPUT);
  for (int i = 0; i < 8; i++)
  {
    // начало таймслота
    digitalWrite(PIN, LOW);
    delayMicroseconds(10);

    // если 1, то устанавлием на пине логическую 1
    if (data & 1) digitalWrite(PIN, HIGH);
    data >>= 1;
    // время на считывание устройством
    delayMicroseconds(48);

    // конец таймслота
    digitalWrite(PIN, HIGH);
    delayMicroseconds(3);
  }
}


bool reset() {
  pinMode(PIN, OUTPUT);

  digitalWrite(PIN, 0);
  delayMicroseconds(500); // 480 - 960

  bool status = false; 
  int counter = 240 * 3.47826;
  pinMode(PIN, INPUT);
  delayMicroseconds(15); // 15-60, на линии значение 1 от подтяжки

  while (counter > 0) { // ждем 0 от slave
    int result = !digitalRead(PIN);
    status = status | result; 
    counter--;
  }

  delayMicroseconds(200);
  return status;
}
