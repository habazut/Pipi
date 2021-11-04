#include <EEPROM.h>

static int sensorPin = A0;
static int outPin=13;
static int sensorLimit=400;
int once = 0;
int eepromval;
int eepromaddr=0;
int sensorValue;
int oldSensorValue;
int counter=0;
int storagefactor=2; // we can save vals 1 to 255 and as sensorLimit is 400 we scale vals to half.

void setup() {
  Serial.begin(115200);
  for (eepromaddr = 0; eepromaddr < 1024; eepromaddr++) {
    eepromval = EEPROM.read(eepromaddr);
    if (0)
      EEPROM.write(eepromaddr,0);
    else {
      if (eepromval == 0)
	break;
    }
    Serial.print("EEPROM ");
    Serial.print(eepromaddr);
    Serial.print(" = ");
    Serial.println(eepromval*storagefactor);
  } 
  Serial.print("EEPROM ADDR: ");
  Serial.println(eepromaddr);
  Serial.println(sensorLimit);
  pinMode(outPin, OUTPUT);
  digitalWrite(outPin, LOW);
}



void loop() {  
  sensorValue = analogRead(sensorPin);

  if (sensorValue != oldSensorValue) {
    oldSensorValue = sensorValue;
    Serial.print("Sensor: ");
    Serial.println(sensorValue);
  }
  
  if (sensorValue < sensorLimit) {
    counter++;
    if (counter > 20) {
      if (once++ == 0) {
	if (sensorValue < storagefactor)
	  sensorValue = storagefactor; // maps to 1
	EEPROM.write(eepromaddr,sensorValue/storagefactor);
      }
      digitalWrite(outPin, HIGH);
    }
  } else {
    counter=0;
    digitalWrite(outPin, LOW);
  }
   
  delay(1000);
}
