
#include <stddef.h>
#include <stdint.h>

#include "minemu/irq.h"
#include "minemu/shell.h"
#include "minemu/uart.h"

#define SHELL_LINE_MAX 20

//string comparse helper implemented with AI
static int string_compare(const char *first, const char *second) {
    while (*first != '\0' && *first == *second) {
        first++;
        second++;
    }

    return (unsigned char)*first - (unsigned char)*second;
}

//parseLine was debugged and tweaked by AI
static void parseLine(char *input) {
    char *command = input;

    while (*command == ' ') {
        command++;
    }

    if (*command == '\0') {
        return;
    }

    char cmd[21];
    int currInd = 0;

    while (*command != ' ' && *command != '\0') {
        if (currInd < 20) {
            cmd[currInd] = *command;
            currInd++;
        }
        command++;
    }

    cmd[currInd] = '\0';

    if (string_compare("echo", cmd) != 0) {
        uart_puts("command not found: ");
        uart_puts(cmd);
        uart_putc('\n');
        return;
    }

    while (*command == ' ') {
        command++;
    }

    uart_puts(command);
    uart_putc('\n');
}

//Ai Assistance: The following shell implementation was developed with the use of ChatGpt.
void shell_run(void) {
    char line[SHELL_LINE_MAX + 1];
    size_t length = 0;

    uart_puts("msh> ");

    for (;;) {
        char byte;

        if (!uart_getc_nonblocking(&byte)) {
            continue;
        }

        if (byte == '\n') {
            uart_putc('\n');

            line[length] = '\0';
            parseLine(line);

            length = 0;
            uart_puts("msh> ");
        } else if (byte == '\b' || (unsigned char)byte == 0x7f) {
            if (length > 0) {
                length--;
            }
        } else if (byte != '\r' && length < SHELL_LINE_MAX) {
            line[length++] = byte;
        }
    }
}