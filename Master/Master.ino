/*
  MASTER
*/

const int PIN = 4;


void setup() {
  Serial.begin(250000);
}

void loop() {
  double temp = TempRead();
  temp  = temp * 0.0625; // conversion accuracy is 0.0625 / LSB
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.println("");

  // uint8_t address[8];
  // AddrRead(address);

  /*DEBUG*/
  // if(!reset()) return;
  // byte data = B11110000;
  // DS18B20_Write(data);

  // if (!reset()) return;
  // // DS18B20_Write(0xCC);          
  // DS18B20_Write(0xBE);         
  // int temp1 = DS18B20_Read();    // Low byte
  // // int temp2 = DS18B20_Read();  // High byte
  // Serial.println(temp1, HEX);
  // // Serial.println(temp2, HEX);

  if(!reset()) return;

  int data = 0;

    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);
    delayMicroseconds(5);
    pinMode(PIN, INPUT);
    delayMicroseconds(10);
    data >>= 1;
    pinMode(PIN, INPUT);
    if (digitalRead(PIN)) data |= 0x80;
    delayMicroseconds(45);
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, HIGH);
    delayMicroseconds(5);

  delay(1100);
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

byte DS18B20_Read()
{
  byte data = 0;
  for (int i = 0; i < 8; i++)
  {
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);
    delayMicroseconds(5);
    pinMode(PIN, INPUT);
    delayMicroseconds(10);
    data >>= 1;
    pinMode(PIN, INPUT);
    if (digitalRead(PIN)) data |= 0x80;
    delayMicroseconds(45);
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, HIGH);
    delayMicroseconds(5);
  }
  return data;
}

int TempRead() {
  if (!reset()) return 0;
  DS18B20_Write(0xCC);  // Send skip ROM command / пропуск ROM
  DS18B20_Write(0x44);  // Send reading start conversion command / Температурного преобразования [44h]
  if (!reset()) return 0;
  DS18B20_Write(0xCC);          // Send skip ROM command
  DS18B20_Write(0xBE);          // Read the register, a total of nine bytes, the first two bytes are the conversion value
  int temp = DS18B20_Read();    // Low byte
  temp |= DS18B20_Read() << 8;  // High byte
  return temp;
}

void AddrRead(uint8_t *addr) {
  if (!reset()) return;

  uint16_t sum = 0;                  // контрольная сумма
  uint8_t crc = 0;                   // обнуляем crc
  for (uint8_t i = 0; i < 8; i++) {  // Прочитать 8 байт адреса
    addr[i] = DS18B20_Read();        // Записать в массив
    //sum += addr[i];                     // контрольная сумма
  }
}
