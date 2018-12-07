#include <Rotor.h>

Rotors initRotors(int rotorAPos, int rotorBPos, int rotorCPos) {
    /*
     * These are the initial rotor positions.  Each array index maps to one and only
     * one index.  For instance, no two values in rotor1 can map to rotor2.
     *
     * Rotor5,6,7 are the reverse or rotor3,2,1 respectively.  This is directly due
     * to the fact that once the message traverses through the first three rotors,
     * it hits the reflector and must travel with the same exact rotors.
     */
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

    rotateRotor(&allRotors, ROTORA, rotorAPos);
    rotateRotor(&allRotors, ROTORB, rotorBPos);
    rotateRotor(&allRotors, ROTORC, rotorCPos);

    return allRotors;
}


/*
 * ROTORA   |   ROTORB   |   ROTORC
 * ---------|------------|--------------
 * rotor1 --|-> rotor2 --|-> rotor3 -> |
 *          |            |         reflector
 * rotor7 <-|-- rotor6 <-|-- rotor5 <- |
 *
 * When we rotate a rotor, we must rotate it's corresponding rotor (i.e.
 * rotor1 <->rotor7).  This function handles that.
 */

void rotateRotor(Rotors *allRotors, int whichRotor, int rotateAmount) {
    switch (whichRotor) {
        case ROTORA : //ROTORA -> rotor1 and rotor 7
            handleRotateOfBothArrays(allRotors->rotor1, allRotors->rotor7, rotateAmount);
            break;
        case ROTORB : //ROTORB -> rotor2 and rotor 6
            handleRotateOfBothArrays(allRotors->rotor2, allRotors->rotor6, rotateAmount);
            break;
        case ROTORC : //ROTORc -> rotor3 and rotor 5
            handleRotateOfBothArrays(allRotors->rotor3, allRotors->rotor5, rotateAmount);
            break;
    }
}

void handleRotateOfBothArrays (int rotorArray1[], int rotorArray2[], int rotateAmount) {
    signed int tempHolder1, tempHolder2;
    int i, j;

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

int travelThroughRotor(int rotorArray[], int index) {
    int fromAtoB = rotorArray[index]; //tells where to move in B

    signed int newIndex = fromAtoB + index;

    // if we are in a valid index, return the index
    if (newIndex >= 0 && newIndex <= (ROTORSIZE - 1)) {
        return newIndex;
    }

    // if we are in a negative index, wrap around
    if (newIndex < 0) {  //newIndex is negative
        return newIndex + ROTORSIZE;  //wrap around to the higher index
    }

    // else, the index is higher than our ROTORSIZE which needs to wrap to 0 index.
    return newIndex - ROTORSIZE;
}

/*
 * Sends the index through rotor1, rotor2, and rotor3
 */
int travelIndexThroughForwardRotors(Rotors allRotors, int index) {
    int indexA = travelThroughRotor(allRotors.rotor1, index);
    int indexB = travelThroughRotor(allRotors.rotor2, indexA);
    return travelThroughRotor(allRotors.rotor3, indexB);
}

/*
 * Sends the index through the reflector
 */
int travelIndexThroughReflector(Rotors allRotors, int index) {
    return travelThroughRotor(allRotors.reflector, index);
}

/*
 * Sends the index through rotor7, rotor6, and rotor4
 */
int travleIndexThroughBackwardRotors(Rotors allRotors, int index) {
    int indexZ = travelThroughRotor(allRotors.rotor5, index);
    int indexY = travelThroughRotor(allRotors.rotor6, indexZ);
    return travelThroughRotor(allRotors.rotor7, indexY);
}

/*
 * Sends the index through all the rotors
 */
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

/*
 * This figures out the index of the char and sends the index through all
 * of the rotors.  It returns the char after traveling all of the rotors.
 */
char getCharThroughAllRotors(Rotors *allRotors, char startLetter) {
    incrementRotors(allRotors);
    if (startLetter == 0xff) {
        return 0xff;
    }
    if (startLetter == 0x20) {
        return 0x20;
    }
    int startIndex = getIndexByChar(startLetter);
    int endIndex = travelIndexThroughAllRotors(*allRotors, startIndex);

    return getCharByIndex(endIndex);
}

void incrementRotors(Rotors *allRotors){
    static uint8_t ia = 0;                      //tracks rotor A rotations
    static uint8_t ib = 0;                      //tracks rotor B rotations

    ia++;                                       //Increment rotor A counter
    rotateRotor(allRotors, ROTORA, 1);          //rotate rotor A

    if (ia==26){                                //once 26 rotations of rotor A
        rotateRotor(allRotors, ROTORB, 1);      //rotate rotor B
        ia = 0;                                 //reset rotor A count
        ib++;                                   //Increment rotor B count

        if (ib==26){                            //once 26 rotations of rotor B
            rotateRotor(allRotors, ROTORC, 1);  //rotate rotor C
            ib = 0;                             //reset rotor B count
        }
    }
}
