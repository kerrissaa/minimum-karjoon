#include "minemu/platform.h"
#include "minemu/uart.h"


// AI assistance: This uart_puts implementation was generated with ChatGPT and reviewed by the author.
void uart_putc(char byte)
{
    while (!(MINEMU_UART0->status & MINEMU_UART_STATUS_TX_READY)) {
    }
    MINEMU_UART0->tx_data = (uint32_t)byte;
}


void uart_puts(const char *str)
{
    while (*str != '\0') {
        uart_putc(*str);
        str++;
    }
}