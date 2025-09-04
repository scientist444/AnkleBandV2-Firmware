/**
 * @file main.c
 * @brief Ankle Band V2 - Main Application
 * @author Muhammad Umer Sajid, Student
 * @date 2025
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "rwip_config.h"
#include "arch_api.h"
#include "arch_main.h"
#include "arch_console.h"
#include "arch_system.h"
#include "user_periph_setup.h"
#include "user_config.h"
#include "user_custs1_def.h"
#include "user_custs1_impl.h"
#include "gpio.h"
#include "i2c.h"
#include "adc.h"
#include "timer0.h"

// Configuration
#define SAMPLE_RATE_HZ          100
#define JUMP_THRESHOLD_G        1.5
#define LANDING_THRESHOLD_G     2.5
#define CALIBRATION_SAMPLES     500
#define LED_PIN                 GPIO_PIN_11
#define PRESSURE_ADC_CHANNEL    ADC_CHANNEL_P0_5

// Data Structures
typedef struct {
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
    uint16_t pressure;
    uint32_t timestamp;
} sensor_data_t;

typedef struct {
    float accel_offset[3];
    float gyro_offset[3];
    bool calibrated;
} cal_data_t;

typedef struct {
    uint32_t total_jumps;
    float jump_height;
    float flight_time;
    bool in_jump;
    uint32_t jump_start;
    uint16_t battery_mv;
} device_state_t;

// Global Variables
static sensor_data_t sensor_data;
static cal_data_t calibration = {0};
static device_state_t device = {0};
static volatile uint32_t system_ticks = 0;

// Function Prototypes
static void system_init(void);
static void calibrate_sensors(void);
static void read_sensors(void);
static void detect_jump(void);
static void ble_transmit(void);
static uint32_t get_time_ms(void);
static void delay_ms(uint32_t ms);
static void led_flash(uint8_t count);

// Timer interrupt for system tick
void timer0_handler(void) {
    system_ticks++;
}

/**
 * @brief Main application entry point
 */
int main(void) {
    system_init_func(NULL);
    
    // Initialize application
    system_init();
    
    // Perform calibration
    calibrate_sensors();
    
    printf("Ankle Band V2 Ready - Jumps: %d\n", (int)device.total_jumps);
    led_flash(3); // Ready indication
    
    // Main application loop
    while (1) {
        read_sensors();
        detect_jump();
        
        if (user_ble_get_state() == BLE_CONNECTED) {
            ble_transmit();
        }
        
        // Low power delay
        delay_ms(10); // 100Hz sampling
    }
}

/**
 * @brief Initialize system peripherals
 */
static void system_init(void) {
    // GPIO initialization
    GPIO_ConfigurePin(GPIO_PORT_0, LED_PIN, OUTPUT, PID_GPIO, false);
    
    // I2C initialization for BMI270
    i2c_env_t i2c_cfg = {
        .clock_cfg.ss_hcnt = I2C_SS_SCL_HCNT_REG_RESET,
        .clock_cfg.ss_lcnt = I2C_SS_SCL_LCNT_REG_RESET,
        .clock_cfg.fs_hcnt = I2C_FS_SCL_HCNT_REG_RESET,
        .clock_cfg.fs_lcnt = I2C_FS_SCL_LCNT_REG_RESET
    };
    i2c_init(&i2c_cfg);
    
    // Timer initialization for system tick
    timer0_init(TIM0_CLK_32K, PWM_MODE_ONE, TIM0_CLK_DIV_32);
    timer0_set_pwm_on_counter(1); // 1ms tick
    timer0_register_callback(timer0_handler);
    timer0_start();
    
    // Initialize I2C for BMI270 (simplified for DA14531)
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_8, INPUT_PULLUP, PID_I2C_SCL, false);
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_9, INPUT_PULLUP, PID_I2C_SDA, false);
    
    printf("I2C initialized for BMI270\n");
}

/**
 * @brief Calibrate IMU sensors
 */
static void calibrate_sensors(void) {
    printf("Calibrating sensors... keep device still\n");
    
    // Simplified calibration for demonstration
    calibration.accel_offset[0] = 0.0f;
    calibration.accel_offset[1] = 0.0f;
    calibration.accel_offset[2] = 0.0f;
    
    calibration.gyro_offset[0] = 0.0f;
    calibration.gyro_offset[1] = 0.0f;
    calibration.gyro_offset[2] = 0.0f;
    
    calibration.calibrated = true;
    printf("Calibration complete\n");
}

/**
 * @brief Read all sensor data
 */
static void read_sensors(void) {
    // Simplified sensor reading for demonstration
    if (calibration.calibrated) {
        // Generate sample data for testing
        sensor_data.accel_x = 0.0f + (rand() % 100 - 50) / 1000.0f;
        sensor_data.accel_y = 0.0f + (rand() % 100 - 50) / 1000.0f;
        sensor_data.accel_z = 1.0f + (rand() % 100 - 50) / 1000.0f;
        
        sensor_data.gyro_x = 0.0f + (rand() % 100 - 50) / 10.0f;
        sensor_data.gyro_y = 0.0f + (rand() % 100 - 50) / 10.0f;
        sensor_data.gyro_z = 0.0f + (rand() % 100 - 50) / 10.0f;
    }
    
    // Read pressure sensor via ADC
    adc_config_t adc_cfg = {
        .input_mode = ADC_INPUT_MODE_SINGLE_ENDED,
        .input = PRESSURE_ADC_CHANNEL,
        .continuous = false,
        .interval_mult = 0,
        .input_attenuator = ADC_INPUT_ATTN_NO,
        .chopping = false,
        .oversampling = 0
    };
    
    adc_init(&adc_cfg);
    adc_enable_channel(PRESSURE_ADC_CHANNEL);
    adc_start();
    
    while (!adc_get_sample_status()) {
        // Wait for conversion
    }
    
    uint16_t adc_raw = adc_get_sample();
    adc_disable();
    
    // Convert to pressure (0-1023 ADC → 0-100 kPa)
    sensor_data.pressure = (adc_raw * 100) / 1023;
    sensor_data.timestamp = get_time_ms();
    
    // Battery monitoring (every 10 seconds)
    static uint32_t last_battery_check = 0;
    if ((get_time_ms() - last_battery_check) > 10000) {
        adc_cfg.input = ADC_CHANNEL_VBAT3V;
        adc_init(&adc_cfg);
        adc_enable_channel(ADC_CHANNEL_VBAT3V);
        adc_start();
        
        while (!adc_get_sample_status());
        
        // Convert to millivolts (10-bit ADC, 3.6V max range)
        device.battery_mv = (adc_get_sample() * 3600) / 1023;
        adc_disable();
        
        last_battery_check = get_time_ms();
        
        if (device.battery_mv < 3100) {
            printf("Low battery: %dmV\n", device.battery_mv);
        }
    }
}

/**
 * @brief Detect jump events
 */
static void detect_jump(void) {
    // Calculate total acceleration magnitude
    float accel_magnitude = sqrtf(sensor_data.accel_x * sensor_data.accel_x +
                                 sensor_data.accel_y * sensor_data.accel_y +
                                 sensor_data.accel_z * sensor_data.accel_z);
    
    static float prev_accel = 1.0f;
    
    // Jump takeoff detection
    if (!device.in_jump && accel_magnitude > JUMP_THRESHOLD_G && prev_accel < 1.2f) {
        device.in_jump = true;
        device.jump_start = get_time_ms();
        printf("Takeoff detected: %.2fg\n", accel_magnitude);
    }
    
    // Landing detection
    if (device.in_jump && accel_magnitude > LANDING_THRESHOLD_G &&
        (get_time_ms() - device.jump_start) > 200) { // Min 200ms flight
        
        device.in_jump = false;
        device.flight_time = (get_time_ms() - device.jump_start) / 1000.0f;
        
        // Calculate jump height using physics: h = (1/2) * g * (t/2)²
        float half_flight = device.flight_time / 2.0f;
        device.jump_height = 0.5f * 9.81f * half_flight * half_flight * 100.0f; // cm
        
        // Validate jump (filter false positives)
        if (device.jump_height >= 5.0f && device.jump_height <= 300.0f) {
            device.total_jumps++;
            
            printf("Jump #%d: %.1fcm (%.3fs)\n",
                   (int)device.total_jumps, device.jump_height, device.flight_time);
            
            led_flash(1); // Jump confirmation
        }
    }
    
    prev_accel = accel_magnitude;
}

/**
 * @brief Transmit data via BLE
 */
static void ble_transmit(void) {
    static uint32_t last_transmission = 0;
    
    // Transmit at 10Hz when connected
    if ((get_time_ms() - last_transmission) < 100) {
        return;
    }
    
    // Prepare data packet
    uint8_t data[16];
    uint8_t idx = 0;
    
    data[idx++] = 0xAA; // Header byte
    
    // Accelerometer data (scaled to fit in bytes)
    data[idx++] = (uint8_t)((sensor_data.accel_x * 50.0f) + 128);
    data[idx++] = (uint8_t)((sensor_data.accel_y * 50.0f) + 128);
    data[idx++] = (uint8_t)((sensor_data.accel_z * 50.0f) + 128);
    
    // Jump metrics
    data[idx++] = (uint8_t)(device.jump_height);
    data[idx++] = (uint8_t)(device.total_jumps & 0xFF);
    data[idx++] = (uint8_t)((device.total_jumps >> 8) & 0xFF);
    
    // Pressure and battery
    data[idx++] = (uint8_t)(sensor_data.pressure);
    data[idx++] = (uint8_t)(device.battery_mv >> 8);
    data[idx++] = (uint8_t)(device.battery_mv & 0xFF);
    
    // Send notification
    user_custs1_sensor_data_send(data, idx);
    
    last_transmission = get_time_ms();
}

/**
 * @brief Get system time in milliseconds
 */
static uint32_t get_time_ms(void) {
    return system_ticks;
}

/**
 * @brief Efficient delay function
 */
static void delay_ms(uint32_t ms) {
    if (ms == 0) return;
    
    uint32_t start_time = get_time_ms();
    while ((get_time_ms() - start_time) < ms) {
        // Enter low power mode until interrupt
        __WFE();
    }
}

/**
 * @brief Flash LED for visual feedback
 */
static void led_flash(uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        GPIO_SetActive(GPIO_PORT_0, LED_PIN);
        delay_ms(100);
        GPIO_SetInactive(GPIO_PORT_0, LED_PIN);
        if (i < (count - 1)) {
            delay_ms(100);
        }
    }
}