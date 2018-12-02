#include "rotor.h"

void rotateRotor(int rotorArray[], int rotateAmount) {
    signed int tempHolder;
    int i, j;
    if (rotateAmount > 0) {  //shifting to the right
        for (i = 0; i < rotateAmount; i++) { //shift rotateAmount times
            tempHolder = rotorArray[ROTORSIZE-1];     //hold the last value so it does not get overwritten
            for(j = ROTORSIZE - 2; j > -1; j--) {        //shift the whole array one time
                rotorArray[j + 1] = rotorArray[j];
            }
            rotorArray[0] = tempHolder;      //add the temp variable back to the front
        }
    }
    else if (rotateAmount < 0) { //shifting to the left
        for (i = 0; i < -rotateAmount; i++) {
            tempHolder = rotorArray[0];
            for(j = 0; j < (ROTORSIZE - 1); j++) {
                rotorArray[j] = rotorArray[j+1];
            }
            rotorArray[ROTORSIZE - 1] = tempHolder;
        }
    }
}
int travelThroughRotor(int rotorArray[], int index) {
    int fromAtoB = rotorArray[index]; //tells where to move in B

    signed int newIndex = fromAtoB + index;

    if (newIndex >= 0 && newIndex <= (ROTORSIZE - 1)) {
        return newIndex;
    }

    if (newIndex < 0) {  //newIndex is negative
        return newIndex + ROTORSIZE;  //wrap around to the higher index
    }
    return newIndex - ROTORSIZE;
}

int travelIndexThroughForwardRotors(Rotors allRotors, int index) {
    int indexA = travelThroughRotor(allRotors.rotor1, 5);
    int indexB = travelThroughRotor(allRotors.rotor2, indexA);
    return travelThroughRotor(allRotors.rotor3, indexB);
}

int travelIndexThroughReflector(Rotors allRotors, int index) {
    return travelThroughRotor(allRotors.reflector, index);
}

int travleIndexThroughBackwardRotors(Rotors allRotors, int index) {
    int indexZ = travelThroughRotor(allRotors.rotor5, 5);
    int indexY = travelThroughRotor(allRotors.rotor6, indexZ);
    return travelThroughRotor(allRotors.rotor7, indexY);
}

int travelIndexThroughAllRotors(Rotors allRotors, int index) {
    int indexForward = travelIndexThroughForwardRotors(allRotors, index);
    int indexReflector = travelIndexThroughReflector(allRotors, indexForward);
    return travleIndexThroughBackwardRotors(allRotors, indexReflector);
}

char getCharByIndex(int index){
    return 65 + index;
}

char getIndexByChar(char character) {
    return character - 65;
}

char getCharThroughAllRotors(Rotors allRotors, char startLetter) {
    int startIndex = getIndexByChar(startLetter);
    int endIndex = travelIndexThroughAllRotors(allRotors, startIndex);
    return getCharByIndex(endIndex);
}

