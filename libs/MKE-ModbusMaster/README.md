Modbus Master 
======

This Arduino library created for ModbusMaster operations

To download, click the ZIP button above. Extract the zip file and  rename the uncompressed folder to MKE-ModbusMaster-master. Check that it contains  MKEModbusMaster.cpp and MKEModbusMaster.h, keywords.txt.

Place the BH1750 library folder your &lt;arduinosketchfolder&gt;/libraries/ folder. Restart the IDE.


Example usage:
```c
/*

Example of MKE-ModbusMaster library usage.

This example initalises the BH1750 object using the default
high resolution mode and then makes a light level reading every second.

Connection:
 VCC-5v
 GND-GND
 SCL-SCL(analog pin 5)
 SDA-SDA(analog pin 4)
 ADD-NC or GND

*/

#include <Wire.h>
#include <BH1750.h>


BH1750 lightMeter;


void setup(){
  Serial.begin(9600);
  lightMeter.begin();
  Serial.println("Running...");
}


void loop() {
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
}
```
