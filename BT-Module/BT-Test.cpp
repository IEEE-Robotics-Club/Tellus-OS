#include <Arduino.h>
#include <string>

//Pin definitions
#define WAKEUP 30
#define INT 34
#define PDN 35
#define BT_SERIAL Serial1

//Globals
char BUFFER[256];

//Function Declarations
int BTinit();
void getName(char *);
void wakeBT();
bool isOK(char *);
int rollLeaderNumber();

//Function Implementations
int BTinit(){
    BT_SERIAL.begin(115200);
    //Setup BT Pins
    pinMode(WAKEUP, OUTPUT);
    pinMode(PDN, OUTPUT);
    pinMode(INT, INPUT);

    //Exit Sleep Mode
    digitalWrite(PDN, HIGH);
    digitalWrite(PDN, LOW);
    delay(1);

    wakeBT();
    //char* nameBuffer[24];
    //char leaderNumber = (char)rollLeaderNumber();
    //char trailingChars[] = "\r\n\0";
    //char renameCommand[] = "TTM:REN-TellusBot-";

    //Assign A name to the module. 
    char renameCommand[] = "TTM:REN-TellusBot-654\r\n\0";
    if(BT_SERIAL.availableForWrite()){
        for (int i = 0; i < sizeof(renameCommand); i++){
            BT_SERIAL.write(renameCommand[i]);
        }
    }

    delay(10);
    int i = 0;
    // Write the incoming characters to the Buffer
    while(BT_SERIAL.available()){
        BUFFER[i] = (char)BT_SERIAL.read();
        i++;
    } 
    return isOK(BUFFER);
}

void getName(char * output){
    wakeBT();
    //Ask it it's name
    char strCommand[] = "TTM:NAM-?\r\n\0";
    if(BT_SERIAL.availableForWrite()){
        for (int i = 0; i < 12; i++){
            BT_SERIAL.write(strCommand[i]);
        }
    }
    int i = 0;
    // Write the incoming characters to the Buffer
    while(BT_SERIAL.available()){
        output[i] = (char)BT_SERIAL.read();
        i++;
    } 
}

void wakeBT(){
    //Wake up the module by sending a falling edge
    digitalWrite(WAKEUP, HIGH);
    digitalWrite(WAKEUP, LOW);
    delay(1);
}

bool isOK(char * btResponse) {
    // Define the is okay string
    const char* globalSubstring = "TTM:OK";

    // Search for the is okay string in the response
    const char* result = strstr(btResponse, globalSubstring);

    // Check if the substring is found.
    return (result != NULL); 
}

int rollLeaderNumber(){
    //Randomly roll a leader number based off of a random seed
    //Sampeled from an unconnected ADC pin.
    randomSeed(analogRead(7));
    return random(0,255);
}
