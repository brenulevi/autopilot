#include "helius/control/pid.h"

#include <stddef.h>

static float clampf(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }
}

void helius_pid_init(helius_pid_t* pid, const helius_pid_config_t* config)
{
    if(pid == NULL || config == NULL)
    {
        return;
    }

    pid->config = *config;
    pid->integral = 0.0f;
}

void helius_pid_reset(helius_pid_t* pid)
{
    if(pid == NULL)
    {
        return;
    }

    pid->integral = 0.0f;
}

float helius_pid_update(helius_pid_t* pid, float error, float error_rate, float dt)
{
    if(pid == NULL || dt <= 0.0f)
    {
        return 0.0f;
    }

    // Update integral term
    pid->integral += error * dt;
    pid->integral = clampf(pid->integral, pid->config.integral_min, pid->config.integral_max);

    const float proportional = pid->config.kp * error;
    const float integral = pid->config.ki * pid->integral;
    const float derivative_term = pid->config.kd * error_rate;

    const float output = proportional + integral + derivative_term;

    return clampf(output, pid->config.output_min, pid->config.output_max);
}