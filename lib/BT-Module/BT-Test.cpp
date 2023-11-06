#include <Arduino.h>
#include <BT4502.cpp>
#define USB_SERIAL Serial


void setup() {
  // initialize both serial ports:
  USB_SERIAL.begin(9600);

  if (BTinit()){ USB_SERIAL.println("Bluetooth setup succesfully"); }
  else{ USB_SERIAL.println("An issue occured with Bluetooth Setup"); }
}

void loop()
{
  char name[25]; 
  getName(name);
  USB_SERIAL.print(name);
  delay(1000);
}
