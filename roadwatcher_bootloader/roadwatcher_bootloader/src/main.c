/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

struct usart_module usart_instance;#define MAX_RX_BUFFER_LENGTH 5
volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];void usart_read_callback(struct usart_module *const usart_module)
{usart_write_buffer_job(&usart_instance,
	(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
}
void usart_write_callback(struct usart_module *const usart_module)
{
//	port_pin_toggle_output_level(LED_0_PIN);
}void configure_usart(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate = 9600;
	config_usart.mux_setting = USART_RX_3_TX_2_XCK_3;
	config_usart.pinmux_pad2 = PINMUX_PA20D_SERCOM3_PAD2;
	config_usart.pinmux_pad3 = PINMUX_PA21D_SERCOM3_PAD3;
	while (usart_init(&usart_instance,
	SERCOM3, &config_usart) != STATUS_OK) {
	}
	usart_enable(&usart_instance);
}
void configure_usart_callbacks(void)
{
	usart_register_callback(&usart_instance,
	usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_instance,
	usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&usart_instance,
	USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
}
int main (void)
{
	system_init();
	
	configure_usart();
	configure_usart_callbacks();
	
	system_interrupt_enable_global();
	uint8_t string[] = "Hello World!\r\n";
	usart_write_buffer_wait(&usart_instance, string, sizeof(string));
	while (true) {
		usart_read_buffer_job(&usart_instance,
		(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
	}
	
	/* Insert application code here, after the board has been initialized. */
}
