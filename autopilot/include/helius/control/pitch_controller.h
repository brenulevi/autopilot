#ifndef HELIUS_CONTROL_PITCH_CONTROLLER_H
#define HELIUS_CONTROL_PITCH_CONTROLLER_H

#include "helius/control/pid.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        helius_pid_t pid;
    } helius_pitch_controller_t;

    void helius_pitch_controller_init(helius_pitch_controller_t *controller, const helius_pid_config_t *pid_config);
    void helius_pitch_controller_reset(helius_pitch_controller_t *controller);
    float helius_pitch_controller_update(helius_pitch_controller_t *controller, float pitch_target_rad, float pitch_rad, float pitch_rate_rad_s, float dt);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HELIUS_CONTROL_PITCH_CONTROLLER_H