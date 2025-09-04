/**
 * @file user_custs1_def.h
 * @brief Custom BLE Service Definition for Ankle Band V2
 * @author Muhammad Umer Sajid, Student
 */

#ifndef USER_CUSTS1_DEF_H_
#define USER_CUSTS1_DEF_H_

#include "attm_db_128.h"

// Service UUID: Custom Ankle Band Service
#define ANKLE_BAND_SERVICE_UUID         {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, \
                                         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}

// Characteristic UUIDs
#define SENSOR_DATA_CHAR_UUID           {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF1, \
                                         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF1}

#define JUMP_METRICS_CHAR_UUID          {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF2, \
                                         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF2}

#define DEVICE_CONTROL_CHAR_UUID        {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF3, \
                                         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF3}

#define BATTERY_STATUS_CHAR_UUID        {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF4, \
                                         0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF4}

// Service and Characteristic Handles
enum {
    // Service
    CUSTS1_IDX_SVC = 0,
    
    // Sensor Data Characteristic
    CUSTS1_IDX_SENSOR_DATA_CHAR,
    CUSTS1_IDX_SENSOR_DATA_VAL,
    CUSTS1_IDX_SENSOR_DATA_NTF_CFG,
    
    // Jump Metrics Characteristic
    CUSTS1_IDX_JUMP_METRICS_CHAR,
    CUSTS1_IDX_JUMP_METRICS_VAL,
    CUSTS1_IDX_JUMP_METRICS_NTF_CFG,
    
    // Device Control Characteristic
    CUSTS1_IDX_DEVICE_CONTROL_CHAR,
    CUSTS1_IDX_DEVICE_CONTROL_VAL,
    
    // Battery Status Characteristic
    CUSTS1_IDX_BATTERY_STATUS_CHAR,
    CUSTS1_IDX_BATTERY_STATUS_VAL,
    CUSTS1_IDX_BATTERY_STATUS_NTF_CFG,
    
    CUSTS1_IDX_NB
};

// Custom Service Database Structure
static const struct attm_desc_128 custs1_att_db[CUSTS1_IDX_NB] = {
    // Ankle Band Service Declaration
    [CUSTS1_IDX_SVC] = {
        (uint8_t*)&att_decl_svc_128,
        PERM(RD, ENABLE),
        0,
        0
    },
    
    // Sensor Data Characteristic Declaration
    [CUSTS1_IDX_SENSOR_DATA_CHAR] = {
        (uint8_t*)&att_decl_char_128,
        PERM(RD, ENABLE),
        0,
        0
    },
    
    // Sensor Data Value
    [CUSTS1_IDX_SENSOR_DATA_VAL] = {
        SENSOR_DATA_CHAR_UUID,
        PERM(RD, ENABLE) | PERM(NTF, ENABLE),
        PERM(RI, ENABLE) | PERM_VAL(20),
        0
    },
    
    // Sensor Data Notification Configuration
    [CUSTS1_IDX_SENSOR_DATA_NTF_CFG] = {
        (uint8_t*)&att_desc_client_char_cfg_128,
        PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(WRITE_REQ, ENABLE),
        0,
        0
    },
    
    // Jump Metrics Characteristic Declaration
    [CUSTS1_IDX_JUMP_METRICS_CHAR] = {
        (uint8_t*)&att_decl_char_128,
        PERM(RD, ENABLE),
        0,
        0
    },
    
    // Jump Metrics Value
    [CUSTS1_IDX_JUMP_METRICS_VAL] = {
        JUMP_METRICS_CHAR_UUID,
        PERM(RD, ENABLE) | PERM(NTF, ENABLE),
        PERM(RI, ENABLE) | PERM_VAL(16),
        0
    },
    
    // Jump Metrics Notification Configuration
    [CUSTS1_IDX_JUMP_METRICS_NTF_CFG] = {
        (uint8_t*)&att_desc_client_char_cfg_128,
        PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(WRITE_REQ, ENABLE),
        0,
        0
    },
    
    // Device Control Characteristic Declaration
    [CUSTS1_IDX_DEVICE_CONTROL_CHAR] = {
        (uint8_t*)&att_decl_char_128,
        PERM(RD, ENABLE),
        0,
        0
    },
    
    // Device Control Value
    [CUSTS1_IDX_DEVICE_CONTROL_VAL] = {
        DEVICE_CONTROL_CHAR_UUID,
        PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(WRITE_REQ, ENABLE),
        PERM(RI, ENABLE) | PERM_VAL(4),
        0
    },
    
    // Battery Status Characteristic Declaration
    [CUSTS1_IDX_BATTERY_STATUS_CHAR] = {
        (uint8_t*)&att_decl_char_128,
        PERM(RD, ENABLE),
        0,
        0
    },
    
    // Battery Status Value
    [CUSTS1_IDX_BATTERY_STATUS_VAL] = {
        BATTERY_STATUS_CHAR_UUID,
        PERM(RD, ENABLE) | PERM(NTF, ENABLE),
        PERM(RI, ENABLE) | PERM_VAL(4),
        0
    },
    
    // Battery Status Notification Configuration
    [CUSTS1_IDX_BATTERY_STATUS_NTF_CFG] = {
        (uint8_t*)&att_desc_client_char_cfg_128,
        PERM(RD, ENABLE) | PERM(WR, ENABLE) | PERM(WRITE_REQ, ENABLE),
        0,
        0
    }
};

// Device Control Commands
#define DEVICE_CMD_CALIBRATE            0x01
#define DEVICE_CMD_RESET_COUNTERS       0x02
#define DEVICE_CMD_SET_MODE_MEDICAL     0x03
#define DEVICE_CMD_SET_MODE_GYMNASTICS  0x04
#define DEVICE_CMD_GET_STATUS           0x05
#define DEVICE_CMD_SLEEP_MODE           0x06

// Data Packet Headers
#define DATA_HEADER_SENSOR              0xAA
#define DATA_HEADER_JUMP_METRICS        0xBB
#define DATA_HEADER_BATTERY             0xCC
#define DATA_HEADER_STATUS              0xDD

// Maximum data lengths
#define MAX_SENSOR_DATA_LEN             20
#define MAX_JUMP_METRICS_LEN            16
#define MAX_CONTROL_DATA_LEN            4
#define MAX_BATTERY_DATA_LEN            4

#endif // USER_CUSTS1_DEF_H_