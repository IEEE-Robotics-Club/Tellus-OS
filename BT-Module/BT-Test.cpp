#include <Arduino.h>

//BT Pin Setup
#define WAKEUP 30
#define INT 34
#define PDN 35
#define BT_SERIAL Serial1
#define USB_SERIAL Serial

void requestName(void);

void setup() {
    // initialize both serial ports:
    USB_SERIAL.begin(9600);
    BT_SERIAL.begin(115200);
    pinMode(WAKEUP, OUTPUT);
    pinMode(PDN, OUTPUT);
    pinMode(INT, INPUT);

    //Exit Sleep Mode
    digitalWrite(PDN, HIGH);
    delay(10);
    digitalWrite(PDN, LOW);

    //Wake up the module by sending a falling edge
    digitalWrite(WAKEUP, HIGH);
    digitalWrite(WAKEUP, LOW);
    delay(1);
    char renameCommand[] = "TTM:REN-TellusBot-1\r\n\0";
    if(BT_SERIAL.availableForWrite()){
        for (int i = 0; i < 22; i++){
            BT_SERIAL.write(renameCommand[i]);
        }
    }
}

void loop()
{
    requestName();

    // Write the incoming characters to the USB Serial
    while(BT_SERIAL.available()){
        char current = (char)BT_SERIAL.read();
        if (current != '\0'){
            USB_SERIAL.write(current);
        }
    }

    delay(1000);
}

void requestName(void){
    //Wake up the module by sending a falling edge
    digitalWrite(WAKEUP, HIGH);
    digitalWrite(WAKEUP, LOW);
    delay(1);

    //Ask it it's name
    char strCommand[] = "TTM:NAM-?\r\n\0";
    if(BT_SERIAL.availableForWrite()){
        for (int i = 0; i < 12; i++){
            BT_SERIAL.write(strCommand[i]);
        }
        USB_SERIAL.println(strCommand);
    }
}
