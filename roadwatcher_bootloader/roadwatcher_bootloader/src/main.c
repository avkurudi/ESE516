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
#include <stdio.h>
#include <string.h>

#define DATA_LENGTH 1
static uint8_t wr_buffer[DATA_LENGTH] = {
	0x00
};
//static uint8_t wr_buffer_reversed[DATA_LENGTH] = {
//	0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
//};

#define CONF_I2C_MASTER_MODULE SERCOM2

struct i2c_master_module i2c_master_instance;

struct i2c_master_packet wr_packet;
struct i2c_master_packet rd_packet;



static uint8_t rd_buffer[DATA_LENGTH];

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
//	while (stdio_serial_init(&usart_instance, SERCOM3, &config_usart) != STATUS_OK){}
	stdio_serial_init(&usart_instance, SERCOM3, &config_usart);
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
void configure_i2c(void)
{
	/* Initialize config structure and software module */
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	/* Change buffer timeout to something longer */
	config_i2c_master.buffer_timeout = 100;
	/* Initialize and enable device with config */
	while(i2c_master_init(&i2c_master_instance, CONF_I2C_MASTER_MODULE,
	&config_i2c_master) \
	!= STATUS_OK);
	i2c_master_enable(&i2c_master_instance);
}


void i2c_write_complete_callback(struct i2c_master_module *const module)
{
	/* Initiate new packet read */
	i2c_master_read_packet_job(&i2c_master_instance,&rd_packet);
}

void configure_i2c_callbacks(void)
{
	/* Register callback function. */
	i2c_master_register_callback(&i2c_master_instance,
	i2c_write_complete_callback,
	I2C_MASTER_CALLBACK_WRITE_COMPLETE);
	i2c_master_enable_callback(&i2c_master_instance,
	I2C_MASTER_CALLBACK_WRITE_COMPLETE);
}



int main (void)
{
	system_init();
	
	configure_usart();
	configure_usart_callbacks();

	/* Configure device and enable. */
	configure_i2c();
	/* Configure callbacks and enable. */
	configure_i2c_callbacks();

	delay_init();
	
	system_interrupt_enable_global();
	while(true)
	{
		for(int i=0;i<128;i++)
		{
			wr_packet.address=i;
			wr_packet.data_length=DATA_LENGTH;
			wr_packet.data=wr_buffer;
			enum status_code i2c_status_job = i2c_master_write_packet_wait(&i2c_master_instance, &wr_packet);
			delay_ms(1);
			//printf("I2C srite status %x= %d\n\r", i, i2c_status_job);
			if(i2c_status_job == 0) printf("Ack at: %x\n\r", i);
//			i2c_master_read_packet_job(&i2c_master_instance, &rd_packet);
//			printf(rd_packet.data);		
		}	
	}
	uint8_t string[] = "Hello World!\r\n";
	usart_write_buffer_wait(&usart_instance, string, sizeof(string));
	printf("Hi from printf!\n");
	///*
	char str1[1];
	while(true)
	{
		scanf("%s", str1);
		printf("%s\n", str1);
	}
	//*/

/*	while (true) {
			// Send every other packet with reversed data 
			if (wr_packet.data[0] == 0x00) {
				wr_packet.data = &wr_buffer_reversed[0];
				} else {
				wr_packet.data = &wr_buffer[0];
			}
			i2c_master_write_packet_job(&i2c_master_instance, &wr_packet);
	}*/




	
	/* Insert application code here, after the board has been initialized. */
}
