/*
  SLAVE
*/
const int PIN = 4;


void setup() {
  // Serial.begin(115200);
}

void loop() {
  reset();
}



bool reset() {
  pinMode(PIN, INPUT);
  if(digitalRead(PIN)) return false;

  delayMicroseconds(120); //120-480

  if(digitalRead(PIN)) return false;

  bool state = false;
  pinMode(PIN, INPUT);

  // ждем 1 от подтяжки
  while(!state){
    state = digitalRead(PIN);
  }
  delayMicroseconds(30); // 15-60

  pinMode(PIN, OUTPUT);

  digitalWrite(PIN, 0);
  delayMicroseconds(190); // 60 - 240
  
  pinMode(PIN, INPUT);
  delayMicroseconds(0);
  return state;
}
