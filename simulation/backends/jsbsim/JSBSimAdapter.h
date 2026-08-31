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

    bool initialize() override;

    SimulationState getState() override;

    void setActuators(
        const SimulationActuators& actuators
    ) override;

    bool step() override;

    double getTime() const override;
    double getDt() const override;

private:
    std::unique_ptr<JSBSim::FGFDMExec> fdm_;
    double dt_ = 0.01;
};