#include "helius/control/roll_controller.h"

#include <stddef.h>

void helius_roll_controller_init(helius_roll_controller_t* controller, const helius_pid_config_t* pid_config)
{
    if(controller == NULL || pid_config == NULL)
    {
        return;
    }

    helius_pid_init(&controller->pid, pid_config);
}

void helius_roll_controller_reset(helius_roll_controller_t* controller)
{
    if(controller == NULL)
    {
        return;
    }

    helius_pid_reset(&controller->pid);
}

float helius_roll_controller_update(helius_roll_controller_t* controller, float roll_target_rad, float roll_rad, float roll_rate_rad_s, float dt)
{
    if(controller == NULL)
    {
        return 0.0f;
    }

    const float roll_error = roll_target_rad - roll_rad;

    return helius_pid_update(&controller->pid, roll_error, -roll_rate_rad_s, dt);
}