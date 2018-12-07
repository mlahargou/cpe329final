/*
 * mspCommunication.h
 *
 *  Created on: Dec 3, 2018
 *      Author: michaellahargou
 */

#ifndef MSPCOMMUNICATION_H_
#define MSPCOMMUNICATION_H_

int messageComplete;
char receivedMessage[1024];

void mspCommunicationInit();
void recievedChar (char letter);
void sendMessege(char * message);


#endif /* MSPCOMMUNICATION_H_ */
