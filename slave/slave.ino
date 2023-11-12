/*
  SLAVE
*/
const int PIN = 4;
const int DEBUG_PIN = 5;
const int DEBUG_DATA_PIN = 18;


void setup() {
  pinMode(DEBUG_PIN, OUTPUT);
  pinMode(DEBUG_DATA_PIN, OUTPUT);
  Serial.begin(250000);
}


static bool value = 1;
static bool a;
static bool b;
static byte data[2];

void loop() {
  if(!reset()) return;

  /*DEBUG*/
  value = !value;
  digitalWrite(DEBUG_PIN, value);
  /*DEBUG*/

  data[0] = read();
  data[1]  = read();
  // debugWrite(data);
  // if(cmd == 0xCC){
  //   a = true;
  // }
  // if(data == 0x44){
  //   b = true;
  // }
  // if(a & b){
  //   digitalWrite(DEBUG_DATA_PIN, 1);
  //   while(true){
  //   }
  // }
  Serial.println(data[0], HEX);
  Serial.println(data[1], HEX);
}

void debugWrite(byte data){
  for(int i = 0; i < 8; i++){
    delayMicroseconds(10);

    digitalWrite(DEBUG_DATA_PIN, data << i);
    delayMicroseconds(10);

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


void write(byte data) {
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

byte read() {
  pinMode(PIN, INPUT);
  byte data = 0;
  while(digitalRead(PIN));
  for (int i = 0; i < 8; i++) {
    pinMode(PIN, INPUT);
    delayMicroseconds(5);
    data >>= 1;
    if (digitalRead(PIN)) data |= 0x80;
    delayMicroseconds(25);
    while(!digitalRead(PIN));
  }
  return data;
}

