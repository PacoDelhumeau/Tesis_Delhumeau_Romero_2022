/*
 * Define las funciones que se ocupan para configurar los periféricos
 */

#ifndef HEADERSC_CONFIGPERIFERICOS_H_
#define HEADERSC_CONFIGPERIFERICOS_H_

// Cosntantes generales
#define MHz 1000000
#define CPU_1 0

// Parametros del funcion EPWM_Config();
#define max_Freq 3125000
#define min_Freq 0.42
#define EPWM_HDIV 2
#define EPWM_LDIV 2

//Contantes de DAC_Config();
#define vref 1

// Estructura para editar los valores de EPWM
volatile struct EPWM_REGS* EPWM_Regs[12] = {&EPwm1Regs,
                                            &EPwm2Regs,
                                            &EPwm3Regs,
                                            &EPwm4Regs,
                                            &EPwm5Regs,
                                            &EPwm6Regs,
                                            &EPwm7Regs,
                                            &EPwm8Regs,
                                            &EPwm9Regs,
                                            &EPwm10Regs,
                                            &EPwm11Regs,
                                            &EPwm12Regs};

// Estructura para editar los valores del ADC y DAC
volatile struct ADC_REGS* ADC_Regs[2] = {&AdcaRegs, &AdcbRegs};
volatile struct DAC_REGS* DAC_Regs[3] = {&DacaRegs, &DacbRegs, &DaccRegs};

// Estructura para editar los valores del SCI
volatile struct SCI_REGS* SCI_Regs[2] = {&ScibRegs, &ScicRegs};

// Obtiene la velocidad del DSP devuelta en MHz tomando como base el cristal de 10MHz
int freqOSC(void){
    int xtal = 10;
    int IMult = ClkCfgRegs.SYSPLLMULT.bit.IMULT;
    int FMult = ClkCfgRegs.SYSPLLMULT.bit.FMULT;
    int DivSel = ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV << 1;
    int freq = xtal * (IMult + FMult) / DivSel;
    //__asm("\tESTOP0");
    return freq;
}

/* Habilita los EPWM con una frecuencia de muestreo
 * EPWM     -> Numero del EPWM a usar
 * Freq     -> Frecuencia
 * ADC_EN   -> Habilita el EPWMxA como SOC
 * cicloT   -> Ciclo de trabajo del comparador A
*/
void EPWM_Config(Uint16 EPWM, float Freq, Uint16 ADC_EN, float cicloT){
    // Operador ternario para el condicional
    //__asm("\tESTOP0");
    bool ternario = (EPWM < 13) && (Freq > min_Freq && Freq < max_Freq) && (cicloT <= 100 && cicloT >= 0) ? true : false;
    if(ternario){
        // Apaga el reloj del modulo para realizar la configuracion
        CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
        EALLOW;
        EPWM_Regs[EPWM]->TBCTL.bit.CTRMODE = 0x0;
        //El Epwm se usara como SOC
        if(!ADC_EN){
            EPWM_Regs[EPWM]->ETSEL.bit.SOCAEN  = 0;
            EPWM_Regs[EPWM]->ETSEL.bit.SOCASEL = 4;
            EPWM_Regs[EPWM]->ETPS.bit.SOCAPRD  = 1;
        }
        //El Epwm se usara convencionalmente
        else{
            EPWM_Regs[EPWM]->AQCTLA.bit.ZRO = 0x01;
            EPWM_Regs[EPWM]->AQCTLA.bit.CAU = 0x02;
            EPWM_Regs[EPWM]->ETSEL.bit.INTSEL = 0x02;
            EPWM_Regs[EPWM]->ETSEL.bit.INTEN = 1;
            EPWM_Regs[EPWM]->ETPS.bit.INTPRD = 0x01;
        }
        //Establece preescaladores
        if( Freq >  48){
            EPWM_Regs[EPWM]->TBCTL.bit.HSPCLKDIV = EPWM_HDIV;      // Preescalador 1 del EPWMCLK (0x2 = 4)
            EPWM_Regs[EPWM]->TBCTL.bit.CLKDIV    = EPWM_LDIV;      // Preescalador 2 de EPWCLK (0x2 = 4)
        }
        else{
            EPWM_Regs[EPWM]->TBCTL.bit.HSPCLKDIV = 0x07;      // Preescalador 1 del EPWMCLK (0x7 = 14)
            EPWM_Regs[EPWM]->TBCTL.bit.CLKDIV    = 0x07;      // Preescalador 2 de EPWCLK (0x7 = 128)
        }
                                                               // TBCLK = EPWCLK / (CLKDIV*HSPCLKDIV)
        //Establece frecuencia del EPWM
        float base = (float)MHz*freqOSC()/(ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV << 1);
        base = base / (  ( 2 * EPWM_Regs[EPWM]->TBCTL.bit.HSPCLKDIV) * ( 1 << EPWM_Regs[EPWM]->TBCTL.bit.CLKDIV ) );
        float f = base/Freq;
        float fT = f*cicloT/100;

        EPWM_Regs[EPWM]->TBPRD = (Uint16)f;        // Frecuencia del EPWM
        EPWM_Regs[EPWM]->CMPA.bit.CMPA = (Uint16)fT; // Valor del comparador A
        EDIS;
    }
    else{
        __asm("\tESTOP0");
        // Existió un error al configurar el EPWM
    }
}

/*
 * Modifica la frecuencia del EPWM
 */
void modifFreqEPWM(Uint16 EPWM, float Freq, float cicloT){
    //Establece frecuencia del EPWM
    float base = (float)MHz*freqOSC()/(ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV << 1);
    base = base / (  ( 2 * EPWM_Regs[EPWM]->TBCTL.bit.HSPCLKDIV) * ( 1 << EPWM_Regs[EPWM]->TBCTL.bit.CLKDIV ) );
    float f = base/Freq;
    float fT = f*cicloT/100;
    EALLOW;
    EPWM_Regs[EPWM]->TBPRD = (Uint16)f;        // Frecuencia del EPWM
    EPWM_Regs[EPWM]->CMPA.bit.CMPA = (Uint16)fT; // Valor del comparador A
    EDIS;
}


/*  Configuración inicial de los ADC. Establece el preeescalador con un facor de cuatro,
 *  la reolución a 12 bits en modo SINGLE.
 *  Configura la interrupción en la conversión para realizarla al final de la trama,
 *  enciende el módulo interno y genera una pausa general de 1ms para habilitar los módulos.
 */
void ADC_Config(Uint16 ADC, Uint16 resolucion, Uint16 modo)
{
    if( ( ADC < 2 ) && ( resolucion < 2 ) && ( modo < 2 ) ){
        EALLOW;
        // Preescalador del ADCCLK /4
        ADC_Regs[ADC]->ADCCTL2.bit.PRESCALE = 6;

        // resolucion 0 -> 12 bits/1 -> 16 bits
        // modo       0 -> single /1 -> diferencial
        AdcSetMode(ADC, resolucion, modo);

        // Configura donde manda la interrupción del ADC
        // 1 -> interrupción al final de la conversión de datos
        ADC_Regs[ADC]->ADCCTL1.bit.INTPULSEPOS = 1;
        // Enciende el módulo interno del ADC
        ADC_Regs[ADC]->ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
        // Delay de 1ms para permitir el encendido del módulo
        DELAY_US(1000);
    }
    else{
        __asm("\tESTOP0");
        // Existió un error al configurar el ADC
    }

}

/*  Configuracion del SOC para el ADC seleccionado
 *  ADC -> Numero del ADC
 *  SOCx -> SOC a utilizar
 *  disparador del SOC
 *  canal del SOC
 */

void ADCSOC_Config(Uint16 ADC, Uint16 SOCx, Uint16 disparador, Uint16 canal){
    // Realiza comparacion para verificar que todo este en el rango
    if( (ADC < 2) && (SOCx < 16) && (disparador < 29) && (canal < 16) ){
        Uint16 acqps = 14;
        if(ADC_RESOLUTION_16BIT == ADC_Regs[ADC]->ADCCTL2.bit.RESOLUTION)
            acqps = 500; //75ns
        if (ADC == ADC_ADCA && canal == 13)
            acqps = 139; //320ns
        EALLOW;
        //Configuracion del SOC
        switch(SOCx){
        case 0:
            // Configura el canal en SOC0
            ADC_Regs[ADC]->ADCSOC0CTL.bit.CHSEL = canal;
            //Configura ventana de adquisicion de datos
            ADC_Regs[ADC]->ADCSOC0CTL.bit.ACQPS = acqps;
            //Configura disparador
            ADC_Regs[ADC]->ADCSOC0CTL.bit.TRIGSEL = disparador;
            break;
        case 1:
            // Configura el canal en SOC1
            ADC_Regs[ADC]->ADCSOC1CTL.bit.CHSEL = canal;
            ADC_Regs[ADC]->ADCSOC1CTL.bit.ACQPS = acqps;
            ADC_Regs[ADC]->ADCSOC1CTL.bit.TRIGSEL = disparador;
            break;
        /*case 2:
            // Configura el canal en SOC2
            ADC_Regs[ADC]->ADCSOC2CTL.bit.CHSEL = canal;
            if(ADC == 0 && canal == 13)
                ADC_Regs[ADC]->ADCSOC2CTL.bit.ACQPS = 139;
            else
                ADC_Regs[ADC]->ADCSOC2CTL.bit.ACQPS = acqps;
            ADC_Regs[ADC]->ADCSOC2CTL.bit.TRIGSEL = disparador;
            break;
        case 3:
            // Configura el canal en SOC3
            ADC_Regs[ADC]->ADCSOC3CTL.bit.CHSEL = canal;
            if(ADC == 0 && canal == 13)
                ADC_Regs[ADC]->ADCSOC3CTL.bit.ACQPS = 139;
            else
                ADC_Regs[ADC]->ADCSOC3CTL.bit.ACQPS = acqps;
            ADC_Regs[ADC]->ADCSOC3CTL.bit.TRIGSEL = disparador;
            break;
        case 4:
            // Configura el canal en SOC4
            ADC_Regs[ADC]->ADCSOC4CTL.bit.CHSEL = canal;
            if(ADC == 0 && canal == 13)
                ADC_Regs[ADC]->ADCSOC4CTL.bit.ACQPS = 139;
            else
                ADC_Regs[ADC]->ADCSOC4CTL.bit.ACQPS = acqps;
            ADC_Regs[ADC]->ADCSOC4CTL.bit.TRIGSEL = disparador;
            break;*/
        }

        // Habilita interrupciones. Se genera pulso al final del SOCx
        ADC_Regs[ADC]->ADCINTSEL1N2.bit.INT1SEL = SOCx;
        // Habilita la bandera ADCINT1
        ADC_Regs[ADC]->ADCINTSEL1N2.bit.INT1E = 1;
        // Limpia la bandera de interrupción
        ADC_Regs[ADC]->ADCINTFLGCLR.bit.ADCINT1 = 1;
        EDIS;
    }
    else{
        __asm("\tESTOP0");
        // Existió un error al configurar el SOC
    }

}

/* Configura el DAC con un valor de 3.0V como referencia
 * Solo habilita los registros correspondientes para utilizar
 * el ADCINA0 y ADCINA1 como salidas (Parte superior e inferior de la tarjeta)
 * El registro es "sombreado" (shadowed)
 */
void DAC_Config(Uint16 DAC){
    if( DAC < 3){
        // Configuracion de los DAC
        EALLOW;
        DAC_Regs[DAC]->DACCTL.bit.DACREFSEL = vref;
        DAC_Regs[DAC]->DACOUTEN.bit.DACOUTEN = 1;
        DAC_Regs[DAC]->DACVALS.all = 0;
        EDIS;
        // Delay para encender módulo
        DELAY_US(10);
    }
    else{
        __asm("\tESTOP0");
        // Existió un error al configurar el DAC
    }
}

/*  Configura el SCI seleccionado dependiendo del baudaje
 *  SCI     -> Numero del SCI
 *  Baudaje -> Valor del baudaje
 *  Se permiten configuirar el SCIb y SCIC ya que se encuentran en el Lauchpad
 */
void SCI_Config(Uint16 SCI, Uint32 baudaje)
{
    float LSPCLK = freqOSC() * MHz / ( 2 * ClkCfgRegs.LOSPCP.bit.LSPCLKDIV );
    if(SCI < 2 && ( ( baudaje <= (LSPCLK/16) ) && baudaje > (LSPCLK/65536) ) ){
        //Define transmisor y receptor
        Uint16 SCIxTx = 86, SCIxRx = 87, perif = 5;
        // Cambia los parametros para el SCIC
        if(SCI == 1)
            SCIxTx = 89, SCIxRx = 90, perif = 6;
        //Configura SCIRX
        GPIO_SetupPinMux(SCIxRx, CPU_1, perif);
        GPIO_SetupPinOptions(SCIxRx, GPIO_INPUT, GPIO_PUSHPULL);
        //Configura SCITX
        GPIO_SetupPinMux(SCIxTx, CPU_1, perif);
        GPIO_SetupPinOptions(SCIxTx, GPIO_OUTPUT, GPIO_ASYNC);

        SCI_Regs[SCI]->SCICCR.all = 0x0007;     // 1 stop bit,  No loopback
                                                // Sin paridad, 8 bits de longitud de palabra,
                                                // modo asíncrono.
        SCI_Regs[SCI]->SCICTL1.all = 0x0003;
        SCI_Regs[SCI]->SCICTL2.bit.TXINTENA = 1;    // Habilita banderas de interrupcion en
        SCI_Regs[SCI]->SCICTL2.bit.RXBKINTENA = 1;  // FIFO de entrada y salida

        //Calculo de la tasa de Bauds
        Uint16 BRR = (Uint16)( LSPCLK / ( baudaje * 8 ) ) - 1;
        SCI_Regs[SCI]->SCIHBAUD.all = BRR & 0xFF00;
        SCI_Regs[SCI]->SCILBAUD.all = BRR & 0x00FF;

        // Limpia la FIFO y la configura Tx
        SCI_Regs[SCI]->SCIFFTX.all = 0x8022;
        // Limpia FIFO y configura Rx
        // Nota: Desactivar mejoras del SCI, de otra manera no ejecuta la interrupción
        SCI_Regs[SCI]->SCIFFRX.all = 0x0021;

        //Limpia banderas e inicia FIFOs
        SCI_Regs[SCI]->SCICTL1.all = 0x0023;
        SCI_Regs[SCI]->SCIFFTX.bit.TXFIFORESET = 1;
        SCI_Regs[SCI]->SCIFFRX.bit.RXFIFORESET = 1;
    }
    else{
        __asm("\tESTOP0");
        // Existió un error al configurar el SCI
    }
}

#endif /* HEADERSC_CONFIGPERIFERICOS_H_ */
