/* 
 * File:   main.c
 * Author: TP_EO_6
 *
 * Created on September 30, 2024, 9:56 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"

int main(void) {
    
    InitTimer1();
    InitTimer23();
    /****************************/
    //Initialisation oscillateur
    /****************************/
    InitOscillator();

    /****************************************/
    //Configuration des input et output (IO)
    /****************************************/
    InitIO();
    LED_BLANCHE_1 = 1;
    LED_BLEUE_1 = 1;
    LED_ORANGE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;
    LED_BLEUE_2 = 1;
    LED_ROUGE_2 = 1;
    LED_VERTE_2 = 1;
    LED_ORANGE_2 = 1;
    LED_BLANCHE_2 = 1;

    /*********************/
    // Boucle Principale
    /********************/
    while (1) {
//        LED_BLEUE_1 = !LED_BLEUE_1;
//        LED_BLANCHE_1 = !LED_BLANCHE_1;
//        LED_ROUGE_2 = !LED_ROUGE_2;
        
    }
    
    
}
