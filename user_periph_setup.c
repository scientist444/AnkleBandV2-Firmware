/**
 * @file user_periph_setup.c
 * @brief Peripheral setup implementation for Ankle Band V2
 * @author Muhammad Umer Sajid, Student
 */

#include "user_periph_setup.h"
#include "user_config.h"
#include "gpio.h"
#include "uart.h"
#include "syscntl.h"

/**
 * @brief Initialize all peripherals
 */
void periph_init(void) {
    // Set pad functions
    set_pad_functions();
    
    // Configure GPIO reservations
    GPIO_reservations();
    
    // Initialize UART for debug output
#if defined(CFG_PRINTF_UART2)
    uart2_init(UART_BAUDRATE_115200, UART_DATABITS_8, UART_PARITY_NONE, UART_STOPBITS_1, UART_AFCE_DIS, UART_FIFO_EN);
#endif
}

/**
 * @brief Set pad functions for all pins
 */
void set_pad_functions(void) {
    // UART2 Debug pins
    GPIO_ConfigurePin(UART2_TX_PORT, UART2_TX_PIN, OUTPUT, PID_UART2_TX, false);
    GPIO_ConfigurePin(UART2_RX_PORT, UART2_RX_PIN, INPUT, PID_UART2_RX, false);
    
    // I2C pins for BMI270
    GPIO_ConfigurePin(I2C_SDA_PORT, I2C_SDA_PIN, INPUT_PULLUP, PID_I2C_SDA, false);
    GPIO_ConfigurePin(I2C_SCL_PORT, I2C_SCL_PIN, INPUT_PULLUP, PID_I2C_SCL, false);
    
    // LED pin
    GPIO_ConfigurePin(GPIO_LED_PORT, GPIO_LED_PIN, OUTPUT, PID_GPIO, false);
    
    // BMI270 interrupt pin
    GPIO_ConfigurePin(GPIO_BMI270_INT1_PORT, GPIO_BMI270_INT1_PIN, INPUT_PULLDOWN, PID_GPIO, false);
    
    // Pressure sensor ADC pin
    GPIO_ConfigurePin(GPIO_PRESSURE_PORT, GPIO_PRESSURE_PIN, INPUT, PID_ADC, false);
    
    // SWD pins (for debugging)
    GPIO_ConfigurePin(SWD_CLK_PORT, SWD_CLK_PIN, INPUT, PID_SPI_CLK, false);
    GPIO_ConfigurePin(SWD_DATA_PORT, SWD_DATA_PIN, INPUT, PID_SPI_DI, false);
}

/**
 * @brief Reserve GPIO pins to prevent conflicts
 */
void GPIO_reservations(void) {
    // Reserve UART pins
    RESERVE_GPIO(UART2_TX, UART2_TX_PORT, UART2_TX_PIN, PID_UART2_TX);
    RESERVE_GPIO(UART2_RX, UART2_RX_PORT, UART2_RX_PIN, PID_UART2_RX);
    
    // Reserve I2C pins
    RESERVE_GPIO(I2C_SDA, I2C_SDA_PORT, I2C_SDA_PIN, PID_I2C_SDA);
    RESERVE_GPIO(I2C_SCL, I2C_SCL_PORT, I2C_SCL_PIN, PID_I2C_SCL);
    
    // Reserve LED pin
    RESERVE_GPIO(LED, GPIO_LED_PORT, GPIO_LED_PIN, PID_GPIO);
    
    // Reserve BMI270 interrupt pin
    RESERVE_GPIO(BMI270_INT1, GPIO_BMI270_INT1_PORT, GPIO_BMI270_INT1_PIN, PID_GPIO);
    
    // Reserve pressure sensor pin
    RESERVE_GPIO(PRESSURE, GPIO_PRESSURE_PORT, GPIO_PRESSURE_PIN, PID_ADC);
    
    // Reserve SWD pins
    RESERVE_GPIO(SWD_CLK, SWD_CLK_PORT, SWD_CLK_PIN, PID_SPI_CLK);
    RESERVE_GPIO(SWD_DATA, SWD_DATA_PORT, SWD_DATA_PIN, PID_SPI_DI);
}