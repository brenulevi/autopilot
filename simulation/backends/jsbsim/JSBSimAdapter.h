#pragma once

#include <memory>

#include "SimulationAdapter.h"

namespace JSBSim
{
    class FGFDMExec;
}

class JSBSimAdapter final : public SimulationAdapter
{
public:
    JSBSimAdapter();
    ~JSBSimAdapter() override;

    bool initialize(const SimulationScenario& scenario) override;

    SimulationState getState() override;

    void setActuators(
        const SimulationActuators& actuators
    ) override;

    bool step() override;

    double getTime() const override;
    SimulationActuators getTrim() const override;

private:
    std::unique_ptr<JSBSim::FGFDMExec> fdm_;
};