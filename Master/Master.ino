/*
  MASTER
*/

const int PIN = 4;


void setup() {
  Serial.begin(115200);
}

void loop() {
  // double temp = TempRead();
  // temp  = temp * 0.0625; // conversion accuracy is 0.0625 / LSB
  // Serial.print("Temperature: ");
  // Serial.print(temp);
  // Serial.println(" °C");
  // Serial.println("");


  // uint8_t address[8];
  // AddrRead(address);

  Serial.println(reset());
  delay(100);
}

// int readByte() {
//   int data = 0;
//   for (int i = 0; i < 8; i++) {
//     data = readBit() << i;
//   }
// }

// int readBit() {
//   pinMode(PIN, OUTPUT);
//   digitalWrite(PIN, 0);
//   delayMicroseconds(2);

//   pinMode(PIN, INPUT);
//   delayMicroseconds(8);

//   int data = digitalRead(PIN);
//   delayMicroseconds(60);
//   return data;
// }


bool reset() {
  pinMode(PIN, OUTPUT);

  digitalWrite(PIN, 0);
  delayMicroseconds(500); // 480 - 960

  bool status = false; 
  int counter = 240 * 3.3;
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

void DS18B20_Write(byte data) {
  pinMode(PIN, OUTPUT);
  for (int i = 0; i < 8; i++) {
    digitalWrite(PIN, LOW);
    delayMicroseconds(10);
    if (data & 1) digitalWrite(PIN, HIGH);
    else digitalWrite(PIN, LOW);
    data >>= 1;
    delayMicroseconds(50);
    digitalWrite(PIN, HIGH);
  }
}

byte DS18B20_Read() {
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  delayMicroseconds(2);
  byte data = 0;
  for (int i = 0; i < 8; i++) {
    digitalWrite(PIN, LOW);
    delayMicroseconds(1);
    digitalWrite(PIN, HIGH);
    pinMode(PIN, INPUT);
    delayMicroseconds(5);
    data >>= 1;
    if (digitalRead(PIN)) data |= 0x80;
    delayMicroseconds(55);
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, HIGH);
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
