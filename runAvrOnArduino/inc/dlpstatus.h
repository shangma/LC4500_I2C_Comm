/*
 * dlpstatus.h
 *
 * Created: 5/15/2014 3:40:58 PM
 *  Author: ms
 */ 

/*
status code:

0x08: start condition sent
0x10: repeated start sent
0x18: SLA+W sent, ACK received
0x20: SLA+W sent, NACK received
0x28: data sent, ACK received
0x30: data sent, NACK received
0x38: lost in SLA+W or data or SLA+R or NACK
0x40: SLA+R sent, ACK received
0x48: SLA+R sent, NACK received
0x50: data byte received, ACK returned
0x58: data byte received, NACK returned
*/


#ifndef DLPSTATUS_H_
#define DLPSTATUS_H_


uint8_t slaveDeviceId = 0x1A;
uint8_t writeOnLow = 0x34;
uint8_t readOnLow = 0x35;

/* commands for DLPC 350 */
uint8_t testCommand4reg = 0x04;
uint8_t hardwareStatus = 0x20;
uint8_t systemStatus = 0x21;
uint8_t mainStatus = 0x22;
uint8_t firmVersion= 0x11;
uint8_t powerControlRead = 0x07;
uint8_t powerControlWrite = 0x07|0x80;
uint8_t softwareResetRead = 0x13;
uint8_t softwareResetWrite = 0x13|0x80;

uint8_t displayModeRead = 0x69;
uint8_t displayModeWrite = 0x69 | 0x80;



#endif /* DLPSTATUS_H_ */