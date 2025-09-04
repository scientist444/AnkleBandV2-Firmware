/**
 * @file user_custs1_impl.h
 * @brief Custom BLE Service Implementation Header
 * @author Muhammad Umer Sajid, Student
 */

#ifndef USER_CUSTS1_IMPL_H_
#define USER_CUSTS1_IMPL_H_

#include "ke_msg.h"
#include "custs1_task.h"
#include "user_custs1_def.h"

// BLE Connection States
typedef enum {
    BLE_DISCONNECTED = 0,
    BLE_CONNECTED,
    BLE_ADVERTISING
} ble_state_t;

// Function Prototypes
void user_custs1_create_db(void);
void user_custs1_enable_ind_handler(ke_msg_id_t const msgid,
                                    struct custs1_env_tag *custs1_env,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);

void user_custs1_disable_ind_handler(ke_msg_id_t const msgid,
                                     struct custs1_env_tag *custs1_env,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id);

void user_custs1_val_write_ind_handler(ke_msg_id_t const msgid,
                                       struct custs1_val_write_ind const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id);

void user_custs1_val_ntf_cfm_handler(ke_msg_id_t const msgid,
                                     struct custs1_val_ntf_cfm const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id);

// Application Functions
void user_custs1_sensor_data_send(uint8_t *data, uint8_t length);
void user_custs1_jump_metrics_send(uint8_t *data, uint8_t length);
void user_custs1_battery_status_send(uint8_t *data, uint8_t length);
ble_state_t user_ble_get_state(void);
void user_ble_set_state(ble_state_t state);

#endif // USER_CUSTS1_IMPL_H_