/*
 * Es necesario verificar el comportamiento de la funcion reverb
 * Agregar efectos de audio para el giroscopio
 */

// Archivos que simpre se incluyen
#include "F28x_Project.h"

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <HeadersC/configPerifericos.h>
#include <HeadersC/coeficientesIIR.h>
#include <HeadersC/efectosAudioMain.h>
#include <HeadersC/coeficientesPaneoEstereo.h>

/*  *************************************************************
 *                      Definición de constantes
 *  *************************************************************
 */
#define vref 1
#define M_PI 3.14159265358979323846
#define FS1 8000
#define FS2 44000
#define FS3 88000
#define Q11 2048

// Definicion de EPWMs
#define EPWM1 0
#define EPWM2 1
//#define EPWM3 2
#define EPWM7 6
#define EPWM8 7
#define EPWM9 8
// s2 -> dos segundos
#define s2 0.5
#define s1 1

// Definicion de ADC
#define ADCA 0
#define ADCB 1
#define SOC 0
#define EPWM_N 1

// Definicion de DACs
#define DACA 0
#define DACB 1
#define DACC 2

// Definicion de SCI
#define SCIB 0
#define Tasa_Bauds 115200

/*  *************************************************************
 *                      Prototipos de funciones
 *  *************************************************************
 */
// Funciones generales
int freqOSC(void);

//Funciones para EL SCI
void SCI_Config(Uint16, Uint32);

// Funciones para el ADC
void ADC_Config(Uint16, Uint16, Uint16);
void EPWM_Config(Uint16, float, Uint16, float);
void configTimer(void);
void ADCSOC_Config(Uint16, Uint16, Uint16, Uint16);
// Funciones para el DAC
void DAC_Config(Uint16);

//Funciones en lenguaje ensamblador
float IIRFloat(int, float, float*, float*);
void cleanFloat();

/*  *************************************************************
 *                         Interrupciones
 *  *************************************************************
 */
interrupt void ADCA1_TEMP_isr(void);
interrupt void ADCB1_AUDIO_isr(void);
interrupt void SCIB_inter(void);
//interrupt void EPWM3_isr(void);
interrupt void EPWM7_isr(void);
interrupt void EPWM8_isr(void);
interrupt void EPWM9_isr(void);

/* ******************************************************************
 *                       Variables globales
 * ******************************************************************
 */
// Variable que recibe el valor transmitido por el SCI
char charRecibido = 0;
// Decodifica la instrucción recibida (primer valor en el SCI)
int instruccion = 0;
// Obtiene el valor y modifica las banderas para realizar los efectos (segundo valor en SCI)
int valor = 0;
// Bandera de los efectos a aplicar. El primero es el efecto [1,4] y la otra es la variante [0,1]
int efecto = 0, banderaEfecto = 0;
// Matriz que guarda signo y magnitud, respectivamente, de cada filtro
float filtros[10] = {1,1,1,1,1,1,1,1,1,1};
// Matriz que guarda signo y magnitud del sensor de orientación [0] -> plano X ; [1] -> Plano Y
Uint16 sensorOrientacion = 90;
// Valores del ADC
int32 ADCBcanal2 = 0, ADCBcanal4 = 0;
// Multiplicador de prueba
float escalarADC = 1;
// Bandera de habilitacion de EPWM
short banderaEpwm = 0;
//Bandera para limpiar buffer de muestras
short limpiador = 0;
//Bandera para delimitar la llegada de una muestra
short banderaMuestreo = 0;

/*  *************************************************************
 *                Codigo principal
 *  *************************************************************
 */
void main(void){
    // Variables que almacenan temporalmente el valor de los efectos
    float canal1 = 0, canal2 = 0;

    // Configuración inicial del DSP. Inicializacón de relojes y módulos necesarios
     InitSysCtrl();
    // Configuración de inicio de los GPIOS
    InitGpio();
    InitEPwmGpio();

/*  *************************************************************
 *                Configuración de las Interrupciones
 *  *************************************************************
 */
    // Deshabilita las interrupciones para su configuración
    DINT;

    // Inicia los registros de configuración de PIE
    InitPieCtrl();
    // Borra banderas de interrupción
    IER = 0x0000;
    IFR = 0x0000;
    // Inicia el vector de PIE a un valor conocido
    InitPieVectTable();

    // Direccionamiento de las interrupciones
    EALLOW;
    PieVectTable.ADCA1_INT = &ADCA1_TEMP_isr;
    PieVectTable.ADCB1_INT = &ADCB1_AUDIO_isr;
    //PieVectTable.EPWM3_INT = &EPWM3_isr;
    PieVectTable.EPWM7_INT = &EPWM7_isr;
    PieVectTable.EPWM8_INT = &EPWM8_isr;
    PieVectTable.EPWM9_INT = &EPWM9_isr;
    PieVectTable.SCIB_RX_INT = &SCIB_inter;
    EDIS;


/*  *************************************************************
 *                Declaración de funciones necesarias
 *  *************************************************************
 */
    // Configuración de los ADCs
    //__asm("\tESTOP0");
    ADC_Config(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    ADC_Config(ADC_ADCB, ADC_RESOLUTION_16BIT, ADC_SIGNALMODE_SINGLE);

    // Inicialización del sensor de Temperatura con su offset inical
    InitTempSensor(3.0);

    // Configuracion de EPWMs
    // Configura EPWM1 para disparar ADCA
    EPWM_Config(EPWM1, s2, SOC, 100);
    //Configura EPWM2 para disparar ADCB
    EPWM_Config(EPWM2, FS2, SOC, 100);
    //Configura EPWM3 para DAC
    //EPWM_Config(EPWM3, FS2, EPWM_N, 100);
    //Configura EPWM7,8,9 para LED
    EPWM_Config(EPWM7, s2, EPWM_N, 50);
    EPWM_Config(EPWM8, 1, EPWM_N, 50);
    EPWM_Config(EPWM9, 2, EPWM_N, 50);
    //configTimer();

    // Configuración de los canales
    ADCSOC_Config(ADC_ADCA, 0, 5, 13);
    ADCSOC_Config(ADC_ADCB, 0, 7, 2);
    ADCSOC_Config(ADC_ADCB, 1, 7, 4);
    // Configuracion del DAC
    DAC_Config(DACA);
    DAC_Config(DACB);
    // Inicializa los valores del SCIB
    SCI_Config(SCIB, Tasa_Bauds);


/*  *************************************************************
 *                Habilitación de interrupciones
 *  *************************************************************
 */
    // Habilita interrupciones del grupo 1 (ADCA y ADCB)
    IER |= M_INT1;
    IER |= M_INT3;
    IER |= M_INT9;
    // Limpia banderas de interrupción
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx2 = 1;
    //PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx8 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx9 = 1;
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
    PieCtrlRegs.PIEIER9.bit.INTx4 = 1;
    // Enable Global interrupt INTM
    EINT;
    // Enable Global realtime interrupt DBGM
    ERTM;


/*  *************************************************************
 *                Configuraciones inicales
 *  *************************************************************
 */
    // Habilitación del EPWM1
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    // Habilita el SOCA
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm2Regs.ETSEL.bit.SOCAEN = 1;
    // Activa la cuenta del EPWM
    EPwm1Regs.TBCTL.bit.CTRMODE = 0;
    EPwm2Regs.TBCTL.bit.CTRMODE = 0;
    EDIS;

    //Habiliatación del TIMER0
    CpuTimer0Regs.TCR.bit.TSS = 0;

    // Limpia el vector del filtro
    cleanFloat();

    while(1){
        if(banderaMuestreo){
            //Efecto de saturacion
             if(efecto == 2){
                 if(banderaEfecto == 0){
                     canal1 = distorcion(ADCBcanal2);
                     canal2 = distorcion(ADCBcanal4);
                 }
                 else{
                     //Efecto de distorción
                     canal1 = saturacion(ADCBcanal2);
                     canal2 = saturacion(ADCBcanal4);
                 }
             }
             //Efecto de reverberacion
             else{
                 if(efecto == 3){
                     int N = 8000;
                     if(banderaEfecto == 1)
                         N = 12000;
                     canal1 = reverberacion(N-1, banderaEfecto, ADCBcanal2, 0);
                     canal2 = reverberacion(N-1, banderaEfecto, ADCBcanal4, 1);
                 }
                 // Efecto de retraso
                 else{
                     if(efecto == 4){
                         int N = 1320;
                         if(banderaEfecto == 1)
                             N = 2200;
                             canal1 = retardo(N-1, ADCBcanal2, 0);
                             canal2 = retardo(N-1, ADCBcanal4, 1);
                     }
                     // Banco de filtros
                     else{
                         if(efecto == 5){
                             if(!limpiador){
                                 cleanFloat();
                                 limpiador++;
                             }
                             canal1 = (((float)ADCBcanal2-32768)/(32768*3.3));
                             canal1 = IIRFloat(0, canal1,&coeficientesIIR[0],&filtros[0])*3*2048;
                             canal2 = (((float)ADCBcanal4-32768)/(32768*3.3));
                             canal2 = IIRFloat(1, canal2,&coeficientesIIR[0],&filtros[0])*3*2048;
                         }
                         // No realiza cambio a la señal
                         else{
                             canal1 = (ADCBcanal2 - 32768) >> 4;
                             canal2 = (ADCBcanal4 - 32768) >> 4;
                         }

                     }
                 }
             }
             if(sensorOrientacion < 180){
                 DacaRegs.DACVALS.all = round(canal1 * coeficientesIzq[sensorOrientacion]) + 2048;
                 DacbRegs.DACVALS.all = round(canal2 * coeficientesDer[sensorOrientacion]) + 2048;
             }
             banderaMuestreo = 0;
        }
    }

}

/*  *************************************************************
 *                      Definición de funciones
 *  *************************************************************
 */



/*  *************************************************************
 *                      Definición de interrupciones
 *  *************************************************************
 */

// Interrupción para el sensor de temperatura
interrupt void ADCA1_TEMP_isr(void)
{
    // Limpia la bandera de interrupcion del ADCA
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    // Condicional en caso de existir overflow
    if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT1)
    {
        AdcaRegs.ADCINTOVFCLR.bit.ADCINT1 = 1;
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    }

    // Aknowledge del vector de PIE
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    // Obtiene los datos del sensor de temperatura
    int sensorSample = AdcaResultRegs.ADCRESULT0;
    // Convierte el valor del sensor de temperatura a Celcius
    int sensorTemp = GetTemperatureC(sensorSample);
}

// Interrupcion del ADCB para muestrear la señal de audio
interrupt void ADCB1_AUDIO_isr(void)
{
    // Limpia la bandera de interrupcion del ADCB
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    // Condicional en caso de existir overflow
    if(1 == AdcbRegs.ADCINTOVF.bit.ADCINT1)
    {
        AdcbRegs.ADCINTOVFCLR.bit.ADCINT1 = 1;
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    }
    // Aknowledge del vector de PIE
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    //ADCBcanal2 = ( AdcbResultRegs.ADCRESULT0 - 32768 ) >> 4;
    //ADCBcanal4 = ( AdcbResultRegs.ADCRESULT1 - 32768 ) >> 4;
    ADCBcanal2 = AdcbResultRegs.ADCRESULT0;
    ADCBcanal4 = AdcbResultRegs.ADCRESULT1;
    banderaMuestreo = 1;
}

// Apaga EPWM3 -> DAC
/*interrupt void EPWM3_isr(void){
    EPwm3Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    EPwm3Regs.TBCTL.bit.CTRMODE = 3;
    EPwm3Regs.TBCTR = 0x0000;
}*/

// Apaga EPWM7 -> LED Azul
interrupt void EPWM7_isr(void){
    EPwm7Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    EPwm7Regs.TBCTL.bit.CTRMODE = 3;
    EPwm7Regs.TBCTR = 0x0000;
}

// Apaga EPWM8 -> LED Verde
interrupt void EPWM8_isr(void){
    EPwm8Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    EPwm8Regs.TBCTL.bit.CTRMODE = 3;
    EPwm8Regs.TBCTR = 0x0000;
}

// Apaga EPWM9 -> LED Rojo
interrupt void EPWM9_isr(void){
    EPwm9Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    if(banderaEpwm)
        EPwm9Regs.TBCTL.bit.CTRMODE = 3;
    EPwm9Regs.TBCTR = 0x0000;
}

// Interrupcion del SCIB para recibir información del Smartphone
interrupt void SCIB_inter(void){
    // Variables locales para datos
    int indiceFiltro = 0, signo = 0;
    float magnitud = 0;

    // Limpia banderas para poder recibir otra interrupción
    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1 ;
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1 ;
    // Reconoce la interrupción
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Guarda el valor recibido en charRecibido
    charRecibido = ScibRegs.SCIRXBUF.all;

    // Identifica si es el primer valor recibido
    if(instruccion == 0)
        instruccion = (int)charRecibido;
    else{
        // Decodifica la instrucción y el valor a ser aplicados
        valor = (int)charRecibido;
        //
        if((instruccion & 0x10) == 0x10){
            switch(valor){
                case 1:
                    modifFreqEPWM(EPWM2, FS1, 100);
                    break;
                case 2:
                    modifFreqEPWM(EPWM2, FS2, 100);
                    break;
                case 3:
                    modifFreqEPWM(EPWM2, FS3, 100);
                    break;
            }
        }
        else{
            if((instruccion & 0x20) == 0x20){
                efecto = valor & 0x0F;
                // El bit más significativo tiene la variante del efecto
                banderaEfecto = (valor & 0x80) >> 7;
                banderaEpwm = 1;
                if(efecto == 3)
                    modifFreqEPWM(EPWM2, FS1, 100);
                else
                    modifFreqEPWM(EPWM2, FS2, 100);
                EPwm7Regs.TBCTL.bit.CTRMODE = 0;
            }
            else{
                //Instrucción = 64 + x -> Ganancia de filtro
                if((instruccion & 0x40) == 0x40){
                    // El numero de filtro está en la parte menos significativa
                    indiceFiltro = (instruccion & 0x0F)-1;
                    // Obtiene el signo
                    signo = (valor & 0x80) >> 7;
                    // Obtiene la magnitud del filtro en DBs en formato Q3
                    if(signo == 1){
                        signo = -1;
                        magnitud = 256-valor;
                    }else{
                        signo = 1;
                        magnitud = valor;
                    }
                    // Esta en formato Q3*base log
                    magnitud = signo*magnitud/(8*10);
                    magnitud = pow(10,magnitud);
                    filtros[indiceFiltro] = magnitud;
                    EPwm8Regs.TBCTL.bit.CTRMODE = 0;
                }
                else{
                    //Instrucción = 128 + x -> Giroscopio
                    if((instruccion & 0x80) == 0x80){
                        // Obtiene el signo del Sensor de Orientación
                        signo = (valor & 0x80) >> 7;
                        // Los grados de giro del sensor de orientación
                        if(signo == 1){
                            signo = -1;
                            magnitud = 256-valor;
                        }else{
                            signo = 1;
                            magnitud = valor;
                        }
                        sensorOrientacion = signo*magnitud + 90;
                        EPwm9Regs.TBCTL.bit.CTRMODE = 0;
                    }
                }
            }
        }
        //Reinicia los valores para recibir
        valor = 0;
        instruccion = 0;
        //Limpia el buffer para los efectos
        limpiador = 0;
    }
}

// Fin del archivo
