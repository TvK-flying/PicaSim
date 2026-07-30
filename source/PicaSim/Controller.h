#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller
{
public:
    enum Channel
    {
        CHANNEL_AILERONS  = 0,
        CHANNEL_ELEVATOR  = 1,
        CHANNEL_RUDDER    = 2,
        CHANNEL_THROTTLE  = 3,
        CHANNEL_LOOKYAW   = 4,
        CHANNEL_LOOKPITCH = 5,
        CHANNEL_AUX1      = 6,
        CHANNEL_SMOKE1    = 7,
        CHANNEL_SMOKE2    = 8,
        CHANNEL_HOOK      = 9,
        MAX_CHANNELS      = 10
    };

    virtual ~Controller() {}

    /// Controls range from -1 to +1
    virtual float GetControl(Channel channel) const = 0;

    /// Elevator trim control from -1 to 1
    virtual float GetElevatorTrim() const {return 0.0f;}

    // control is ControllerSettings::ControllerControls
    virtual void SetInputControl(int control, float value) {}
    virtual float GetInputControl(int control) const {return 0.0f;}

    /// 4D thrust-vectoring mode (values match
    /// GameSettings::ControllerSettings::VectorMode). Only meaningful when this
    /// controller has 4D mode enabled; returns VECTOR_MODE_NONE (0) otherwise,
    /// including for controllers (AI, replay, etc.) that don't support 4D at all.
    virtual int GetVectorMode() const {return 0;}

    /// Whether 4D mode (reverse thrust) is actually enabled on this controller.
    /// GetVectorMode() alone can't distinguish "4D is off" from "4D is on with
    /// 'No vector' selected" - both return 0 - so anything that should only ever
    /// apply while 4D is on (e.g. reduced control throw while flying backward)
    /// must check this too, not just branch on GetVectorMode()'s value.
    virtual bool GetEnable4DMode() const {return false;}

};

#endif
