#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <avr/io.h>
#include <avr/interrupt.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

int i = 0;
int BNO_Error = 0;

IntervalTimer controlTimer;
elapsedMillis freeCPU;
elapsedMillis usedCPU;
int lastUsed = 0;



void setup(void) 
{
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    BNO_Error = 1;
  }

  //Let the BNO get settled
  delay(1000);
    
  bno.setExtCrystalUse(true);

  //Open the I2C interface to the Pi
  Wire.begin(I2C_MASTER,0,I2C_PINS_18_19,I2C_PULLUP_INT,I2C_RATE_1000);

  Wire.beginTransmission(51);
  Wire.print("Booting.");
  Wire.endTransmission();

  analogReadResolution(14);

  //Setup the 100Hz control loop timer
  controlTimer.begin(ControlLoop, 10000);
}

void loop(void) 
{
}

void ControlLoop(void)
{
  Wire.beginTransmission(51);

  Wire.print("CPU: ");
  Wire.print(lastUsed);
  Wire.print("/");
  Wire.print(lastUsed+freeCPU);
  
  usedCPU = 0;
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);

  Wire.print(" Seq: ");
  Wire.print(String(i));
  Wire.print(" X: ");
  Wire.print(String(event.orientation.x));
  Wire.print(" Y: ");
  Wire.print(String(event.orientation.y));
  Wire.print(" Z: ");
  Wire.print(String(event.orientation.z));

  int ana0 = analogRead(0);
  int ana1 = analogRead(1);
  int ana2 = analogRead(2);
  int ana3 = analogRead(3);
  int ana6 = analogRead(6);
  int ana7 = analogRead(7);
  int ana8 = analogRead(8);
  int ana9 = analogRead(9);
  int ana10 = analogRead(10);
  int ana11 = analogRead(11);
  int ana12 = analogRead(12);

  Wire.print(" A0: ");
  Wire.print(String((double)ana0/4964.8484849));
  Wire.print("V");

  Wire.print(" A1: ");
  Wire.print(String((double)ana1/4964.8484849));

  Wire.print(" A2: ");
  Wire.print(String(ana2));

  Wire.print(" A3: ");
  Wire.print(String(ana3));

  Wire.print(" A6: ");
  Wire.print(String(ana6));

  Wire.print(" A7: ");
  Wire.print(String(ana7));

  Wire.print(" A8: ");
  Wire.print(String(ana8));

  Wire.print(" A9: ");
  Wire.print(String(ana9));

  Wire.print(" A10: ");
  Wire.print(String(ana10));

  Wire.print(" A11: ");
  Wire.print(String(ana11));

  Wire.print(" A12: ");
  Wire.print(String(ana12));

  Wire.print("\n");
 
  Wire.endTransmission();

  lastUsed = usedCPU;
  i++;
  freeCPU = 0;
}

