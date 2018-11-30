#include "rotor.h"



void rotateRotor(int rotorArray[], int rotateAmount) {
    signed int tempHolder;
    int i, j;
    if (rotateAmount > 0) {  //shifting to the right
        for (i = 0; i < rotateAmount; i++) { //shift rotateAmount times
            tempHolder = rotorArray[25];     //hold the last value so it does not get overwritten
            for(j = 24; j > -1; j--) {        //shift the whole array one time
                rotorArray[j + 1] = rotorArray[j];
            }
            rotorArray[0] = tempHolder;      //add the temp variable back to the front
        }
    }
    else if (rotateAmount < 0) { //shifting to the left
        for (i = 0; i < -rotateAmount; i++) {
            tempHolder = rotorArray[0];
            for(j = 0; j < 25; j++) {
                rotorArray[j] = rotorArray[j+1];
            }
            rotorArray[25] = tempHolder;
        }
    }
}
int travelThroughRotor(int rotorArray[], int index) {
    int fromAtoB = rotorArray[index]; //tells where to move in B

    signed int newIndex = fromAtoB + index;

    if (newIndex >= 0 && newIndex <= 25) {
        return newIndex;
    }

    if (newIndex < 0) {  //newIndex is negative
        return newIndex + 26;  //wrap around to the higher index
    }
    return newIndex - 26;
}

int travelIndexThroughAllRotors(int rotorArrayA[], int rotorArrayB[], int rotorArrayC[], int index) {
    int indexA = travelThroughRotor(rotorArrayA, 5);
    int indexB = travelThroughRotor(rotorArrayB, indexA);
    return travelThroughRotor(rotorArrayC, indexB);
}

