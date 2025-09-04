/**
 * @file user_custs1_impl.c
 * @brief Custom BLE Service Implementation
 * @author Muhammad Umer Sajid, Student
 */

#include <string.h>
#include <stdio.h>
#include "user_custs1_impl.h"
#include "user_custs1_def.h"
#include "user_config.h"
#include "prf_utils.h"
#include "custs1.h"
#include "custs1_task.h"
#include "attm_db.h"
#include "gapc_task.h"
#include "arch_console.h"

// Global Variables
static ble_state_t ble_connection_state = BLE_DISCONNECTED;
static uint8_t connection_idx = 0;

/**
 * @brief Create custom service database
 */
void user_custs1_create_db(void) {
    struct custs1_create_db_req *req = KE_MSG_ALLOC(CUSTS1_CREATE_DB_REQ,
                                                    TASK_CUSTS1,
                                                    TASK_APP,
                                                    custs1_create_db_req);

    // Service configuration
    req->cfg_flag = CUSTS1_CFG_FLAG_MANDATORY_MASK;
    req->max_nb_att = CUSTS1_IDX_NB;

    ke_msg_send(req);
}

/**
 * @brief Service enable indication handler
 */
void user_custs1_enable_ind_handler(ke_msg_id_t const msgid,
                                    struct custs1_env_tag *custs1_env,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id) {
    printf("Custom service enabled\n");
}

/**
 * @brief Service disable indication handler
 */
void user_custs1_disable_ind_handler(ke_msg_id_t const msgid,
                                     struct custs1_env_tag *custs1_env,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id) {
    printf("Custom service disabled\n");
}

/**
 * @brief Value write indication handler
 */
void user_custs1_val_write_ind_handler(ke_msg_id_t const msgid,
                                       struct custs1_val_write_ind const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id) {
    switch (param->handle) {
        case CUSTS1_IDX_DEVICE_CONTROL_VAL:
            if (param->length > 0) {
                uint8_t command = param->value[0];
                printf("Control command received: 0x%02X\n", command);
                
                switch (command) {
                    case DEVICE_CMD_CALIBRATE:
                        printf("Calibration requested\n");
                        // Trigger calibration in main loop
                        break;
                        
                    case DEVICE_CMD_RESET_COUNTERS:
                        printf("Reset counters requested\n");
                        // Reset jump counters
                        break;
                        
                    case DEVICE_CMD_SET_MODE_MEDICAL:
                        printf("Medical mode selected\n");
                        break;
                        
                    case DEVICE_CMD_SET_MODE_GYMNASTICS:
                        printf("Gymnastics mode selected\n");
                        break;
                        
                    case DEVICE_CMD_GET_STATUS:
                        printf("Status request\n");
                        // Send current device status
                        break;
                        
                    default:
                        printf("Unknown command\n");
                        break;
                }
            }
            break;
            
        case CUSTS1_IDX_SENSOR_DATA_NTF_CFG:
            if (param->length == 2) {
                uint16_t ntf_cfg = (param->value[1] << 8) | param->value[0];
                if (ntf_cfg == PRF_CLI_START_NTF) {
                    printf("Sensor data notifications enabled\n");
                } else {
                    printf("Sensor data notifications disabled\n");
                }
            }
            break;
            
        case CUSTS1_IDX_JUMP_METRICS_NTF_CFG:
            if (param->length == 2) {
                uint16_t ntf_cfg = (param->value[1] << 8) | param->value[0];
                if (ntf_cfg == PRF_CLI_START_NTF) {
                    printf("Jump metrics notifications enabled\n");
                } else {
                    printf("Jump metrics notifications disabled\n");
                }
            }
            break;
            
        default:
            break;
    }
}

/**
 * @brief Notification confirmation handler
 */
void user_custs1_val_ntf_cfm_handler(ke_msg_id_t const msgid,
                                     struct custs1_val_ntf_cfm const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id) {
    // Notification was successfully sent
}

/**
 * @brief Send sensor data notification
 */
void user_custs1_sensor_data_send(uint8_t *data, uint8_t length) {
    if (ble_connection_state != BLE_CONNECTED || length > MAX_SENSOR_DATA_LEN) {
        return;
    }
    
    struct custs1_val_ntf_ind_req *req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                                           prf_get_task_from_id(TASK_ID_CUSTS1),
                                                           TASK_APP,
                                                           custs1_val_ntf_ind_req,
                                                           length);
    
    req->conidx = connection_idx;
    req->handle = CUSTS1_IDX_SENSOR_DATA_VAL;
    req->length = length;
    req->notification = true;
    
    memcpy(req->value, data, length);
    ke_msg_send(req);
}

/**
 * @brief Send jump metrics notification
 */
void user_custs1_jump_metrics_send(uint8_t *data, uint8_t length) {
    if (ble_connection_state != BLE_CONNECTED || length > MAX_JUMP_METRICS_LEN) {
        return;
    }
    
    struct custs1_val_ntf_ind_req *req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                                           prf_get_task_from_id(TASK_ID_CUSTS1),
                                                           TASK_APP,
                                                           custs1_val_ntf_ind_req,
                                                           length);
    
    req->conidx = connection_idx;
    req->handle = CUSTS1_IDX_JUMP_METRICS_VAL;
    req->length = length;
    req->notification = true;
    
    memcpy(req->value, data, length);
    ke_msg_send(req);
}

/**
 * @brief Send battery status notification
 */
void user_custs1_battery_status_send(uint8_t *data, uint8_t length) {
    if (ble_connection_state != BLE_CONNECTED || length > MAX_BATTERY_DATA_LEN) {
        return;
    }
    
    struct custs1_val_ntf_ind_req *req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                                           prf_get_task_from_id(TASK_ID_CUSTS1),
                                                           TASK_APP,
                                                           custs1_val_ntf_ind_req,
                                                           length);
    
    req->conidx = connection_idx;
    req->handle = CUSTS1_IDX_BATTERY_STATUS_VAL;
    req->length = length;
    req->notification = true;
    
    memcpy(req->value, data, length);
    ke_msg_send(req);
}

/**
 * @brief Get BLE connection state
 */
ble_state_t user_ble_get_state(void) {
    return ble_connection_state;
}

/**
 * @brief Set BLE connection state
 */
void user_ble_set_state(ble_state_t state) {
    ble_connection_state = state;
    
    switch (state) {
        case BLE_DISCONNECTED:
            printf("BLE: Disconnected\n");
            break;
        case BLE_CONNECTED:
            printf("BLE: Connected\n");
            break;
        case BLE_ADVERTISING:
            printf("BLE: Advertising\n");
            break;
    }
}

/**
 * @brief Connection event handler
 */
void user_on_connection(uint8_t conidx, struct gapc_connection_req_ind const *param) {
    connection_idx = conidx;
    user_ble_set_state(BLE_CONNECTED);
}

/**
 * @brief Disconnection event handler
 */
void user_on_disconnect(struct gapc_disconnect_ind const *param) {
    user_ble_set_state(BLE_DISCONNECTED);
    connection_idx = 0;
}