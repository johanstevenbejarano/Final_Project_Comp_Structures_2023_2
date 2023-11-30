#include "PWM_cont.h"
#include "stm32l4xx_hal.h"


// Definiciones de los tiempos.
#define DOT_DURATION 150
#define DASH_DURATION 800
#define ELEMENT_GAP 500
#define LETTER_GAP 1000
#define WORD_GAP 1400

// Estados para la máquina de estados.
typedef enum {
    PWM_STATE_IDLE,
    PWM_STATE_DOT,
    PWM_STATE_DASH,
    PWM_STATE_GAP,
    PWM_STATE_END
} PWM_State;

// Estructura para manejar la secuencia Morse.
typedef struct {
    PWM_State state;
    uint32_t lastUpdateTime;
    const char *sequence;
    uint32_t sequenceIndex;
} PWM_Morse;

static PWM_Morse morseController = {PWM_STATE_IDLE, 0, NULL, 0};

// Prototipos de funciones privadas.
static void PWM_SetDutyCycle(uint32_t dutyCycle);
static void PWM_UpdateState(void);

void PWM_cont_Init(void) {
	 // Enables the clock for the GPIO port and the timer TIM2.
	    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	    // Configs PA0 as an alternative output for TIM2_CH1.
	    GPIOA->MODER &= ~GPIO_MODER_MODE0;  // Cleans the bits of mode for PA0.
	    GPIOA->MODER |= GPIO_MODER_MODE0_1; // Configs as alternative mode.
	    GPIOA->AFR[0] |= (1 << (4 * 0));    // Configs AF1 for PA0 (TIM2_CH1).

	    // Configs the timer TIM2.
	    TIM2->PSC = 79;                     // Establish the pre scaler.
	    TIM2->ARR = 999;                  // Establish the period.
	    TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // PWM mode for CH1.
	    TIM2->CCER |= TIM_CCER_CC1E;        // Enables the channel 1.

	    // Starts the timer.
	    TIM2->CR1 |= TIM_CR1_CEN;
}

void PWM_SendSOS(void) {
    morseController.sequence = "...---...";
    morseController.sequenceIndex = 0;
    morseController.state = PWM_STATE_DOT;
    PWM_SetDutyCycle(60000); // Iniciar el PWM con el primer símbolo.
}

void PWM_SendOpen(void) {
    morseController.sequence = "--- .--. . -."; // Secuencia Morse para "OPEN"
    morseController.sequenceIndex = 0;
    morseController.state = PWM_STATE_DOT;
    PWM_SetDutyCycle(60000); // Iniciar el PWM con el primer símbolo.
}

void PWM_SendClose(void) {
    morseController.sequence = "-.-. .-.. --- ... ."; // Secuencia Morse para "CLOSE"
    morseController.sequenceIndex = 0;
    morseController.state = PWM_STATE_DOT;
    PWM_SetDutyCycle(60000); // Iniciar el PWM con el primer símbolo.
}

void PWM_Stop(void) {
	PWM_SetDutyCycle(0);  // Apaga el PWM
	morseController.state = PWM_STATE_END;
}

void PWM_Morse_Update(void) {
	    uint32_t currentTime = HAL_GetTick();
	    uint32_t elapsed = currentTime - morseController.lastUpdateTime;
	    char currentSymbol = morseController.sequence[morseController.sequenceIndex];

	    if ((morseController.state == PWM_STATE_DOT && elapsed >= DOT_DURATION) ||
	        (morseController.state == PWM_STATE_DASH && elapsed >= DASH_DURATION) ||
	        (morseController.state == PWM_STATE_GAP && elapsed >= ELEMENT_GAP)) {
	        PWM_UpdateState();
	        morseController.lastUpdateTime = currentTime;
	    }
	}

static void PWM_UpdateState(void) {
	  char currentSymbol = morseController.sequence[morseController.sequenceIndex];

	    switch (morseController.state) {
	        case PWM_STATE_DOT:
	        case PWM_STATE_DASH:
	            PWM_SetDutyCycle(0); // Apagar el PWM
	            morseController.state = PWM_STATE_GAP; // Cambiar al estado GAP
	            morseController.lastUpdateTime = HAL_GetTick();
	            break;

	        case PWM_STATE_GAP:
	            if (currentSymbol != '\0') {
	                morseController.sequenceIndex++; // Avanzar al siguiente símbolo
	                currentSymbol = morseController.sequence[morseController.sequenceIndex];
	            }

	            if (currentSymbol == '.') {
	                PWM_SetDutyCycle(60000); // Encender para un punto
	                morseController.state = PWM_STATE_DOT;
	            } else if (currentSymbol == '-') {
	                PWM_SetDutyCycle(60000); // Encender para una raya
	                morseController.state = PWM_STATE_DASH;
	            } else if (currentSymbol == '\0') { // Fin de la secuencia
	                morseController.state = PWM_STATE_END;
	            }
	            morseController.lastUpdateTime = HAL_GetTick();
	            break;

	        case PWM_STATE_END:
	            // Opcional: Manejo para cuando se termina la secuencia
	            break;

	        default:
	            morseController.state = PWM_STATE_IDLE;
	            break;
	    }
	}

// Function to set the duty cycle for the PWM signal
void PWM_SetDutyCycle(uint32_t dutyCycle) {
    // Clamp duty cycle to maximum value if needed
    if (dutyCycle > 65535) {
        dutyCycle = 65535;
    }

    // Set the duty cycle for TIM2 Channel 1
    TIM2->CCR1 = dutyCycle;
}
