/*
  SLAVE_TEST_2
*/
const int PIN = 4;

void setup() {
  Serial.begin(250000);
}

uint data = 170;

void loop() {
  if(!reset()) return;

  for(int i = 0; i < 8; i++){
    writeBit((data >> i) & 1);
  }
}

void writeBit(uint8_t bit){
  noInterrupts();

  pinMode(PIN, INPUT);
  while(digitalRead(PIN));

  pinMode(PIN, OUTPUT);
  delayMicroseconds(2); //2
  if(bit & 1) digitalWrite(PIN, HIGH);
  else digitalWrite(PIN, LOW);

  delayMicroseconds(53);
  pinMode(PIN, INPUT);

  interrupts();
}

byte read() {
  pinMode(PIN, INPUT);
  byte data = 0;

  for (int i = 0; i < 8; i++) {
    //ожидаем начало таймслота
    while(digitalRead(PIN));

    // сдвиг вправо 
    data >>= 1;
    delayMicroseconds(15);

    // если на пине логическая 1, то ставим маску 1000 0000 на data
    if (digitalRead(PIN)) data |= 0x80;
    delayMicroseconds(10);

    //ожидаем конца таймслота
    while(!digitalRead(PIN));
  }
  return data;
}


bool reset() {
  pinMode(PIN, INPUT);
  if(digitalRead(PIN)) return false;

  delayMicroseconds(120); //120-480

  if(digitalRead(PIN)) return false;

  bool state = false;
  pinMode(PIN, INPUT);

  
  int microseconds = 60 * 4; // 15-60
  int counter = (microseconds * 3.47826); 
  // ждем 1 от подтяжки
  while(!state && counter > 0){
    state = digitalRead(PIN);
    counter--;
  }
  if(!state) return false; // если не дождались 1
  delayMicroseconds(30); // 15-60

  pinMode(PIN, OUTPUT);

  digitalWrite(PIN, 0);
  delayMicroseconds(190); // 60 - 240
  
  pinMode(PIN, INPUT);
  delayMicroseconds(0);
  return state;
}
