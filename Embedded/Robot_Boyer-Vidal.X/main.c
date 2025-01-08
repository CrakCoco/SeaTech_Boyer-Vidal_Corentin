#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "main.h"

//unsigned int ADCValue0, ADCValue1, ADCValue2;

int main(void) {
    /***************************************************************************/
    //Initialisation oscillateur
    /***************************************************************************/
    InitOscillator();
    /******************************************************************************/
    // Configuration des input et output (IO)
    /******************************************************************************/

    InitIO();
    LED_BLANCHE_1 = 1;
    LED_BLEUE_1 = 1;
    LED_ORANGE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;




    InitPWM();
    InitADC1();
    InitTimer23();
    InitTimer1();
    InitTimer4();


    /******************************************************************************/
    // Boucle Principale
    /******************************************************************************/
    while (1) {
        float distance30 = 0x936;
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();

            float volts = ((float) result [0])* 3.3 / 4096;
            //robotState.distanceTelemetreEXGauche = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096;
            //robotState.distanceTelemetreEXDroite = 34 / volts - 5;

            if (result[0] > distance30) {
                LED_BLANCHE_2 = 1;
            } else {
                LED_BLANCHE_2 = 0;
            }
            if (result[1] > distance30) {
                LED_BLEUE_2 = 1;
            } else {
                LED_BLEUE_2 = 0;
            }
            if (result[2] > distance30) {
                LED_ORANGE_2 = 1;
            } else {
                LED_ORANGE_2 = 0;
            }
            if (result[3] > distance30) {
                LED_ROUGE_2 = 1;
            } else {
                LED_ROUGE_2 = 0;
            }
            if (result[4] > distance30) {
                LED_VERTE_2 = 1;
            } else {
                LED_VERTE_2 = 0;
            }
        }
    }
} // fin main

unsigned char stateRobot;
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char capteurs = 0;

    // Encodage des capteurs dans un mot binaire
    if (robotState.distanceTelemetreExtremeGauche < 30) capteurs |= 0b10000; // Bit 4
    if (robotState.distanceTelemetreGauche < 30) capteurs |= 0b01000; // Bit 3
    if (robotState.distanceTelemetreCentre < 30) capteurs |= 0b00100; // Bit 2
    if (robotState.distanceTelemetreDroit < 30) capteurs |= 0b00010; // Bit 1
    if (robotState.distanceTelemetreExtremeDroit < 30) capteurs |= 0b00001; // Bit 0

    // Gestion des Ã©tats en fonction des capteurs
    switch (capteurs) {
            // Aucun obstacle
        case 0b00000:
        case 0b10001:
            nextStateRobot = STATE_AVANCE;
            break;

        // Obstacle au centre uniquement
        case 0b00100:
        case 0b11010:
        case 0b01011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE;
            break;
            
        case 0b11100:
        case 0b10100:
        case 0b01100:        
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
            
        case 0b00111:
        case 0b00101:
        case 0b00110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;

        case 0b10000:
        case 0b01000:
        case 0b11000:
        case 0b01001:
        case 0b10010:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
         
        case 0b00010:
        case 0b00001:
        case 0b00011 :
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
            
        case 0b01111:
        case 0b10111:
        case 0b11011:
        case 0b11101:
        case 0b11110:
        case 0b11111:
        case 0b01110:
        case 0b10110:
        case 0b01101:
        case 0b01010:
        case 0b10101:
            nextStateRobot = STATE_RECULE;
            break;
            
        default :
            nextStateRobot = STATE_AVANCE;
            break;
            
    }

    //Si l’on n’est pas dans la transition de lé’tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;

}


//operatin system loop

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
            break;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;

        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_EN_COURS;
            break;

        case STATE_TOURNE_SUR_PLACE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;


        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(-5, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;

        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(-30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(5, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;

        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_RECULE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            if (!(robotState.distanceTelemetreExtremeGauche < 30 ||
                  robotState.distanceTelemetreGauche < 30 ||
                  robotState.distanceTelemetreCentre < 30 ||
                  robotState.distanceTelemetreDroit < 30 ||
                  robotState.distanceTelemetreExtremeDroit < 30)) {
                stateRobot = STATE_TOURNE_SUR_PLACE;
            }
            break;

        case STATE_RECULE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}