#include "hc_sr04.h"
#include "stm32l4xx_hal.h"



// Definitions for error and time management.
#define HC_SR04_TIMEOUT 100  // Timeout in milliseconds
#define SOUND_SPEED 0.0343   // Speed of sound in cm/us


/**
  * @brief  This function creates a precise delay in microseconds.
  *  It's used to generate the precise timing required for the ultrasonic sensor.
  */

void microDelay(uint32_t us) {
	// Save the current counter value
    uint32_t start = LL_TIM_GetCounter(TIM3);

    // Calculate the end value by adding microseconds to the counter
    uint32_t end = start + us;

    // Check for counter overflow
    if (end < start) {

    	 // Wait for the counter to reach its maximum value in case of overflow
        while (LL_TIM_GetCounter(TIM3) > start) {}

        start = 0; // Reset the start count after overflow
    }

    // Wait until the counter reaches the final value
    while (LL_TIM_GetCounter(TIM3) < end) {}
}


/**
  * @brief This function returns the current count of the timer (TIM3), which is used to calculate time intervals in microseconds.
  */
uint32_t micros(void) {
	 // Read the current counter value
    return LL_TIM_GetCounter(TIM3);
}



/**
  * @brief  This function initializes the GPIO pins used for the HC-SR04 ultrasonic sensor.
  * The Trigger pin is set as an output, and the Echo pin is set as an input.
  */
void HC_SR04_Init(void) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable the clock for the corresponding GPIO ports
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    // Configure the Trigger pin as output
    GPIO_InitStruct.Pin = HC_SR04_TRIG_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    LL_GPIO_Init(HC_SR04_TRIG_PORT, &GPIO_InitStruct);

    // Configure the Echo pin as input
    GPIO_InitStruct.Pin = HC_SR04_ECHO_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(HC_SR04_ECHO_PORT, &GPIO_InitStruct);
}


/**
  * @brief  This function generates a 10-microsecond pulse on the Trigger pin.
  * This pulse is required to start a measurement with the HC-SR04 sensor.
  */

void HC_SR04_Trigger(void) {
	// Ensure that Trigger is low
    LL_GPIO_ResetOutputPin(HC_SR04_TRIG_PORT, HC_SR04_TRIG_PIN);
    microDelay(2); // Wait to stabilize the sensor

    // Generate a 10-microsecond pulse
    LL_GPIO_SetOutputPin(HC_SR04_TRIG_PORT, HC_SR04_TRIG_PIN);
    microDelay(10); // Precise 10us delay
    LL_GPIO_ResetOutputPin(HC_SR04_TRIG_PORT, HC_SR04_TRIG_PIN);
}


/**
  * @brief  Manages the process of sending a pulse and measuring the time until the Echo pin changes state.
  * It calculates the distance based on the time elapsed and the speed of sound.
  *  The function returns the measured distance or 0 if a timeout occurs,
  *  indicating an error or that no object was detected within range.
  */

uint32_t HC_SR04_Read(void) {
    uint32_t startTick, endTick;
    uint32_t timeoutTick = HAL_GetTick() + HC_SR04_TIMEOUT;

    // Send the ultrasonic pulse
    HC_SR04_Trigger();

    // Wait for Echo to be high or until timeout is reached
    while (HAL_GPIO_ReadPin(HC_SR04_ECHO_PORT, HC_SR04_ECHO_PIN) == GPIO_PIN_RESET) {
        if (HAL_GetTick() > timeoutTick) {
            return 0; // Return 0 as an error or timeout indicator
        }
    }
    startTick = micros();

    // Wait for Echo to be low or until timeout is reached
    while (HAL_GPIO_ReadPin(HC_SR04_ECHO_PORT, HC_SR04_ECHO_PIN) == GPIO_PIN_SET) {
        if (micros() - startTick > HC_SR04_TIMEOUT * 1000) {
            return 0; // Return 0 as an error or timeout indicator
        }
    }
    endTick = micros();

    // Calculate the distance
    return ((endTick - startTick) * SOUND_SPEED)/2;
}
