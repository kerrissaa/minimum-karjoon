#include <stdint.h>

#include "minemu/platform.h"
#include "minemu/uart.h"
#include "minemu/irq.h"

static volatile char rx_buffer[MINEMU_UART_RX_CAPACITY];
static volatile uint32_t rx_head = 0;
static volatile uint32_t rx_tail = 0;


// AI assistance: This uart implementation was generated with ChatGPT and reviewed by the author.
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

void uart_init(void) {
    MINEMU_UART0->control = MINEMU_UART_CONTROL_RX_IRQ_ENABLE;
        /* Enable UART0 in the interrupt controller. */
    MINEMU_INTERRUPT->enable |= (1u << MINEMU_IRQ_UART0);
}

void uart_rx_irq_handler(void) {
    while (MINEMU_UART0->status & MINEMU_UART_STATUS_RX_READY) {
        char byte = (char)MINEMU_UART0->rx_data;
        uint32_t next = (rx_head + 1u) % MINEMU_UART_RX_CAPACITY;

        /* If the buffer is full, drop this character. */
        if (next != rx_tail) {
            rx_buffer[rx_head] = byte;
            rx_head = next;
        }
    }
}

int uart_getc_nonblocking(char *out) {
    int avail = 0;

    /* Protect the shared buffer while accessing it. */
    minemu_irq_disable();

    if (rx_tail != rx_head) {
        *out = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1u) % MINEMU_UART_RX_CAPACITY;
        avail = 1;
    }

    minemu_irq_enable();

    return avail;
}
