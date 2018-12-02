/*
 * rotor.h
 *
 *  Created on: Nov 30, 2018
 *      Author: michaellahargou
 */

#ifndef ROTOR_H_
#define ROTOR_H_

#define ROTORSIZE 26//95

typedef struct {
    signed int rotor1[ROTORSIZE];
    signed int rotor2[ROTORSIZE];
    signed int rotor3[ROTORSIZE];
    signed int reflector[ROTORSIZE];
    signed int rotor5[ROTORSIZE];
    signed int rotor6[ROTORSIZE];
    signed int rotor7[ROTORSIZE];
} Rotors;

void rotateRotor(int* rotorArray, int rotateAmount);
int travelThroughRotor(int rotorArray[], int index);
int travelIndexThroughForwardRotors(Rotors allRotors,  int index);
int travelIndexThroughReflector(Rotors allRotors,  int index);
int travleIndexThroughBackwardRotors(Rotors allRotors,  int index);
int travelIndexThroughAllRotors(Rotors allRotors, int index);
char getCharThroughAllRotors(Rotors allRotors, char startLetter);
char getCharByIndex(int index);
char getCharThroughAllRotors(Rotors allRotors, char startLetter);

#endif /* ROTOR_H_ */
