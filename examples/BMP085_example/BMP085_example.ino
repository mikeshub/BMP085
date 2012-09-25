#include <I2C.h>
#include <BMP085.h>
#include <digitalWriteFast.h>

BMP085 baro;

short temperature;
long pressure;

void setup(){
  Serial.begin(115200);
  I2c.begin();
  I2c.setSpeed(1);
  baro.init();


}

void loop(){
  baro.PollPressure();
  if (baro.newData == true){
    temperature = baro.vars.temperature;
    pressure = baro.vars.pressure;
    Serial.print("Temperature: ");
    Serial.print(temperature, DEC);
    Serial.println(" *0.1 deg C");
    Serial.print("Pressure: ");
    Serial.print(pressure, DEC);
    Serial.println(" Pa");
    Serial.println();
    baro.newData = false;
  }
}



