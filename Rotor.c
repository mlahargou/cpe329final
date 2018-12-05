#include <Rotor.h>

Rotors initRotors(int rotorAPos, int rotorBPos, int rotorCPos) {
    Rotors allRotors = {
            //index:     5             10              15          20
        // a  b  c d  e  f  g h i   j   k  l  m   n  o p  q r s  t  u  v   w  x  y  z
         {-6,-5,-4,3,-4,-2,-1,8,13,-10,-9,-7,-10,-3,-2,4,-9,6,0,-8,-3,-13,-9,-7,-10,10}, //allRotors.rotor1
        //a b c   d  e  f  g h  i  j  k  l m n o  p q  r  s   t  u  v  w  x  y  z
         {0,8,13,-1,-5,-9,11,4,-3,-8,-7,-1,2,6,10,5,0,-11,12,-6,-13,2,-10,11,-3,-7},     //allRotors.rotor2
        // a  b c  d e   f g   h i  j k   l m n o   p q  r s  t  u  v   w  x   y   z
         {-7,-1,4,-2,11,-3,12,-4,8,-5,10,-6,9,0,11,-8,8,-9,5,-10,2,-10,-5,-13,-10,-13},  //allRotors.rotor3
         {1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1},             //allRotors.reflector
        //a b c d e f  g h i j  k  l  m  n o   p   q r  s  t  u   v  w  x  y  z
         {1,2,3,4,5,6,-4,8,9,10,13,10,13,0,10,-11,-8,5,-12,7,-10,-9,-2,-5,-8,-11},       //allRotors.rotor5
        //a b c d  e  f g  h   i   j k  l m  n  o  p  q  r  s  t  u v w  x  y  z
         {0,8,1,7,-12,3,11,13,-11,-8,1,-4,10,6,-2,-13,0,-11,7,-6,-5,3,9,-2,-10,5},       //allRotors.rotor6
        //a  b  c d  e  f  g h i j  k l  m n o p  q  r  s  t u v w x  y z
         {4,9,10,2,7,1,-3,9,13,-10,3,8,2,9,10,-8,7,3,0,-4,6,-13,5,-6,4,10}               //allRotors.rotor7
    };

    rotateRotor(allRotors, ROTORA, rotorAPos);
    rotateRotor(allRotors, ROTORB, rotorBPos);
    rotateRotor(allRotors, ROTORC, rotorCPos);

    return allRotors;
}


/*
 * ROTORA   |   ROTORB   |   ROTORC
 * ---------|------------|--------------
 * rotor1 --|-> rotor2 --|-> rotor3 -> |
 *          |            |         reflector
 * rotor7 <-|-- rotor6 <-|-- rotor5 <- |
 */

Rotors rotateRotor(Rotors allRotors, int whichRotor, int rotateAmount) {
    switch (whichRotor) {
        case ROTORA :
            handleRotateOfBothArrays(allRotors.rotor1, allRotors.rotor7, rotateAmount);
            break;
        case ROTORB :
            handleRotateOfBothArrays(allRotors.rotor2, allRotors.rotor6, rotateAmount);
            break;
        case ROTORC :
            handleRotateOfBothArrays(allRotors.rotor3, allRotors.rotor5, rotateAmount);
            break;
    }
    return allRotors;
}

void handleRotateOfBothArrays (int rotorArray1[], int rotorArray2[], int rotateAmount) {
    signed int tempHolder1, tempHolder2;
    int i, j;

    if (rotateAmount > 0) {  //shifting to the right
        for (i = 0; i < rotateAmount; i++) { //shift rotateAmount times
            tempHolder1 = rotorArray1[ROTORSIZE-1];     //hold the last value so it does not get overwritten
            tempHolder2 = rotorArray2[ROTORSIZE-1];     //hold the last value so it does not get overwritten
            for(j = ROTORSIZE - 2; j > -1; j--) {        //shift the whole array one time
                rotorArray1[j + 1] = rotorArray1[j];
                rotorArray2[j + 1] = rotorArray2[j];
            }
            rotorArray1[0] = tempHolder1;      //add the temp variable back to the front
            rotorArray2[0] = tempHolder2;      //add the temp variable back to the front
        }
    }
    else if (rotateAmount < 0) { //shifting to the left
        for (i = 0; i < -rotateAmount; i++) {
            tempHolder1 = rotorArray1[0];
            tempHolder2 = rotorArray2[0];
            for(j = 0; j < (ROTORSIZE - 1); j++) {
                rotorArray1[j] = rotorArray1[j+1];
                rotorArray2[j] = rotorArray2[j+1];
            }
            rotorArray1[ROTORSIZE - 1] = tempHolder1;
            rotorArray2[ROTORSIZE - 1] = tempHolder2;
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
    int indexA = travelThroughRotor(allRotors.rotor1, index);
    int indexB = travelThroughRotor(allRotors.rotor2, indexA);
    return travelThroughRotor(allRotors.rotor3, indexB);
}

int travelIndexThroughReflector(Rotors allRotors, int index) {
    return travelThroughRotor(allRotors.reflector, index);
}

int travleIndexThroughBackwardRotors(Rotors allRotors, int index) {
    int indexZ = travelThroughRotor(allRotors.rotor5, index);
    int indexY = travelThroughRotor(allRotors.rotor6, indexZ);
    return travelThroughRotor(allRotors.rotor7, indexY);
}

int travelIndexThroughAllRotors(Rotors allRotors, int index) {
    int indexForward = travelIndexThroughForwardRotors(allRotors, index);
    int indexReflector = travelIndexThroughReflector(allRotors, indexForward);
    return travleIndexThroughBackwardRotors(allRotors, indexReflector);
}

char getCharByIndex(int index){
    return 97 + index;
}

char getIndexByChar(char character) {
    return character - 97;
}

char getCharThroughAllRotors(Rotors allRotors, char startLetter) {
    if (startLetter == 0xff) {
        return 0xff;
    }
    int startIndex = getIndexByChar(startLetter);
    int endIndex = travelIndexThroughAllRotors(allRotors, startIndex);
    return getCharByIndex(endIndex);
}

