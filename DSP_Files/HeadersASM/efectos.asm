
*	Funcion en lenguaje ensamblador para aplicar efectos de retardo

	.global _delay, _reverb, _movSignal, _cleanSignal

	.data
N	 	 .set 12000		; Muestras maximas para delay del efecto Chorus-Echo
step	 .set 800
buff1 	 .space N*16	; Almacena los valores del delay
buff2	 .space N*16

	.text
	;Funcion que se usa para reacomodar el vector que guarda los valores del delay
_movSignal
	MOVW DP,#buff1			; Direcciona a la pagina del resultado

	; Al tener el paso de dos int se utilizan AL=N y AH=x(i), XAR4 con el canal
	MOV AR0,AL				;Asignamos ARO = N
	MOV AR2,AH				;Asignamos AR2 = x(i)
	MOV AH,#0				;Asignamos ACC = N

	MOVL ACC, XAR4			;Decide que vector va a utilizar
	BF MovVectD,NEQ
	MOVL XAR7,#buff1
	LB VectD
MovVectD
	MOVL XAR7,#buff2
VectD
	ZAPA
	MOV AL,AR0
	ADDL ACC,XAR7			;Calcula la direccion final del vector de delay ACC = #delay + N
	MOVL  XAR7,ACC			;Apunta al final del vector de delay
	RPT AR0
	||DMOV *--XAR7 			;La instruccion DMOV toma el valor actual y
							; lo coloca en la localidad anterior de 16 bits
	MOVL ACC, XAR4			;Decide que vector va a utilizar
	BF MovVectD2,NEQ
	MOVL XAR7,#buff1
	LB VectD2
MovVectD2
	MOVL XAR7,#buff2
VectD2
	MOV *XAR7, AR2			;Copia el valor de la senal x(i) en inicio del vector delay
	LRETR

; Delay
_delay
	SETC SXM			; Habilita la aritmetica de signo extendido
	SETC OVM			; Habilita las operaciones con overflow
	SPM #0
	MOVW DP,#buff1		; Direcciona a la pagina del resultado

	; Al tener el paso de dos int se utilizan AL=N y AH=x(i) y xar4 para el vector a usar
	MOV AR2, AH				;Asignamos AR2 = x(i)
	MOV AH,#0				;Asignamos ACC = N
	MOV AR5, AL

	MOVL ACC, XAR4
	BF delayVect,NEQ
	MOVL XAR7,#buff1
	LB dVect
delayVect
	MOVL XAR7,#buff2
dVect
	MOV AL,AR5
	ADDL ACC,XAR7			;Calcula la direccion final del vector de delay ACC = #delay + N
	MOVL  XAR7,ACC			;Apunta al final del vector de delay en XAR7
	MOV AL, *XAR7			;Asigna a ACC el valor final de delay
	ASR	AL, #1				;Divide el valor de final delay a la mitad
	ADD AL, AR2				;Suma el valor final de delay *0.5 + x(i)

	;El dato es regresado en AL
	LRETR

_reverb
	SETC SXM			; Habilita la aritmetica de signo extendido
	SETC OVM			; Habilita las operaciones con overflow
	SPM #0
	MOVW DP,#buff1	; Direcciona a la pagina del resultado

	; Al tener el paso de dos int se utilizan AL=N y AH=x(i), XAR4 con el canal y XAR5 el vector de coef
	MOV AR2, AH			; AR2 contiene x(i)
	MOV AR3, AL 		; AR3 tiene la cantidad de valores

	ZAPA
	MOVL ACC, XAR4
	BF reverbVect,NEQ
	MOVL XAR7,#buff1
	MOV AR3,#10-1		; Cantidad de ciclos
	LB rVect
reverbVect
	MOVL XAR7,#buff2
	MOV AR3,#15-1
rVect
	MOVL ACC, XAR7		; Apunta a la primera localidad del vector delay
	ADD	 AL, #step		; Apunta a la primera localidad del vector delay + step
	MOVL XAR7, ACC
	ZAPA
cicloRev
	MAC  P,*XAR5++,*XAR7
	MOVL XAR0, ACC
	MOVL ACC,XAR7
	ADD AL,#step
	MOVL XAR7,ACC
	MOVL ACC,XAR0
	BANZ cicloRev,AR3--
	ADDL  ACC,P
	LSL	ACC, #4			; Corrimiento para coreccion en punto fijo
	MOV AL, AH		    ; Q12*Q12 = Q24 -> Q24-16 = 8
	ADD AL, AR2
	;El dato es regresado en AL
	LRETR

	;Funcion para dejar en ceros a los vectores delay
_cleanSignal
	MOVW DP,#buff1
	MOVL XAR7,#buff1
	MOV AR0,AL
	RPT AR0
	||MOV *XAR7++, #0
	MOVL XAR7,#buff2
	MOV AR0,AL
	RPT AR0
	||MOV *XAR7++, #0
	LRETR
	.end
