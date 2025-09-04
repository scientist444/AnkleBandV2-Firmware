/**
 * @file user_periph_setup.h
 * @brief Peripheral setup configuration for Ankle Band V2
 */

#ifndef USER_PERIPH_SETUP_H_
#define USER_PERIPH_SETUP_H_

#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "spi.h"
#include "arch_system.h"

// UART Configuration for Debug
#define UART2_TX_PORT           GPIO_PORT_0
#define UART2_TX_PIN            GPIO_PIN_10
#define UART2_RX_PORT           GPIO_PORT_0
#define UART2_RX_PIN            GPIO_PIN_11

// I2C Configuration for BMI270
#define I2C_SDA_PORT            GPIO_PORT_0
#define I2C_SDA_PIN             GPIO_PIN_8
#define I2C_SCL_PORT            GPIO_PORT_0
#define I2C_SCL_PIN             GPIO_PIN_9

// GPIO Configuration
#define GPIO_LED_PORT           GPIO_PORT_0
#define GPIO_LED_PIN            GPIO_PIN_11

#define GPIO_BMI270_INT1_PORT   GPIO_PORT_0
#define GPIO_BMI270_INT1_PIN    GPIO_PIN_6

#define GPIO_PRESSURE_PORT      GPIO_PORT_0
#define GPIO_PRESSURE_PIN       GPIO_PIN_5

// SWD Configuration
#define SWD_CLK_PORT            GPIO_PORT_0
#define SWD_CLK_PIN             GPIO_PIN_2
#define SWD_DATA_PORT           GPIO_PORT_0
#define SWD_DATA_PIN            GPIO_PIN_10

// Function Prototypes
void periph_init(void);
void set_pad_functions(void);
void GPIO_reservations(void);

#endif // USER_PERIPH_SETUP_H_