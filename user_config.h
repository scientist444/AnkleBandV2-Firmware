/**
 * @file user_config.h
 * @brief User configuration for Ankle Band V2
 */

#ifndef USER_CONFIG_H_
#define USER_CONFIG_H_

// System Configuration
#define CFG_DEVELOPMENT_DEBUG           (1)
#define CFG_PRINTF                      (1)
#define CFG_PRINTF_UART2                (1)

// BLE Configuration
#define CFG_BLE                         (1)
#define CFG_PERIPHERAL                  (1)
#define CFG_CUSTOM_SERVICE              (1)
#define CFG_CUSTS1                      (1)

// Power Management
#define CFG_EXT_SLEEP                   (1)
#define CFG_DEEP_SLEEP                  (0)
#define CFG_WAKEUP_EXT_PROCESSOR        (0)

// Device Information
#define USER_DEVICE_NAME                "AnkleBand"
#define USER_DEVICE_NAME_LEN            (sizeof(USER_DEVICE_NAME) - 1)

// BLE Parameters
#define USER_ADV_INTERVAL_MIN           (160)    // 100ms (160 * 0.625ms)
#define USER_ADV_INTERVAL_MAX           (160)    // 100ms
#define USER_CONNECTION_INTERVAL_MIN    (80)     // 100ms (80 * 1.25ms)
#define USER_CONNECTION_INTERVAL_MAX    (80)     // 100ms
#define USER_SLAVE_LATENCY              (0)
#define USER_SUPERVISION_TIMEOUT        (1000)   // 10s (1000 * 10ms)

// Hardware Configuration
#define GPIO_ALERT_LED_PORT             GPIO_PORT_0
#define GPIO_ALERT_LED_PIN              GPIO_PIN_11

#define I2C_SDA_PORT                    GPIO_PORT_0
#define I2C_SDA_PIN                     GPIO_PIN_8
#define I2C_SCL_PORT                    GPIO_PORT_0
#define I2C_SCL_PIN                     GPIO_PIN_9

#define PRESSURE_SENSOR_PORT            GPIO_PORT_0
#define PRESSURE_SENSOR_PIN             GPIO_PIN_5

// BMI270 Configuration
#define BMI270_I2C_ADDR                 (0x68)
#define BMI270_INT1_PORT                GPIO_PORT_0
#define BMI270_INT1_PIN                 GPIO_PIN_6

// Application Configuration
#define ANKLE_BAND_VERSION              "2.0"
#define MANUFACTURER_NAME               "YourCompany"
#define MODEL_NUMBER                    "AB-V2-001"
#define SERIAL_NUMBER                   "AB2025001"

// Memory Configuration
#define CFG_MAX_CONNECTIONS             (1)
#define CFG_CON_CTE_RSP_ENABLE          (0)

// Debug Configuration
#ifdef CFG_DEVELOPMENT_DEBUG
    #define DBG_TRACE_ENABLE            (1)
    #define DBG_UART_ENABLE             (1)
#else
    #define DBG_TRACE_ENABLE            (0)
    #define DBG_UART_ENABLE             (0)
#endif

// Low Power Configuration
#define LP_CLK_OTP_OFFSET               (0x7f74)
#define USE_POWER_OPTIMIZATIONS         (1)

// Application Specific
#define SENSOR_SAMPLE_RATE_HZ           (100)
#define JUMP_DETECTION_ENABLED          (1)
#define PRESSURE_SENSOR_ENABLED         (1)
#define BATTERY_MONITORING_ENABLED      (1)

#endif // USER_CONFIG_H_