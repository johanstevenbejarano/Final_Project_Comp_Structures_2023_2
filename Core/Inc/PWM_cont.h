/*
 * PWM_cont.h
 *
 *  Created on: Nov 25, 2023
 *      Author: ANDRES CUELLAR
 */

#ifndef INC_PWM_CONT_H_
#define INC_PWM_CONT_H_


#include <stdint.h>
// Inicializa la configuración PWM.
void PWM_cont_Init(void);

// Inicia la secuencia para enviar SOS en código Morse.
void PWM_SendSOS(void);

// Inicia la secuencia para enviar OPEN en código Morse.
void PWM_SendOpen(void);

// Inicia la secuencia para enviar CLOSE en código Morse.
void PWM_SendClose(void);

void PWM_Stop(void);

// Actualiza el estado de la máquina de estados Morse.
void PWM_Morse_Update(void);

#endif /* INC_PWM_CONT_H_ */
