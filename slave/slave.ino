/*
  SLAVE
*/
const int PIN = 4;
// const int DEBUG_PIN = 5;
// const int DEBUG_DATA_PIN = 18;


void setup() {
  // pinMode(DEBUG_PIN, OUTPUT);
  // pinMode(DEBUG_DATA_PIN, OUTPUT);
  // Serial.begin(250000);
}


// static bool value = 1;
static byte data[2];

void loop() {
  if(!reset()) return;

   //ожидаем начало таймслота
    pinMode(PIN, INPUT);
    while(digitalRead(PIN)){
      delayMicroseconds(1);
    }

    // если 1, то устанавлием на пине логическую 1
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, HIGH);

    // if (data & 1) digitalWrite(PIN, HIGH);k
    // else digitalWrite(PIN, LOW);

    // время на считывание устройством
    delayMicroseconds(25);

  // data[0] = read();
  // data[1]  = read();
 
  // Serial.println(data[0], HEX);
  // Serial.println(data[1], HEX);

  /*DEBUG*/
  // value = !value;
  // digitalWrite(DEBUG_PIN, value);
  /*DEBUG*/

  // if(cmd == 0xCC){
  // }
  // if(data == 0x44){
  // }
  // if(data[1] == 0xBE){ 
    //TODO: // 
      // writeDS(0xCC); //LOW byte
      // writeDS(0xCC); //HIGH byte
  // }
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

void writeDS(byte data) {

  for (int i = 0; i < 8; i++) {
    //ожидаем начало таймслота
    pinMode(PIN, INPUT);
    while(digitalRead(PIN)){
      delayMicroseconds(1);
    }

    // если 1, то устанавлием на пине логическую 1
    pinMode(PIN, OUTPUT);
    if (data & 1) digitalWrite(PIN, HIGH);
    else digitalWrite(PIN, LOW);
    data >>= 1;

    // время на считывание устройством
    delayMicroseconds(25);
  }
}

// void debugWrite(byte data){
//   for(int i = 0; i < 8; i++){
//     delayMicroseconds(10);

//     digitalWrite(DEBUG_DATA_PIN, data << i);
//     delayMicroseconds(10);

//     value = !value;
//     digitalWrite(DEBUG_PIN, value);
//   }
// }



