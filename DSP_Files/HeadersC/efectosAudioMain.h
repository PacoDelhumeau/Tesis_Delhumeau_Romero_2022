#ifndef HEADERSC_EFECTOSAUDIOMAIN_H_
#define HEADERSC_EFECTOSAUDIOMAIN_H_

#include <stdlib.h>
#include  <math.h>

// Coeficientes Reverb
#include "coeficientesReverb.h"

//Funciones para Reverb
extern int reverb(int, int, int, int*);
extern int delay(int,int,int);
extern void movSignal(int,int,int);
extern void cleanSignal(int);

extern short limpiador;

//Definicion de constantes
#define Q11 2048
#define Q12 2048
#define Q15 32768
#define Q16 65536
#define refDAC 3
#define refADC 3.3f

/*  *******************************************************
 *              M�todos de saturaci�n
 *  *******************************************************
 */

//Aplica el efecto de distorsi�n.
int distorcion(long muestra){
    int effect = 0;
    float ytemp, xtemp;
    //Quita offset de la se�al

    // Se aplica un offset para poder tener una se�al de amplitud positiva y negativa
    xtemp = (float)muestra - Q15;
    xtemp = xtemp / (Q16 * refADC);
    // Si el valor absoluto es menor a 0.33 de la m�xima amplitud, se multiplica por dos el valor
    if (fabs(xtemp) < 0.33)
        ytemp = 2*xtemp;
    // Si el valor absoluto es menor a 0.66 de la m�xima amplitud, se utiliza una ecuaci�n no lineal
    else if (fabs(xtemp) < 0.66)
        ytemp = (xtemp/fabs(xtemp))*(1-(pow((2-(3*fabs(xtemp))),2)/3));
    //Si el valor es mayor a 0.66 su valor es el m�ximo posible, manteniendo el signo
    else
        ytemp = xtemp/fabs(xtemp);

    //Se multiplica la se�al por el valor m�ximo del DAC
    ytemp *= (refDAC * Q12);
    effect = ytemp;
    return effect;
}

// Este m�todo solo ocupa una ecuaci�n para amplificar la se�al
int saturacion(long muestra){
    int effect = 0,sign;
    float ytemp, xtemp;

    // Se aplica un offset para poder tener una se�al de amplitud positiva y negativa
    xtemp = (float)muestra - Q15;
    xtemp = xtemp / (Q16 * refADC);

    // Obtiene el sigo
    sign = xtemp/fabs(xtemp);
    // constante g=5 para amplificar el valor
    ytemp = -fabs(xtemp*15);
    effect = Q12 * refDAC * sign * ( 1-exp(ytemp) );
    return effect;
}

/*  *******************************************************
 *              M�todos de reverberaci�n
 *  *******************************************************
 */

//El efecto de reverb genera retrasos de la senal multiplicados por un coerficiente
int reverberacion(int totalMuestras, int banderaEfecto, long muestra, int canal){
    // Valor de la muestra en Q16
    int valorReverb = 0;
    int* apuntadorReverb = &expRev1[0];

    if(banderaEfecto)
        apuntadorReverb = &expRev2[0];

    float senal = ( (float)muestra - Q15 ) / 16;
    int muestraAcon = (int)senal;

    // Limpia el buffer en caso de haber iniciado el efecto
    if(limpiador == 0){
        cleanSignal(totalMuestras);
        limpiador++;
    }

    movSignal(totalMuestras, muestraAcon, canal);
    valorReverb += reverb(totalMuestras, muestraAcon, canal, apuntadorReverb);

    return valorReverb;
}

/*  *******************************************************
 *              M�todos de retardo
 *  *******************************************************
 */

//El efecto de retraso genera la misma se�al multiplicada por un escalar cierto tiempo despu�s
int retardo(int totalMuestras, long muestra, int canal){
    int valorRetardo = 0;

    float senal = ( (float)muestra - Q15 ) / 16;
    int muestraAcon = (int)senal;


    // Limpia el buffer en caso de ser la primera vez
    if(limpiador == 0){
        cleanSignal(totalMuestras);
        limpiador++;
    }

    movSignal(totalMuestras, muestraAcon, canal);
    valorRetardo += delay(totalMuestras, muestraAcon, canal);

    return valorRetardo;
}

#endif /* HEADERSC_EFECTOSAUDIOMAIN_H_ */
