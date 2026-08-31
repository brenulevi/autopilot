#ifndef HELIUS_CONTROL_PID_H
#define HELIUS_CONTROL_PID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    float kp;
    float ki;
    float kd;

    float output_min;
    float output_max;

    float integral_min;
    float integral_max;
} helius_pid_config_t;

typedef struct
{
    helius_pid_config_t config;

    float integral;
} helius_pid_t;

void helius_pid_init(helius_pid_t* pid, const helius_pid_config_t* config);
void helius_pid_reset(helius_pid_t* pid);
float helius_pid_update(helius_pid_t* pid, float error, float error_rate, float dt);

#ifdef __cplusplus
}
#endif

#endif // HELIUS_CONTROL_PID_H