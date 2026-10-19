#include <xc.h>
#include "i2c.h"

void init_i2c(void)
{
    TRISC3 = 1;  // Set SCL pin as input
    TRISC4 = 1;  // Set SDA pin as input

    SSPCON1 = 0x28;  // Configure MSSP in I2C master mode

    SSPADD = 0x31;  // Set I2C clock to 100 kHz

    CKE = 0;  // Enable standard I2C levels

    SMP = 1;  // Disable slew rate for 100 kHz operation

    SSPIF = 0;  // Clear I2C interrupt flag
    BCLIF = 0;  // Clear bus collision flag
}

void i2c_idle(void)
{
    while (!SSPIF);  // Wait until I2C operation completes
    SSPIF = 0;  // Clear I2C interrupt flag
}

void i2c_start(void)
{
    SEN = 1;  // Generate I2C start condition
    i2c_idle();  // Wait for operation to complete
}

void i2c_stop(void)
{
    PEN = 1;  // Generate I2C stop condition
    i2c_idle();  // Wait for operation to complete
}

void i2c_rep_start(void)
{
    RSEN = 1;  // Generate repeated start condition
    i2c_idle();  // Wait for operation to complete
}

void i2c_write(unsigned char data)
{
    SSPBUF = data;  // Load data into I2C buffer
    i2c_idle();  // Wait for transmission to complete
}

void i2c_rx_mode(void)
{
    RCEN = 1;  // Enable I2C receive mode
    i2c_idle();  // Wait for data reception
}

void i2c_no_ack(void)
{
    ACKDT = 1;  // Select NACK response
    ACKEN = 1;  // Send NACK signal
    i2c_idle();  // Wait for acknowledgement sequence to complete
}

unsigned char i2c_read(void)
{
    unsigned char data;

    i2c_rx_mode();  // Receive data from I2C device

    data = SSPBUF;  // Read received data

    i2c_no_ack();  // Send NACK after the final byte

    return data;  // Return received data
}
