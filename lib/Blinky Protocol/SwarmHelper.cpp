#include <Entropy.h>
#include <string>

char rollLeaderNumber();
void buildName(char *);

char rollLeaderNumber(){
    //Randomly roll a leader number based off of a random seed
    //Sampled from an unconnected ADC pin.
    Entropy.Initialize();
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