#include "JSBSimAdapter.h"

#include "FGFDMExec.h"

JSBSimAdapter::JSBSimAdapter()
    : fdm_(std::make_unique<JSBSim::FGFDMExec>())
{
    fdm_->Setdt(dt_);
}

JSBSimAdapter::~JSBSimAdapter() = default;

bool JSBSimAdapter::initialize()
{
    fdm_->SetDebugLevel(0);

    fdm_->SetRootDir(SGPath(HELIUS_JSBSIM_ROOT_DIR));

    fdm_->SetAircraftPath(SGPath(HELIUS_JSBSIM_ROOT_DIR "/aircraft"));
    fdm_->SetEnginePath(SGPath(HELIUS_JSBSIM_ROOT_DIR "/engine"));
    fdm_->SetSystemsPath(SGPath(HELIUS_JSBSIM_ROOT_DIR "/systems"));

    if (!fdm_->LoadModel("c172x"))
        return false;

    return fdm_->RunIC();
}

SimulationState JSBSimAdapter::getState()
{
    SimulationState state{};

    state.roll_rad =
        fdm_->GetPropertyValue("attitude/phi-rad");

    state.pitch_rad =
        fdm_->GetPropertyValue("attitude/theta-rad");

    state.yaw_rad =
        fdm_->GetPropertyValue("attitude/psi-rad");

    state.p_rad_s =
        fdm_->GetPropertyValue("velocities/p-rad_sec");

    state.q_rad_s =
        fdm_->GetPropertyValue("velocities/q-rad_sec");

    state.r_rad_s =
        fdm_->GetPropertyValue("velocities/r-rad_sec");

    return state;
}

void JSBSimAdapter::setActuators(
    const SimulationActuators& actuators)
{
    fdm_->SetPropertyValue(
        "fcs/aileron-cmd-norm",
        actuators.aileron
    );

    fdm_->SetPropertyValue(
        "fcs/elevator-cmd-norm",
        actuators.elevator
    );

    fdm_->SetPropertyValue(
        "fcs/rudder-cmd-norm",
        actuators.rudder
    );

    fdm_->SetPropertyValue(
        "fcs/throttle-cmd-norm[0]",
        actuators.throttle
    );
}

bool JSBSimAdapter::step()
{
    return fdm_->Run();
}

double JSBSimAdapter::getTime() const
{
    return fdm_->GetSimTime();
}

double JSBSimAdapter::getDt() const
{
    return dt_;
}