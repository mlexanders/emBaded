/*
  MASTER_TEST_2
*/

const int PIN = 4;

uint data = 0;

void setup() {
    Serial.begin(115200);
}

// uint8_t inData[8] = {
//   0,0,0,0,0,0,0,0
// };
uint8_t inData = 0;

void loop() {
  if(!reset()) return;
  
  DS18B20_Write(0xCC); 
  DS18B20_Write(0x44); 
  // Serial.println("");

  if (!reset()) return;
  DS18B20_Write(0xCC);          // Send skip ROM command
  DS18B20_Write(0xBE);          // Read the register, a total of nine bytes, the first two bytes are the conversion value
    
  Serial.println(read(), BIN);
  Serial.println(read(), BIN);
  Serial.println(" ---------");
  delay(50);
}

uint8_t read(){
  uint8_t data = 0;
  for(int i = 0; i < 8; i++){
    data = data | readBit() << i;
    delayMicroseconds(3); // added
  };
  return data;
}

int getTemp(){
    if (!reset()) return -1;
    DS18B20_Write(0xCC);  // Send skip ROM command / пропуск ROM
    DS18B20_Write(0x44);  // Send reading start conversion command / Температурного преобразования [44h]
    
    delay(10); // 750

    if (!reset()) return -1;
    DS18B20_Write(0xCC);          // Send skip ROM command
    DS18B20_Write(0xBE);          // Read the register, a total of nine bytes, the first two bytes are the conversion value
    
    uint16_t temp = read();    // Low byte
    temp |= read() << 8;  // High byte
    return temp; // conversion accuracy is 0.0625 / LSB
    // return temp * 0.0625; // conversion accuracy is 0.0625 / LSB
}

uint8_t readBit(){
  uint8_t r;

  noInterrupts();
  
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
  delayMicroseconds(5); // change 3 ->> 5

  pinMode(PIN, INPUT);
  delayMicroseconds(10);
  r = digitalRead(PIN);
  delayMicroseconds(53);

  interrupts();
  return r;
}

void DS18B20_Write(byte data)
{
  noInterrupts();

  pinMode(PIN, OUTPUT);
  for (int i = 0; i < 8; i++)
  {
    // начало таймслота
    digitalWrite(PIN, LOW);
    delayMicroseconds(10); //10

    // если 1, то устанавлием на пине логическую 1
    if (data & 1) digitalWrite(PIN, HIGH);
    data >>= 1;
    // время на считывание устройством
    delayMicroseconds(55);

    // конец таймслота
    digitalWrite(PIN, HIGH);
    delayMicroseconds(3);

    interrupts();
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
