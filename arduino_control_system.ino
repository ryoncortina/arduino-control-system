const unsigned  long debounceDelay = 50; //Debounce delay for relays 1-4

//Auxillary Power
const int startBtnPin = 11;
const int stopBtnPin = 12;
const int estopSwitchPin = 10;

const int blueLedPin = 1;
const int greenLedPin = 0;

int startBtnState;
int stopBtnState;
int estopSwitchState;

bool systemRunning = false;


//RELAY 1
const int relay1Pin = 9;
const int relay1BtnPin = 5;
int relay1BtnState = HIGH;
int lastRelay1BtnState = HIGH;
bool relay1State = false;
//RELAY 1 DEBOUNCE
unsigned long relay1LastDebounceTime = 0;
int relay1Reading;

//RELAY 2
const int relay2Pin = 8;
const int relay2BtnPin = 4;
int relay2BtnState = HIGH;
int lastRelay2BtnState = HIGH;
bool relay2State = false;
//RELAY 2 DEBOUNCE
unsigned long relay2LastDebounceTime = 0;
int relay2Reading;

//RELAY 3
const int relay3Pin = 7;
const int relay3BtnPin = 3;
int relay3BtnState = HIGH;
int lastRelay3BtnState = HIGH;
bool relay3State = false;
//RELAY 3 DEBOUNCE
unsigned long relay3LastDebounceTime = 0;
int relay3Reading;

//RELAY 4 *MASTER PERMISSIVE*
const int masterPermissivePin = 6;
const int masterPermissiveBtnPin = 2;
int masterPermissiveBtnState = HIGH;
int lastMasterPermissiveBtnState = HIGH;
bool masterPermissive = false;
//RELAY 4 DEBOUNCE
unsigned long masterPermissiveLastDebounceTime = 0;
int masterPermissiveReading;


void setup() {

  Serial.begin(9600);

  //Auxillary Power
  pinMode(startBtnPin, INPUT_PULLUP);
  pinMode(stopBtnPin, INPUT_PULLUP);
  pinMode(estopSwitchPin, INPUT_PULLUP);

  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);


  //RELAY 1
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay1BtnPin, INPUT_PULLUP);


  //RELAY 2
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay2BtnPin, INPUT_PULLUP);

  //RELAY 3
  pinMode(relay3Pin, OUTPUT);
  pinMode(relay3BtnPin, INPUT_PULLUP);

  //RELAY 4
  pinMode(masterPermissivePin, OUTPUT);
  pinMode(masterPermissiveBtnPin, INPUT_PULLUP);

}

void loop() {

Serial.print("System: ");
Serial.print(systemRunning);

Serial.print("  Master: ");
Serial.print(masterPermissive);

Serial.print("  Relay1: ");
Serial.println(relay1State);

delay(100);

//----------------------------AUXILLARY----------------------------
  //Read Pin State
  startBtnState = digitalRead(startBtnPin);
  stopBtnState = digitalRead(stopBtnPin);
  estopSwitchState = digitalRead(estopSwitchPin);

  //System running permissives
  if(startBtnState == LOW && estopSwitchState == HIGH){
    systemRunning = true;
  }
  if(stopBtnState == LOW){
    systemRunning = false;
  }
  if(estopSwitchState == LOW){
    systemRunning = false;
  }

  //State output
  if(estopSwitchState == LOW){
    digitalWrite(blueLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  }
  else if(systemRunning == true){
    digitalWrite(greenLedPin, HIGH);
  }
  else{
    digitalWrite(blueLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
  }

//-----------------------------RELAY 1-----------------------------
  //Read Input
  relay1Reading = digitalRead(relay1BtnPin);
  //Debounce
  if(relay1Reading != lastRelay1BtnState){
    relay1LastDebounceTime = millis();
  }
  if((millis() - relay1LastDebounceTime) > debounceDelay){
    if(relay1Reading != relay1BtnState){
      relay1BtnState = relay1Reading;
      if(relay1BtnState == LOW && masterPermissive == true && systemRunning == true){
        relay1State = !relay1State;
      }
    }
  }
  //Control

  if(masterPermissive == false || systemRunning == false){ //Relay 1 master permissive
    relay1State = false;
  }
  if(relay1State == true){
    digitalWrite(relay1Pin, LOW);
  }
  else{digitalWrite(relay1Pin, HIGH);
  }
  lastRelay1BtnState = relay1Reading;

//-----------------------------RELAY 2-----------------------------
  //Read Input
  relay2Reading = digitalRead(relay2BtnPin);
  //Debounce
  if(relay2Reading != lastRelay2BtnState){
    relay2LastDebounceTime = millis();
  }
  if((millis() - relay2LastDebounceTime) > debounceDelay){
    if(relay2Reading != relay2BtnState){
      relay2BtnState = relay2Reading;
      if(relay2BtnState == LOW && masterPermissive == true && systemRunning == true){
        relay2State = !relay2State;
      }
    }
  }

  //Control
  if(masterPermissive == false || systemRunning == false){ //Relay 2 master permissive
    relay2State = false;
  }
  if(relay2State == true){
    digitalWrite(relay2Pin, LOW);
  }
  else{
    digitalWrite(relay2Pin, HIGH);
  }
  lastRelay2BtnState = relay2Reading;

//-----------------------------RELAY 3-----------------------------
  //Read Input
  relay3Reading = digitalRead(relay3BtnPin);
  //Debounce
  if(relay3Reading != lastRelay3BtnState){
    relay3LastDebounceTime = millis();
  }
  if((millis() - relay3LastDebounceTime) > debounceDelay){
    if(relay3Reading != relay3BtnState){
      relay3BtnState = relay3Reading;
      if(relay3BtnState == LOW && masterPermissive == true && systemRunning == true){
        relay3State = !relay3State;
      }
    }
  }
  //Control
  if(masterPermissive == false || systemRunning == false){ //Relay 3 master permissive
    relay3State = false;
  }
  if(relay3State == true){
    digitalWrite(relay3Pin, LOW);
  }
  else{
    digitalWrite(relay3Pin, HIGH);
  }
  lastRelay3BtnState = relay3Reading;

//-----------------------------RELAY 4-----------------------------
  //Read Input
  masterPermissiveReading = digitalRead(masterPermissiveBtnPin);

  //System Running permissive
  if(estopSwitchState == LOW){
    masterPermissive = false;
  }
  //Debounce
  if(masterPermissiveReading != lastMasterPermissiveBtnState){
    masterPermissiveLastDebounceTime = millis();
  }
  if((millis() - masterPermissiveLastDebounceTime) > debounceDelay){
    if(masterPermissiveReading != masterPermissiveBtnState){
      masterPermissiveBtnState = masterPermissiveReading;
      if(masterPermissiveBtnState == LOW && systemRunning == true && stopBtnState == HIGH && estopSwitchState == HIGH){
        masterPermissive = !masterPermissive;
      }
    }
  }
  //Control
  if(masterPermissive == true){
    digitalWrite(masterPermissivePin, LOW);
  }
  else{
    digitalWrite(masterPermissivePin, HIGH);
  }
  lastMasterPermissiveBtnState = masterPermissiveReading;
}
