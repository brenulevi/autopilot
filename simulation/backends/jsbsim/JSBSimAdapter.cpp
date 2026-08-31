#include "JSBSimAdapter.h"

#include "FGFDMExec.h"

namespace
{
    constexpr double M_TO_FT = 3.280839895;
    constexpr double MPS_TO_KTS = 1.943844492;
    constexpr double RAD_TO_DEG =
        180.0 / std::numbers::pi;
}

JSBSimAdapter::JSBSimAdapter()
    : fdm_(std::make_unique<JSBSim::FGFDMExec>())
{
}

JSBSimAdapter::~JSBSimAdapter() = default;

bool JSBSimAdapter::initialize(const SimulationScenario &scenario)
{
    fdm_->Setdt(scenario.dt_s);

    fdm_->SetDebugLevel(0);

    fdm_->SetRootDir(SGPath(HELIUS_JSBSIM_ROOT_DIR));

    fdm_->SetAircraftPath(SGPath(HELIUS_JSBSIM_ROOT_DIR "/aircraft"));
    fdm_->SetEnginePath(SGPath(HELIUS_JSBSIM_ROOT_DIR "/engine"));
    fdm_->SetSystemsPath(SGPath(HELIUS_JSBSIM_ROOT_DIR "/systems"));

    if (!fdm_->LoadModel(scenario.aircraft))
        return false;

    // Disable default ap
    fdm_->SetPropertyValue("ap/attitude_hold", 0.0);
    fdm_->SetPropertyValue("ap/altitude_hold", 0.0);
    fdm_->SetPropertyValue("ap/heading_hold", 0.0);
    fdm_->SetPropertyValue("ap/airspeed_hold", 0.0);

    /*
     * Engine running.
     */
    fdm_->SetPropertyValue(
        "propulsion/set-running",
        -1.0);

    /*
     * Initial conditions.
     */
    fdm_->SetPropertyValue(
        "ic/h-sl-ft",
        scenario.altitude_m * M_TO_FT);

    fdm_->SetPropertyValue(
        "ic/vc-kts",
        scenario.airspeed_m_s * MPS_TO_KTS);

    fdm_->SetPropertyValue(
        "ic/phi-deg",
        scenario.roll_rad * RAD_TO_DEG);

    fdm_->SetPropertyValue(
        "ic/theta-deg",
        scenario.pitch_rad * RAD_TO_DEG);

    fdm_->SetPropertyValue(
        "ic/psi-true-deg",
        scenario.heading_rad * RAD_TO_DEG);

    fdm_->SetPropertyValue(
        "ic/gamma-deg",
        scenario.flight_path_angle_rad * RAD_TO_DEG);

    if (!fdm_->RunIC())
        return false;

    if (scenario.trim)
    {
        fdm_->SetPropertyValue(
            "simulation/do_simple_trim",
            1.0);

        trim_.aileron = fdm_->GetPropertyValue(
            "fcs/aileron-cmd-norm");
        trim_.elevator = fdm_->GetPropertyValue(
            "fcs/elevator-cmd-norm");
        trim_.rudder = fdm_->GetPropertyValue(
            "fcs/rudder-cmd-norm");
        trim_.throttle = fdm_->GetPropertyValue(
            "fcs/throttle-cmd-norm[0]");

        std::cout
            << "Trim:\n"
            << "  aileron: "
            << fdm_->GetPropertyValue(
                   "fcs/aileron-cmd-norm")
            << '\n'

            << "  elevator: "
            << fdm_->GetPropertyValue(
                   "fcs/elevator-cmd-norm")
            << '\n'

            << "  pitch trim: "
            << fdm_->GetPropertyValue(
                   "fcs/pitch-trim-cmd-norm")
            << '\n'

            << "  rudder: "
            << fdm_->GetPropertyValue(
                   "fcs/rudder-cmd-norm")
            << '\n'

            << "  throttle: "
            << fdm_->GetPropertyValue(
                   "fcs/throttle-cmd-norm[0]")
            << '\n';
    }

    return true;
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
    const SimulationActuators &actuators)
{
    fdm_->SetPropertyValue(
        "fcs/aileron-cmd-norm",
        actuators.aileron);

    fdm_->SetPropertyValue(
        "fcs/elevator-cmd-norm",
        actuators.elevator);

    fdm_->SetPropertyValue(
        "fcs/rudder-cmd-norm",
        actuators.rudder);

    fdm_->SetPropertyValue(
        "fcs/throttle-cmd-norm[0]",
        actuators.throttle);
}

bool JSBSimAdapter::step()
{
    return fdm_->Run();
}

double JSBSimAdapter::getTime() const
{
    return fdm_->GetSimTime();
}

SimulationActuators JSBSimAdapter::getTrim() const
{
    return trim_;
}
