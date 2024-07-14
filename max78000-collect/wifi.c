#include "wifi.h"

char g_serial_buffer[SERIAL_BUFFER_SIZE];
static mxc_uart_regs_t *COMM_UART = MXC_UART_GET_UART(2);
static int g_buffer_index = 0;

int MXC_UART_WriteBytes(mxc_uart_regs_t *uart, const uint8_t *bytes, int len)
{
    int err = E_NO_ERROR;
    for (int i = 0; i < len; i++) {
        // Wait until FIFO has space for the character.
        while (MXC_UART_GetTXFIFOAvailable(uart) < 1) {}

        if ((err = MXC_UART_WriteCharacterRaw(uart, bytes[i])) != E_NO_ERROR) {
            return err;
        }
    }

    return E_NO_ERROR;
}

// Transmit a message over the console's UART with a newline appended.
int send_msg(const char *msg)
{
    int ret = 0;
    int len = strlen(msg);

    // Transmit message string
    if ((ret = MXC_UART_WriteBytes(COMM_UART, (uint8_t *)msg, len)) != E_NO_ERROR) {
        return ret;
    }
    // Transmit newline to complete the message.
    if ((ret = MXC_UART_WriteBytes(COMM_UART, (uint8_t *)"\n", 1)) != E_NO_ERROR) {
        return ret;
    }

    return E_NO_ERROR;
}

// Recieve a message into the global serial buffer.  Returns 1 if a full message
// has been received, otherwise returns 0.
int recv_msg(char *buffer)
{
    int available = MXC_UART_GetRXFIFOAvailable(COMM_UART);
    while (available > 0) {
        char c = MXC_UART_ReadCharacter(COMM_UART);

        if (c == '\n') {
            // Received newline character, terminate the string and return
            g_serial_buffer[g_buffer_index] = '\0';
            return 1;
        } else {
            // Received a character, add to buffer and continue
            g_serial_buffer[g_buffer_index] = c;
        }

        // Increment buffer index (wrap if necessary)
        if (++g_buffer_index >= SERIAL_BUFFER_SIZE) {
            g_buffer_index = 0;
        }

        available = MXC_UART_GetRXFIFOAvailable(COMM_UART);
    }

    // If we reach here, we've received all available characters but
    // no newline.  Return 0 - a full command hasn't been received
    // yet.
    return 0;
}

void clear_buffer(void)
{
    memset(g_serial_buffer, '\0', SERIAL_BUFFER_SIZE);
    g_buffer_index = 0;
}

void clear_uart(void)
{
    MXC_UART_ClearRXFIFO(COMM_UART);
}

void wifi_setup() 
{
    int ret = E_NO_ERROR;
    ret = MXC_UART_Init(COMM_UART, UART_BAUD, MXC_UART_IBRO_CLK);

    if (ret!= E_NO_ERROR) {
        printf("UART init failed\n");
    }

    printf("UART init successful\n");
    
    MXC_Delay(MXC_DELAY_MSEC(3000));
    
    clear_uart();

    ret = send_msg("AT+RESTART\r\n");
    while (true) {
        if (recv_msg(g_serial_buffer)) {
            // printf("Received: %s", g_serial_buffer);
            if (strncmp(g_serial_buffer, "OK", strlen("OK")) == 0) {
                clear_buffer();
                break;
            } else if (strncmp(g_serial_buffer, "ERROR", strlen("ERROR")) == 0) {
                clear_buffer();
                ret = send_msg("AT+RESTART\r\n");
                continue;
            }
            clear_buffer();
        }
    }
    printf("WiFi init successful\n");

    ret = send_msg("AT+WFJAP=czl,4,2,12345678\r\n");
    while (true) {
        if (recv_msg(g_serial_buffer)) {
            // printf("Received: %s\n", g_serial_buffer);
            if (strncmp(g_serial_buffer, "+WFJAP:1", strlen("+WFJAP:1")) == 0) {
                clear_buffer();
                break;
            }
            clear_buffer();
        }
    }
    printf("WiFi connected\n");

    ret = send_msg("AT+NWMQCL=1\r\n");
    while (true) {
        if (recv_msg(g_serial_buffer)) {
            // printf("Received: %s\n", g_serial_buffer);
            if (strncmp(g_serial_buffer, "+NWMQCL:1", strlen("+NWMQCL:1")) == 0) {
                clear_buffer();
                break;
            }
            clear_buffer();
        }
    }
    printf("MQTT Client Enabled\n");

    // ret = send_msg("AT+NWMQBR=broker.emqx.io,1883\r\n");
    ret = send_msg("AT+NWMQBR=192.168.137.1,1883\r\n");
    while (true) {
        if (recv_msg(g_serial_buffer)) {
            // printf("Received: %s\n", g_serial_buffer);
            if (strncmp(g_serial_buffer, "OK", strlen("OK")) == 0) {
                clear_buffer();
                break;
            }
            clear_buffer();
        }
    }
    printf("Connected to MQTT Broker\n");
}