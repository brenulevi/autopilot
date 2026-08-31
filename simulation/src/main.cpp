#include <iostream>

#include "SimulationAdapter.h"
#include "JSBSimAdapter.h"

#include "helius/autopilot.h"

int main()
{
    JSBSimAdapter simulator;

    if (!simulator.initialize())
    {
        std::cerr << "Failed to initialize simulation\n";
        return 1;
    }

    while (simulator.getTime() < 10.0)
    {
        SimulationState state = simulator.getState();

        SimulationActuators actuators{};

        actuators.throttle = 0.5;

        simulator.setActuators(actuators);

        if (!simulator.step())
            break;
    }

    return 0;
}