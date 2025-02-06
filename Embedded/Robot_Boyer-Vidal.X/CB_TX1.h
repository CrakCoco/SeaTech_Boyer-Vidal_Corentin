#ifndef CB_TX1_H
#define CB_TX1_H


void SendMessage();
void CB_TX1_Add();
void __attribute__(( )) _U1TXInterrupt() ;
void SendOne();
int CB_TX1_GetRemainingSize();
unsigned char CB_TX1_IsTranmitting();

#endif /* CB_TX1_H */
