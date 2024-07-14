#ifndef _WIFI_H_
#define _WIFI_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "mxc_delay.h"

#define UART_BAUD 115200
#define SERIAL_BUFFER_SIZE 512

extern char g_serial_buffer[SERIAL_BUFFER_SIZE];

int MXC_UART_WriteBytes(mxc_uart_regs_t *uart, const uint8_t *bytes, int len);
int send_msg(const char *msg);
int recv_msg(char *buffer);
void clear_buffer(void);
void clear_uart(void);
void wifi_setup();

#endif