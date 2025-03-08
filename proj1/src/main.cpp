#include <Arduino.h>
#include "Adafruit_VL53L0X.h" //libary for distsensor
#include "LiquidCrystal_I2C.h" //library for controling the LCD display

//global constants
#define LED_PIN 32
#define SDA_PIN 21
#define SCL_PIN 23
#define ADDRESS_DISPLAY 0x27
//ADDRESS_SENSOR default is 0x29

//scrapped line for tuning sensor
//#define SENSOR_READING_OFFSET 0;

//global variables
Adafruit_VL53L0X distSensor = Adafruit_VL53L0X();
LiquidCrystal_I2C display(ADDRESS_DISPLAY, 16, 2);

//function declarations
void ERROR();

void setup() {
  //setup pins
  pinMode(LED_PIN, OUTPUT);

  //INIT DISTANCE SENSOR
  //begin returns 0 if sensor did not initalize properly
  if (!distSensor.begin())
  {
    ERROR();    
  }
  //wait for distance sensor to init
  delay(500);

  //INIT DISPLAY
  display.init();
  display.backlight();
  //wait for display to initalize, clear the display and start cursor at begining.
  delay(500);
  display.clear();
  display.setCursor(0,0);  
}

void loop() {
  VL53L0X_RangingMeasurementData_t dist;
  bool ledStatus = false;
  //get sensor distance reading
  distSensor.rangingTest(&dist, false);
 
  //scrapped line for tuning sensor.
  //dist.RangeMilliMeter += SENSOR_READING_OFFSET;


  //reset from last reading
  display.clear();
  display.setCursor(0,0);  

  //display only distances 0.1 m <= distances <= 1.2 m  with +- 10%
  //this means display readings dist of 90 mm <= readings <= 1080 mm    
  
  
  if (dist.RangeMilliMeter >= 90 && dist.RangeMilliMeter <= 1360){
    display.print("Distance:");
    display.setCursor(0, 1);
    display.print(dist.RangeMilliMeter);
    display.print(" mm");

    //light up led only distances 0.5 m <= distances with +- 10%
    //this means led lights up for readings dist of 550 mm <= readings
    if (dist.RangeMilliMeter <= 550 ){
      //object is there
      ledStatus = true;
    }
    else{
      //object is removed aka not there
      ledStatus = false;
    }
    
  }
  else{
    display.print("Out of Range");
    ledStatus = false;
  }
  //update pin based on LED status
  digitalWrite(LED_PIN, ledStatus);
  //wait one second to take another reading
  delay(1000);
}

//for debugging, indicate an error by flashing led
void ERROR(){
  while (1){
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    digitalWrite(LED_PIN, LOW);
    delay(250);
  }
}