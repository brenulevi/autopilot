#include <iostream>

#include "SimulationAdapter.h"
#include "JSBSimAdapter.h"

#include "helius/autopilot.h"

namespace
{
    constexpr double RAD_TO_DEG =
        180.0 / std::numbers::pi;
}

int main()
{
    JSBSimAdapter simulator;

    SimulationScenario scenario{
        .aircraft = "c172x",

        .dt_s = 0.01, // 100 Hz

        .altitude_m = 914.4,  // 3000 ft
        .airspeed_m_s = 46.3, // ~90 kt

        .roll_rad = 0.0,
        .pitch_rad = 0.0,
        .heading_rad = 0.0,

        .flight_path_angle_rad = 0.0,

        .trim = true};

    if (!simulator.initialize(scenario))
    {
        std::cerr << "Failed to initialize simulation\n";
        return 1;
    }

    auto trim = simulator.getTrim();

    helius_autopilot_t autopilot{};

    helius_autopilot_config_t autopilot_config{
        .roll_pid_config = {
            .kp = 0.5f,
            .ki = 0.0f,
            .kd = 0.3f,

            .output_min = -0.3f,
            .output_max = 0.3f,

            .integral_min = -0.2f,
            .integral_max = 0.2f},
        .pitch_pid_config = {
            .kp = 0.5f,
            .ki = 0.0f,
            .kd = 0.3f,

            .output_min = -0.3f,
            .output_max = 0.3f,

            .integral_min = -0.2f,
            .integral_max = 0.2f}};

    helius_autopilot_init(&autopilot, &autopilot_config);

    while (simulator.getTime() < 10.0)
    {
        SimulationState sim_state = simulator.getState();

        helius_state_t state{
            .roll_rad = static_cast<float>(sim_state.roll_rad),
            .pitch_rad = static_cast<float>(sim_state.pitch_rad),
            .yaw_rad = static_cast<float>(sim_state.yaw_rad),

            .p_rad_s = static_cast<float>(sim_state.p_rad_s),
            .q_rad_s = static_cast<float>(sim_state.q_rad_s),
            .r_rad_s = static_cast<float>(sim_state.r_rad_s)
        };

        helius_setpoint_t setpoint{
            .roll_rad = 0.0f,
            .pitch_rad = 3.4f
        };

        helius_control_output_t control_output{};

        helius_autopilot_update(
            &autopilot,
            &state,
            &setpoint,
            static_cast<float>(scenario.dt_s),
            &control_output
        );

        SimulationActuators actuators = trim;
        actuators.aileron += control_output.aileron;

        double disturbance = 0.0;

        if(simulator.getTime() >= 5.0 && simulator.getTime() < 5.5)
        {
            disturbance = 0.1; // Apply a disturbance to the aileron
        }

        actuators.aileron += disturbance;

        simulator.setActuators(actuators);

        std::cout
            << simulator.getTime() << ","
            << state.roll_rad * RAD_TO_DEG << ","
            << state.p_rad_s * RAD_TO_DEG << ","
            << control_output.aileron << ","
            << disturbance << ","
            << actuators.aileron
            << '\n';

        if (!simulator.step())
            break;
    }

    return 0;
}