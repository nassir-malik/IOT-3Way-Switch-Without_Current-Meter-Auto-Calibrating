#include <EEPROM.h>
extern int current_sensor_pin;
float amplitude_current; //amplitude current
float effective_value; //effective current
int sensor_max;
extern int deviceState;
extern int device_on_current_threshold;

void eepromWrite(int buffLocation, String ids);
int eepromRead(int buffLocation );

void pins_init()
{
pinMode(current_sensor_pin, INPUT);
}
/*Function: Sample for 1000ms and get the maximum value from the SIG pin*/
int getMaxValue()
{
int sensorValue; //value read from the sensor
int sensorMax = 0;
uint32_t start_time = millis();
while((millis()-start_time) < 1000)//sample for 1000ms
{
sensorValue = analogRead(current_sensor_pin);
if (sensorValue > sensorMax)
{
/*record the maximum sensor value*/
sensorMax = sensorValue;
}
}
return sensorMax;
}

void currentMeter()
{

sensor_max = getMaxValue();
//amplitude_current=(float)sensor_max/1024*5/200*1000000;
//effective_value=amplitude_current/1.414;
//Serial.println("The effective value of the current is(in mA)");
//Serial.println(effective_value,1);
Serial.println("The effective value of the current is(in mA)");
Serial.println(sensor_max);
effective_value=sensor_max;
      if(effective_value>device_on_current_threshold){
        Serial.println("Switch was on");
        deviceState = 1 ;
      }else{
        Serial.println("Switch was off");
        deviceState = 0 ;
      }
}

void eepromWrite(String ids)
{
     for (int i = 0; i < 500; i++){
      EEPROM.write(i, 0);
   }
  const char* id;
  id= ids.c_str();
   for (int i = 0; i < 500; i++){ //clear buffer before write
      EEPROM.write(i, 0);
   }
  size_t i; 

  for(i=0; i<=strlen(id); i++) //write value to eeprom
  {
    EEPROM.write(i, (uint8_t) id[i]); 
  }
  EEPROM.commit();
}

int eepromRead( ){
  Serial.println("Reading EEPROM");
  String doorStatus;
  for (int i = 0; i < 500; ++i)
    {
      doorStatus += char(EEPROM.read(i));
    }
    //Serial.println(doorStatus.length());
    Serial.print("Door Status: ");
    Serial.println(doorStatus);
    return doorStatus.toInt();
}
void calibrateOnCurrent(){
  int vvl =getMaxValue();
  Serial.println("Saving Calibrating current...");
  Serial.println(vvl);
  eepromWrite(String(vvl-5));
  device_on_current_threshold = eepromRead();
  Serial.println("Reading Calibrating current...");
  Serial.println(device_on_current_threshold);
}

