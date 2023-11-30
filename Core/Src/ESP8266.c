#include "esp8266.h"
#include "main.h"



void Rx_USART1_init(USART1_t *ring_buffer, uint8_t *buffer, uint16_t capacity)
{
	ring_buffer->buffer = buffer;
	ring_buffer->capacity = capacity;

	ring_buffer->head = 0;
	ring_buffer->tail = 0;
	ring_buffer->is_full = 0;
}
uint8_t Rx_USART1_put(USART1_t *ring_buffer, uint8_t data)
{
	ring_buffer->buffer[ring_buffer->head] = data;
	ring_buffer->head = (ring_buffer->head + 1) % ring_buffer->capacity;

	if (ring_buffer->is_full != 0) {
		ring_buffer->tail = (ring_buffer->tail + 1) % ring_buffer->capacity;
	}

	if (ring_buffer->head == ring_buffer->tail) {
		ring_buffer->is_full = 1;
		return 0;
	}
	return 1;
}
// La definición de la función aquí
void Rx_USART1(USART1_t *ring_buffer) {
    if (LL_USART_IsActiveFlag_RXNE(USART1)){
        uint8_t received_data = LL_USART_ReceiveData8(USART1);
        Rx_USART1_put(ring_buffer, received_data); // Sin desreferenciar
        //printf("data %x \r\n", received_data);
    }
}
