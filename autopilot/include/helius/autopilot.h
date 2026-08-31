#ifndef HELIUS_AUTOPILOT_H
#define HELIUS_AUTOPILOT_H

#include "helius/control/roll_controller.h"
#include "helius/control/pitch_controller.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        float roll_rad;
        float pitch_rad;
        float yaw_rad;

        float p_rad_s;
        float q_rad_s;
        float r_rad_s;

        float altitude_m;
        float airspeed_m_s;
    } helius_state_t;

    typedef struct
    {
        float roll_rad;
        float pitch_rad;
    } helius_setpoint_t;

    typedef struct
    {
        float aileron;
        float elevator;
        float rudder;
        float throttle;
    } helius_control_output_t;

    typedef struct
    {
        helius_pid_config_t roll_pid_config;
        helius_pid_config_t pitch_pid_config;
    } helius_autopilot_config_t;

    typedef struct
    {
        helius_roll_controller_t roll_controller;
        helius_pitch_controller_t pitch_controller;
    } helius_autopilot_t;

    void helius_autopilot_init(helius_autopilot_t *autopilot, const helius_autopilot_config_t *config);
    void helius_autopilot_reset(helius_autopilot_t *autopilot);
    void helius_autopilot_update(
        helius_autopilot_t *autopilot,
        const helius_state_t *state,
        const helius_setpoint_t *setpoint,
        float dt,
        helius_control_output_t *control_output);

#ifdef __cplusplus
}
#endif

#endif // HELIUS_AUTOPILOT_H