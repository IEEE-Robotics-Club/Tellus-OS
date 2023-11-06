#include <Arduino.h>
#include <BT4502.cpp>
#define USB_SERIAL Serial


void setup() {
  // initialize both serial ports:
  USB_SERIAL.begin(9600);
  if (BTinit()){ USB_SERIAL.println("Bluetooth setup succesfully"); }
  else{ USB_SERIAL.println("An issue occured with Bluetooth Setup"); }
}

char api_buffer[256];
int buffer_index = 0;
bool rx_flag = false;

void loop()
{
    while(BT_SERIAL.available()){
        api_buffer[buffer_index] = (char)BT_SERIAL.read();
        buffer_index++;
        rx_flag = true;
    } 

    if (rx_flag){
        for (int i = 0; i < buffer_index; i++){
            if (api_buffer[i] != '\n'){
                api_controller(buffer_index);
            }
        }
        //Finished processing the rx signal
        rx_flag = false; 
    }
}

void api_controller(char);
void api_controller(char input){
    switch(input){
        case 'f':
            //Move Forward
            USB_SERIAL.println("Moving Forward");
            break;
        case 'b':
            //Move Backward
            USB_SERIAL.println("Moving Backward");
            break;
        case 'l':
            //Turn Left
            USB_SERIAL.println("Turning Left");
            break;
        case 'r':
            //Turn Right
            USB_SERIAL.println("Turning Right");
            break;
        case 's':
            //Brake, Stop
            USB_SERIAL.println("Braking...");
            break;
        default:
            //Do nothing
    }
}