#include "lock.h"
#include "ring_buffer.h"
#include "keypad.h"
#include "main.h"
#include "gui.h"
#include "PWM_cont.h"
#include "ssd1306.h"

#include <stdio.h>
#include <string.h>


#define MAX_PASSWORD 12


uint8_t password[MAX_PASSWORD] = "1992";

uint8_t keypad_buffer[MAX_PASSWORD];

ring_buffer_t keypad_rb;


extern volatile uint16_t keypad_event;



static uint8_t lock_get_passkey(void)
{
	while (ring_buffer_size(&keypad_rb) == 0) {
		/* wait for key press */
		uint8_t key_pressed = keypad_run(&keypad_event);
		if (key_pressed != KEY_PRESSED_NONE) {
			ring_buffer_put(&keypad_rb, key_pressed);
		}
	}
	uint8_t key_pressed;
	ring_buffer_get(&keypad_rb, &key_pressed);
	if (key_pressed == '*' || key_pressed == '#') {
		return 0xFF;
	}
	return key_pressed;
}

static uint8_t lock_get_password(void)
{
	uint8_t idx = 0;
	uint8_t passkey = 0;
	uint8_t new_password[MAX_PASSWORD];
	memset(new_password, 0, MAX_PASSWORD);
	uint8_t password_shadow[MAX_PASSWORD + 1]  = {
			'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '\0'
	};
	while (passkey != 0xFF) {
		GUI_update_password(password_shadow);
		passkey = lock_get_passkey();
		password_shadow[idx] = '*';
		new_password[idx++] = passkey;
		GUI_update_password(new_password);
		HAL_Delay(200);
	}

	if (idx > 1) {
		memcpy(password, new_password, MAX_PASSWORD);
		GUI_update_password_success();
	} else {
		GUI_locked();
		return 0;
	}
	return 1;
}

static uint8_t lock_validate_password(void)
{
	uint8_t sequence[MAX_PASSWORD];
	uint8_t seq_len = ring_buffer_size(&keypad_rb);
	for (uint8_t idx = 0; idx < seq_len; idx++) {
		ring_buffer_get(&keypad_rb, &sequence[idx]);
	}
	if (memcmp(sequence, password, 4) == 0) {

		return 1;
	}
	return 0;
}

static void lock_update_password(void)
{
	if (lock_validate_password() != 0) {
		GUI_update_password_init();
		lock_get_password();
	} else {
		GUI_locked();
		PWM_SendClose();

	}
}

static void lock_open_lock(void)
{
	if (lock_validate_password() != 0) {
		GUI_unlocked();
		PWM_SendOpen();

	} else {
		GUI_locked();
		PWM_SendClose();
}
}

void lock_init(void)
{
	ring_buffer_init(&keypad_rb, keypad_buffer, 12);
	GUI_init();
	PWM_cont_Init();
}

void lock_sequence_handler(uint8_t key)
{
	static uint8_t m = 20; // Posición inicial en X para los caracteres
	 static uint8_t first_key_pressed = 1; // Bandera para la primera tecla presionada
	 static uint8_t first_num=1;

	if (key == '*') {
		lock_update_password();
		first_key_pressed = 1; // Reiniciar la bandera para la próxima secuencia
		m = 20;
		first_num = 1;
	} else if (key == '#') {
		lock_open_lock();
		m = 20;
	    first_key_pressed = 1; // Reiniciar la bandera para la próxima secuencia
	    first_num = 1;


	}else if(key == 'D'){
		PWM_Stop();
	}else {
		if (first_key_pressed) {
				 first_key_pressed = 0; // La pantalla se ha limpiado, actualizar la bandera
				 if (first_num != 0){
					 ssd1306_Fill(Black); // Limpiar la pantalla al inicio de una nueva secuencia
					 ssd1306_UpdateScreen(); // Actualizar la pantalla después de limpiarla
					 first_num = 0;
				 }
			        }
		ring_buffer_put(&keypad_rb, key);
		GUI_display_key((char)key, m, 45);
		HAL_Delay(200);
		GUI_display_key('*', m, 45); // Mostrar '*' en lugar del carácter real
		m += 12; // Mover la posición X para el siguiente carácter
	}




}

uint8_t ultrasonicSensorEnabled = 0; // Variable to check the state of the ultrasonic sensor.

void lock_control_ultrasonic_sensor(uint8_t key) {
    if (key == 'B') {
        // Enables the ultrasonic sensor
    	ultrasonicSensorEnabled = 1;
        return ;
    } else if (key == 'C') {
        // turn off the sensor
    	ultrasonicSensorEnabled = 0;
        return ;
    }
}
