/*
Este archivo define el vector de reset, es decir, la dirección 
donde comienza la ejecución cuando el procesador arranca o se reinicia.
 */

.global  _reset_vector 
.extern  _start
.code 	 32
.section .reset//Secciones de entrada

_reset_vector:
	LDR PC, = _start
.end

/*
.global _reset_vector: Hace visible el símbolo _reset_vector para otros archivos.

.extern _start: Indica que la etiqueta _start está en otro archivo.

.section .reset: Coloca el vector de reset en una sección especial.

_reset_vector: B _start: Cuando el procesador arranca, salta (branch) a _start para comenzar la ejecución principal.
 */