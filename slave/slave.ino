/*
  SLAVE
*/
const int PIN = 4;
const int DEBUG_PIN = 5;



void setup() {
  pinMode(DEBUG_PIN, OUTPUT);
}


static bool value = 1;

void loop() {
  if(reset()){
    value = !value;
     digitalWrite(DEBUG_PIN, value);
  }
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
