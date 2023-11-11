// количество датчиков для удобства
#define DS_SENSOR_AMOUNT 3

// создаём двухмерный массив с адресами
uint8_t addr[][8] = {
  {0x28, 0xFF, 0x64, 0x1, 0xB9, 0x5E, 0x19, 0x51},
  {0x28, 0xFF, 0x64, 0x1, 0xB2, 0x31, 0x80, 0x5F},
  {0x28, 0xFF, 0x64, 0x1, 0xB9, 0x5E, 0xE0, 0xB9},
};

#include <microDS18B20.h>
// указываем DS_ADDR_MODE для подключения блока адресации
// и создаём массив датчиков на пине D2
MicroDS18B20<4, DS_ADDR_MODE> sensor[DS_SENSOR_AMOUNT];

void setup() {
  Serial.begin(115200);
  // устанавливаем адреса
  for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
    sensor[i].setAddress(addr[i]);
  }
}

void loop() {
  // конструкция программного таймера на 1c
  static uint32_t tmr;
  if (millis() - tmr >= 1000) {
    tmr = millis();

    // выводим показания в порт
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
      Serial.print(sensor[i].getTemp());
      Serial.print(',');
    }
    Serial.println();

    // запрашиваем новые
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
      sensor[i].requestTemp();
    }
  }
}
