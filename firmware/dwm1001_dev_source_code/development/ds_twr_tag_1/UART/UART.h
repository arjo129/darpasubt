/*!
* @brief Component name:	UART
*
* Defines public interface to UART application level driver.
*
* @file UART.h
*/

// UART circular buffers - Tx and Rx size
#define UART_TX_BUF_SIZE 512
#define UART_RX_BUF_SIZE 32

bool boUART_Init	(void);
bool boUART_getc	(uint8_t *u8ch);
