#pragma once

struct SimulationState
{
    double roll_rad;
    double pitch_rad;
    double yaw_rad;

    double p_rad_s;
    double q_rad_s;
    double r_rad_s;

    double altitude_m;
    double airspeed_m_s;
};

struct SimulationActuators
{
    double aileron;
    double elevator;
    double rudder;
    double throttle;
};

class SimulationAdapter
{
public:
    virtual ~SimulationAdapter() = default;

    virtual bool initialize() = 0;

    virtual SimulationState getState() = 0;

    virtual void setActuators(
        const SimulationActuators& actuators
    ) = 0;

    virtual bool step() = 0;

    virtual double getTime() const = 0;
    virtual double getDt() const = 0;
};