#ifndef MINEMU_UART_H
#define MINEMU_UART_H

void uart_putc(char byte);
void uart_puts(const char *str);



void uart_init(void);
void uart_rx_irq_handler(void);
int uart_getc_nonblocking(char *out);
#endif