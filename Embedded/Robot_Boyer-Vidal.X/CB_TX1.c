#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "CB_TX1.h"
#define CBTX1_BUFFER_SIZE 128
int cbTx1Head;
int cbTx1Tail;
unsigned char cbTx1Buffer[CBTX1_BUFFER_SIZE];
unsigned char isTransmitting = 0;
void SendMessage(unsigned char* message, int length)
{
unsigned char i=0;
if(CB_TX1_RemainingSize()>length)
{
//On peut écrire le message
for(i=0;i<length;i++)
CB_TX1_Add(message[i]);
if(!CB_TX1_IsTranmitting())
SendOne();
}
}
void CB_TX1_Add(unsigned char value)
{
    // Ajout du caractère à la position du head dans le buffer
    cbTx1Buffer[cbTx1Head] = value;

    // Incrémentation du head, avec gestion du dépassement circulaire
    cbTx1Head = (cbTx1Head + 1) % CBTX1_BUFFER_SIZE;

    // Si le buffer est plein, on avance également le tail pour écraser la donnée la plus ancienne
    if (cbTx1Head == cbTx1Tail)
    {
        // Le buffer est plein, on écrase la donnée la plus ancienne
        cbTx1Tail = (cbTx1Tail + 1) % CBTX1_BUFFER_SIZE;
    }
}


unsigned char CB_TX1_Get(void)
{
    // Récupération de la donnée à la position du tail
    unsigned char value = cbTx1Buffer[cbTx1Tail];

    // Incrémentation du tail avec gestion du dépassement circulaire
    cbTx1Tail = (cbTx1Tail + 1) % CBTX1_BUFFER_SIZE;

    return value;
}


void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
IFS0bits.U1TXIF = 0; // clear TX interrupt flag
if (cbTx1Tail!=cbTx1Head)
{
SendOne();
}
else
isTransmitting = 0;
}
void SendOne()
{
isTransmitting = 1;
unsigned char value=CB_TX1_Get();
U1TXREG = value; // Transmit one character
}
unsigned char CB_TX1_IsTranmitting(void)
{
    return isTransmitting;
}

int CB_TX1_GetDataSize(void)
{
    int dataSize;

    // Calcul de la taille des données dans le buffer (différence entre head et tail)
    dataSize = (cbTx1Head - cbTx1Tail + CBTX1_BUFFER_SIZE) % CBTX1_BUFFER_SIZE;

    return dataSize;
}

int CB_TX1_GetRemainingSize(void)
{
    int remainingSize;

    // Calcul de la taille restante dans le buffer
    remainingSize = (CBTX1_BUFFER_SIZE - 1) - CB_TX1_GetDataSize();

    return remainingSize;
}

