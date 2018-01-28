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
#include <string.h>

#define DATA_LENGTH 1
static uint8_t wr_buffer[DATA_LENGTH] = {
	0x00
};
static uint8_t rd_buffer[DATA_LENGTH];
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
	while(i2c_master_init(&i2c_master_instance, CONF_I2C_MASTER_MODULE, &config_i2c_master)	!= STATUS_OK);
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

void i2c_scanner()
{
	//TODO: make this a proper function, with arguments and returns (likely an array of ints w/ I2C addresses)
	for(int i=0;i<128;i++)
	{
		wr_packet.address=i;
		wr_packet.data_length=DATA_LENGTH;
		wr_packet.data=wr_buffer;
		enum status_code i2c_status_job = i2c_master_write_packet_wait(&i2c_master_instance, &wr_packet);
		delay_ms(1);
		//printf("I2C write status %x= %d\n\r", i, i2c_status_job);
		if(i2c_status_job == 0) printf("Ack at: Ox%x\n\r", i);
		//			i2c_master_read_packet_job(&i2c_master_instance, &rd_packet);
		//			printf(rd_packet.data);
	}
}

void i2c_accel()
{
	wr_packet.address=0x53;
	wr_packet.data_length=1;
	wr_packet.data=wr_buffer;
	rd_packet.address=0x53;
	rd_packet.data_length=1;
	rd_packet.data=rd_buffer;
	enum status_code i2c_status_job_write = i2c_master_write_packet_wait(&i2c_master_instance, &wr_packet);
	delay_us(10);
	enum status_code i2c_status_job_read = i2c_master_read_packet_wait(&i2c_master_instance, &rd_packet);
	printf("I2C write status %x= %d\n\r", 0x53, i2c_status_job_write);
	printf("I2C read status %x= %d\n\r", 0x53, i2c_status_job_read);
	if(i2c_status_job_read == 0) printf(" Return Value: 0x%x\n\r", rd_buffer[0]);
	//i2c_master_read_packet_wait(&i2c_master_instance, &rd_packet);
	delay_ms(1);
}


struct adc_module adc_instance;

void init_adc(void)
{
	struct adc_config config_adc;
	adc_get_config_defaults(&config_adc);
	config_adc.reference = ADC_REFERENCE_INT1V;
	adc_init(&adc_instance, ADC, &config_adc);
	adc_enable(&adc_instance);	
}

void configure_adc(void)
{
	adc_disable(&adc_instance);
	struct adc_config config_adc;
	adc_get_config_defaults(&config_adc);
	config_adc.reference = ADC_REFERENCE_INT1V;
	adc_init(&adc_instance, ADC, &config_adc);
	adc_enable(&adc_instance);
}

void configure_adc_temp(void)
{
	adc_disable(&adc_instance);
	struct adc_config config_adc_temp;
	adc_get_config_defaults(&config_adc_temp);
	config_adc_temp.reference = ADC_REFERENCE_INT1V;
	config_adc_temp.positive_input = ADC_POSITIVE_INPUT_TEMP;
	adc_init(&adc_instance, ADC, &config_adc_temp);
	adc_enable(&adc_instance);
}

void config_port_pin(void)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	port_pin_set_config(PIN_PA10, &config_port_pin);
	port_pin_set_output_level(PIN_PA10, false);
}

void config_all()
{
	system_init();
	
	configure_usart();
	configure_usart_callbacks();

	configure_i2c();
	configure_i2c_callbacks();

	init_adc();

	config_port_pin();

	delay_init();
	
	system_interrupt_enable_global();
}

int main (void)
{

	config_all();

	#define MAX_INPUT_SIZE 90

	char name[MAX_INPUT_SIZE];

	while(true)
	{
		
		//printf("Value of PA10: %d \n", port_pin_get_input_level(PIN_PA10));
		/*
		configure_adc();
		adc_start_conversion(&adc_instance);
		uint16_t result;
		do {
			//Wait for conversion to be done and read out result 
		} while (adc_read(&adc_instance, &result) == STATUS_BUSY);
		printf("ADC: %d\n", result);

		configure_adc_temp();
		adc_start_conversion(&adc_instance);
		uint16_t result_temp;
		do {
			//Wait for conversion to be done and read out result
		} while (adc_read(&adc_instance, &result_temp) == STATUS_BUSY);
		printf("TEMP: %d\n", result_temp);
		*/
		//printf("\n");
		//fgets(name, MAX_INPUT_SIZE, stdin);
		//printf(name);
		i2c_accel(); //I2C Accel not working; wrong type of "read job vs wait"?
		//i2c_scanner();	
	}
}
