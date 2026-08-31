#ifndef HELIUS_CONTROL_ROLL_CONTROLLER_H
#define HELIUS_CONTROL_ROLL_CONTROLLER_H

#include "helius/control/pid.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        helius_pid_t pid;
    } helius_roll_controller_t;

    void helius_roll_controller_init(helius_roll_controller_t *controller, const helius_pid_config_t *pid_config);
    void helius_roll_controller_reset(helius_roll_controller_t *controller);
    float helius_roll_controller_update(helius_roll_controller_t *controller, float roll_target_rad, float roll_rad, float roll_rate_rad_s, float dt);

#ifdef __cplusplus
}
#endif

#endif // HELIUS_CONTROL_ROLL_CONTROLLER_H