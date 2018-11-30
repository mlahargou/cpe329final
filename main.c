#include "msp.h"
#include "rotor.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    signed int rotor1[26] = {
         //index:     5             10              15          20
     // a  b  c d  e  f  g h i   j   k  l  m   n  o p  q r s  t  u  v   w  x  y  z
       -6,-5,-4,3,-4,-2,-1,8,13,-10,-9,-7,-10,-3,-2,4,-9,6,0,-8,-3,-13,-9,-7,-10,10
    };
    signed int rotor2[26] = {
    // a b c   d  e  f  g h  i  j  k  l m n o  p q  r  s   t  u  v  w  x  y  z
       0,8,13,-1,-5,-9,11,4,-3,-9,-7,-1,2,6,10,5,0,-11,12,-6,-13,2,-10,11,-3,-7
    };
    signed int rotor3[26] = {
     // a  b c  d e   f g   h i  j k   l m n o   p q  r s  t  u  v   w  x   y   z
       -7,-1,4,-2,11,-3,12,-4,8,-5,10,-6,9,0,11,-8,8,-9,5,-10,2,-10,-5,-13,-10,-13
    };

    //rotateRotor(rotor1, -2);

    int index = travelIndexThroughAllRotors(rotor1, rotor2, rotor3, 5);


    while(1);
}
