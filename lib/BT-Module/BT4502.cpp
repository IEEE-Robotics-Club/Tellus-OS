#include <Arduino.h>
#include <string>
#include <Entropy.h>

//Pin definitions
#define WAKEUP 30
#define INT 34
#define PDN 35
#define BT_SERIAL Serial1
#define USB_SERIAL Serial

//Globals
char BUFFER[256];

//Function Declarations
bool BTinit();
void getName(char *);
void wakeBT();
bool isOK(char *);
char rollLeaderNumber();
void buildName(char *);

//Function Implementations
bool BTinit(){
    BT_SERIAL.begin(115200);
    Entropy.Initialize();
    //Setup BT Pins
    pinMode(WAKEUP, OUTPUT);
    pinMode(PDN, OUTPUT);
    pinMode(INT, INPUT);

    //Exit Sleep Mode
    digitalWrite(PDN, HIGH);
    digitalWrite(PDN, LOW);
    delay(2);

    wakeBT();
    //Create a name with a random number appended
    char name[25];
    buildName(name);
    USB_SERIAL.println(name);

    //Change the name of the Module to this name
    if(BT_SERIAL.availableForWrite()){
        for (int i = 0; i < sizeof(name)-1; i++){
            BT_SERIAL.write(name[i]);
            if(name[i]=='\0'){
                break;
            }
        }
    }

    //Setting the name is a little slow. If we don't wait, we may not get
    //The OKAY in 1 go, and will intermittently fail the setup check.
    delay(100);

    // Write the incoming characters to the Buffer
    int i = 0;
    while(BT_SERIAL.available()){
        BUFFER[i] = (char)BT_SERIAL.read();
        if(BUFFER[i]=='\0'){
            break;
        }
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
    delay(2);
}

bool isOK(char * btResponse) {
    // Define the is okay string
    const char* globalSubstring = "TTM:OK";

    // Search for the is okay string in the response
    const char* result = strstr(btResponse, globalSubstring);

    // Check if the substring is found.
    return (result != NULL); 
}

char rollLeaderNumber(){
    //Randomly roll a leader number based off of a random seed
    //Sampled from an unconnected ADC pin.
    return (char)Entropy.random(0,99);
}

void buildName(char * inputArray) {
    char trailingChars[] = "\r\n\0";
    char renamePrefix[] = "TTM:REN-TellusBot-";
    int counter = 0;
    int j = 0;

    for (int i = 0; i < sizeof(renamePrefix)-1; i++){
        inputArray[i] = renamePrefix[i];
        counter++;
    }

    char randomNumber[4];
    sprintf(randomNumber, "%d", rollLeaderNumber());
    j = 0;
    for (int i = counter; i< (counter + sizeof(randomNumber)-1); i++){
        if(randomNumber[j] == '\0'){
            break;
        }
        inputArray[i] = randomNumber[j];
        j++;
        counter++;
    }
    inputArray[counter] = '\r';
    inputArray[counter + 1] = '\n';
    inputArray[counter + 2] = '\0';

}