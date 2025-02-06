#ifndef CB_TX1_H
#define CB_TX1_H


void SendMessage();
void CB_TX1_Add(unsigned char);
void __attribute__(( )) _U1TXInterrupt() ;
void SendOne();
int CB_TX1_GetDataSize();
int CB_TX1_GetRemainingSize();
unsigned char CB_TX1_IsTranmitting();
//int CB_TX1_RemainingSize();
#endif /* CB_TX1_H */
