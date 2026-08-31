#include "helius/autopilot.h"

#include <stddef.h>

void helius_autopilot_init(helius_autopilot_t *autopilot, const helius_autopilot_config_t *config)
{
    if(autopilot == NULL || config == NULL)
    {
        return;
    }

    helius_roll_controller_init(&autopilot->roll_controller, &config->roll_pid_config);
    helius_pitch_controller_init(&autopilot->pitch_controller, &config->pitch_pid_config);
}

void helius_autopilot_reset(helius_autopilot_t *autopilot)
{
    if(autopilot == NULL)
    {
        return;
    }

    helius_roll_controller_reset(&autopilot->roll_controller);
    helius_pitch_controller_reset(&autopilot->pitch_controller);
}

void helius_autopilot_update(
    helius_autopilot_t *autopilot,
    const helius_state_t *state,
    const helius_setpoint_t *setpoint,
    float dt,
    helius_control_output_t *control_output)
{
    if(autopilot == NULL || state == NULL || setpoint == NULL || control_output == NULL)
    {
        return;
    }

    // Update roll controller
    float aileron_command = helius_roll_controller_update(
        &autopilot->roll_controller,
        setpoint->roll_rad,
        state->roll_rad,
        state->p_rad_s,
        dt);

    // Update pitch controller
    float elevator_command = helius_pitch_controller_update(
        &autopilot->pitch_controller,
        setpoint->pitch_rad,
        state->pitch_rad,
        state->q_rad_s,
        dt);

    // Set control outputs
    control_output->aileron = aileron_command;
    control_output->elevator = elevator_command;
    control_output->rudder = 0.0f;   // Placeholder for rudder control
    control_output->throttle = 0.0f; // Placeholder for throttle control
}
