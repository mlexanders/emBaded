// пример работы с двумя датчиками без адресации
// один датчик - один пин

#include <microDS18B20.h>
// Датчики на D2 и D3
MicroDS18B20<4> sensor1;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // запрос температуры
  sensor1.requestTemp();
  // вместо delay используй таймер на millis(), пример async_read
  delay(1000);

  // ПЕРВЫЙ ДАТЧИК
  Serial.print("t1: ");
  
  // просто выводим температуру первого датчика
  Serial.println(sensor1.getTemp());
}
