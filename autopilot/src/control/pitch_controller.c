#include "helius/control/pitch_controller.h"

#include <stddef.h>

void helius_pitch_controller_init(helius_pitch_controller_t* controller, const helius_pid_config_t* pid_config)
{
    if(controller == NULL || pid_config == NULL)
    {
        return;
    }

    helius_pid_init(&controller->pid, pid_config);
}

void helius_pitch_controller_reset(helius_pitch_controller_t* controller)
{
    if(controller == NULL)
    {
        return;
    }

    helius_pid_reset(&controller->pid);
}

float helius_pitch_controller_update(helius_pitch_controller_t* controller, float pitch_target_rad, float pitch_rad, float pitch_rate_rad_s, float dt)
{
    if(controller == NULL)
    {
        return 0.0f;
    }

    const float pitch_error = pitch_target_rad - pitch_rad;

    return helius_pid_update(&controller->pid, pitch_error, -pitch_rate_rad_s, dt);
}