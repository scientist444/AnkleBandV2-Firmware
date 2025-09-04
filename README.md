# Ankle Band V2 - DA14531 Firmware

Complete firmware project for the ankle band device with BMI270 IMU, pressure sensor, and BLE connectivity.

## Project Structure

```
AnkleBandV2/
├── src/
│   ├── main.c                    # Main application file
│   ├── user_custs1_impl.c        # BLE service implementation
│   └── user_periph_setup.c       # Peripheral setup (create this)
├── inc/
│   ├── user_config.h             # Configuration header
│   ├── user_custs1_def.h         # BLE service definitions
│   ├── user_custs1_impl.h        # BLE service header
│   └── user_periph_setup.h       # Peripheral setup header
└── README.md                     # This file
```

## Hardware Configuration

- **MCU**: DA14531MOD-00F01002
- **IMU**: BMI270 (I2C: P0.8/P0.9)
- **Pressure Sensor**: Analog sensor (ADC: P0.5)
- **LED**: Status LED (GPIO: P0.11)
- **Power**: Coin cell battery (estimated 1.7 years)

## Keil µVision 5 Setup

### 1. Install Required Software
- Download and install **Keil µVision 5** (ARM MDK)
- Download **DA14531 SDK** from Renesas (Dialog Semiconductor)

### 2. Create New Project
1. Open Keil µVision 5
2. Project → New µVision Project
3. Select location: `C:\AnkleBandV2\`
4. Project name: `AnkleBandV2`
5. Device selection: Dialog DA14531

### 3. Add Files to Project
1. Right-click "Source Group 1" → Add Files to Group
2. Add all `.c` files from `src/` folder
3. Add all `.h` files from `inc/` folder

### 4. Configure Project Settings
**Target Options (Alt+F7):**
- **Device**: DA14531
- **Target**: ARM Cortex-M0+
- **Code Generation**: ARM Compiler 5 (or 6)

**C/C++ Options:**
- **Include Paths**: Add SDK paths:
  ```
  ..\..\sdk\platform\include
  ..\..\sdk\platform\core_modules\ble\api
  ..\..\sdk\platform\driver\bmi270
  ..\..\sdk\platform\driver\adc
  ..\..\sdk\platform\driver\gpio
  ..\..\sdk\platform\driver\i2c
  ..\..\inc
  ```

**Define Symbols:**
```
CFG_BLE
CFG_PERIPHERAL
CFG_CUSTOM_SERVICE
CFG_CUSTS1
CFG_PRINTF
__DA14531__
```

**Linker Options:**
- Use SDK linker script: `da14531_scatter_config.sct`

### 5. Build Configuration
1. Build → Rebuild all target files
2. Check for errors in output window
3. Flash → Download to device

## Features

### Core Functionality
- **100Hz IMU Sampling**: Real-time motion tracking
- **Jump Detection**: Physics-based algorithm with height calculation
- **Pressure Sensing**: ADC-based pressure monitoring
- **BLE Connectivity**: Custom service with 4 characteristics
- **Power Management**: Ultra-low power with 1.7-year battery life

### BLE Services
**Service UUID**: `123456789ABCDEF0123456789ABCDEF0`

**Characteristics**:
1. **Sensor Data** (Notify): Real-time IMU + pressure data
2. **Jump Metrics** (Notify): Jump height, count, flight time
3. **Device Control** (Write): Commands for calibration, mode changes
4. **Battery Status** (Notify): Battery voltage and status

### Commands (Device Control)
- `0x01`: Start calibration
- `0x02`: Reset jump counters
- `0x03`: Medical mode
- `0x04`: Gymnastics mode
- `0x05`: Get device status

## Usage

1. **Power On**: Device starts with LED flash sequence
2. **Calibration**: Keep device still for 5 seconds during startup
3. **BLE Connection**: Device advertises as "AnkleBand"
4. **Jump Detection**: Automatic detection with LED confirmation
5. **Data Streaming**: Real-time data via BLE when connected

## Mobile App Integration

**Data Packet Format (Sensor Data)**:
```
[0xAA][AccelX][AccelY][AccelZ][JumpHeight][JumpCount_L][JumpCount_H][Pressure][Battery_H][Battery_L]
```

**Jump Metrics Format**:
```
[0xBB][Height][FlightTime][TotalJumps][MaxHeight][SessionTime]
```

## Troubleshooting

### Common Issues
1. **Build Errors**: Check SDK paths and include directories
2. **Flash Errors**: Ensure SWD connections (P0.2, P0.10)
3. **BMI270 Not Found**: Check I2C connections (P0.8, P0.9)
4. **BLE Not Working**: Verify antenna and crystal oscillator

### Debug Output
- Enable UART2 debug: P0.10 (TX), P0.11 (RX)
- Baud rate: 115200, 8N1
- Use terminal software to monitor debug messages

## Power Optimization

- **Sleep Mode**: Device enters extended sleep between samples
- **BLE Intervals**: Optimized for power (100ms connection interval)
- **Sensor Power**: BMI270 low-power mode when idle
- **Wake Sources**: BMI270 interrupt, BLE events, timer

## Medical vs Gymnastics Mode

- **Medical Mode**: Focus on rehabilitation tracking, slower sampling
- **Gymnastics Mode**: Real-time performance feedback, higher precision
- Switch modes via BLE control command or mobile app

---

**Version**: 2.0  
**Author**: Muhammad Umer Sajid, Student  
**Date**: 2025  
**Target**: DA14531MOD-00F01002