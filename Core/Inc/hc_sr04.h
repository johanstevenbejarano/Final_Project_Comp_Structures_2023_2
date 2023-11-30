/*
 * hc_sr04.h
 *
 *  Created on: Nov 26, 2023
 *      Author: ANDRES CUELLAR
 */

#ifndef INC_HC_SR04_H_
#define INC_HC_SR04_H_

#include "main.h"

// Definiciones de pines para el sensor HC-SR04
#define HC_SR04_TRIG_PIN GPIO_PIN_1  // Pin PA1 como Trigger
#define HC_SR04_TRIG_PORT GPIOA
#define HC_SR04_ECHO_PIN GPIO_PIN_4  // Pin PA4 como Echo
#define HC_SR04_ECHO_PORT GPIOA

// Prototipos de funciones
void HC_SR04_Init(void);
void HC_SR04_Trigger(void);
uint32_t HC_SR04_Read(void);
void microDelay(uint32_t us);
uint32_t micros(void);

#endif /* INC_HC_SR04_H_ */
