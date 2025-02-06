#ifndef UART_H
#define UART_H
void InitUART(void);
void SendMessageDirect();
void __attribute__(( )) _U1RXInterrupt();
#endif /* UART_H */