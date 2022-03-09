*	Funciones para aplicar un filtros IIR pasa banda de orden 1 y 2

	.global _IIRFloat, _cleanFloat
	.global coeficientesIIR, _filtros

	.data

NIIR	.set 44000	; Frecuencia de muestreo
Nfilt	.set 10		;
Nprod	.set 9		; Numero de operaciones
space	.set 0
space1  .float 0
coefA	.set 4		; Coeficientes ai
coefB	.set 5		; Coeficientes bi
xVal1	.space coefB*32		; Vector xi
yVal1	.space 50*32		; Vector yi
spaceY1 .float 0.0
xVal2	.space coefB*32
yVal2	.space 50*32
spaceY2  .float 0.0
suma	.float 0

	.text

_IIRFloat
	MOVW DP,#xVal1
	SETC SXM
	SETC OVM
	SPM #0
	; Los datos se pasan por AL = canal y R0H = muestra, XAR4 coeff y XAR5 ganancia

	; Selecciona el vector dependiendo de la entrada
	MOV AH, #0		; ACC = canal
	MOV AH, AL
	BF filterVect, NEQ	;Realiza salto para evitar asignacion
	MOVL XAR3,#xVal1
	LB fVect
filterVect
	MOVL XAR3, #xVal2
fVect
	; Recorre el vector de Xval
	MOVL XAR7, XAR3			; XAR7 apuunta a Xval
	MOV AR0, #coefB-1		; Inicia ciclo para acomodo
DmXvect
	MOV32 R1H,*XAR7
	MOV32 *XAR7++,R0H
	MOV32 R0H,R1H
	BANZ DmXvect, AR0--
	;ESTOP0
	; El vector Xval ya está acomodado

	MOVL XAR6, XAR4			; Apunta a coeficientes
	MOVL XAR2, #suma		; Apunta a suma
	ZAPA
	MOVL *XAR2, ACC
	MOVL ACC, XAR3
	ADD ACC, #10
	MOVL XAR1, ACC			; Apunta a yval
	MOV AR0, #Nfilt-1		; Inicia ciclo de filtros
forFiltro
	MOVL XAR7, XAR3			; XAR3 = XAR7 = #xVal
	MOV32 R6H, @space1
    MOV32 R2H, @space1
	MOV32 R3H, @space1
	MOV32 R7H, @space1
	MOV32 R0H, @space1
	NOP
	RPT #coefB-1
	||MACF32	R7H, R3H, *XAR6++, *XAR7++
	NOP
	ADDF32 R7H, R7H, R6H
	NOP
	ADDF32 R3H, R3H, R7H
	NOP
	MOVL XAR7, XAR1
	MOV32 R7H, @space1
	NOP
	RPT #coefB-1
	||MACF32	R7H, R3H, *XAR6++, *XAR7++
	NOP
	ADDF32 R7H, R7H, R6H
	NOP
	ADDF32 R0H, R3H, R7H	; Se retorna el valor por R0H
	NOP
	;Mueve el vector Yx
	MOV32 R3H,R0H
	; Ordena el resultado del vector y
	MOV AR7, #coefA-1
DmYvect
	MOV32 R1H,*XAR1
	MOV32 *XAR1++,R0H
	MOV32 R0H,R1H
	BANZ DmYvect, AR7--

	MOV32 R0H, R3H
	MOV32 R1H, *XAR5++
	MPYF32 R0H, R0H, R1H
	NOP
	NOP
	; Guarda el valor en suma
	MOV32 R1H, *XAR2
	ADDF32 R0H, R0H, R1H	; Suma los valores
	NOP
	MOVL ACC, *XAR1++
	MOV32 *XAR2, R0H

	BANZ forFiltro, AR0--

	MOV32 R0H, *XAR2		; R0H = suma total
	LRETR

_cleanFloat
	MOVW DP,#xVal1
	MOVL XAR7, #xVal1
	RPT #200-1
	||MOV *XAR7++,#0
	LRETR

	.end
