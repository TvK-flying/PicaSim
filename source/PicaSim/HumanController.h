#ifndef HUMAN_CONTROLLER_H
#define HUMAN_CONTROLLER_H

#include "Controller.h"
#include "GameSettings.h"

#include "Framework.h"

#include <memory>

class HumanController : public Controller, public Entity, public RenderOverlayObject
{
public:
    HumanController(GameSettings& gameSettings);
    ~HumanController();

    void SetAeroplane(const class Aeroplane* aeroplane) {mAeroplane = aeroplane;}

    // Which physical joystick device this controller reads from. Defaults to
    // -1, meaning "use the shared GameSettings.mOptions.mJoystickID", which
    // is exactly the old single-player behaviour. Set this explicitly to
    // give a second (or third...) HumanController its own transmitter.
    void SetJoystickId(int joystickId) {mJoystickId = joystickId;}

    // Which calibration/button-mapping table to use for this controller's
    // joystick input. Defaults to null, meaning "use the shared
    // GameSettings.mJoystickSettings" (old single-player behaviour). Point
    // this at a different JoystickSettings (e.g. GameSettings.mJoystickSettings2)
    // to give a second player their own independent calibration - the object
    // pointed to must outlive this HumanController.
    void SetJoystickSettings(const JoystickSettings* js) {mJoystickSettings = js;}

    // Shifts this controller's on-screen stick overlay up/down as a fraction
    // of screen height, purely so two controllers' overlays don't render
    // exactly on top of each other before split-screen rendering exists.
    void SetOverlayYOffset(float offset) {mLeftControllerPosY += offset; mRightControllerPosY += offset;}

    void EntityUpdate(float deltaTime, int entityLevel) OVERRIDE;

    float GetControl(Channel channel) const OVERRIDE;

    void SetInputControl(int control, float value) OVERRIDE;
    float GetInputControl(int control) const OVERRIDE;

    float GetElevatorTrim() const OVERRIDE {return mElevatorTrim;}

    void RenderOverlayUpdate(int renderLevel, DisplayConfig& displayConfig) OVERRIDE;

private:
    void RenderStick(float xMid, float yMid, bool renderStickCross, 
        float controlX, float controlY, float controllerSizeX, float controllerSizeY, 
        float stickIndicatorSizeX, float stickIndicatorSizeY, const class ControllerShader* controllerShader);
    void RenderController(float xMid, float yMid, float controllerSizeX, float controllerSizeY, 
        Options::ControllerStyle style, const class ControllerShader* controllerShader);
    void UpdateKeyboard(float deltaTime);
    void UpdateAccelerometer(float deltaTime);
    void UpdateScreenSticks(float deltaTime);
    void UpdateJoystick(float deltaTime);

    GameSettings& mGameSettings;
    const class Aeroplane* mAeroplane;
    int mJoystickId = -1; // -1 = use the shared GameSettings.mOptions.mJoystickID
    const JoystickSettings* mJoystickSettings = nullptr; // null = use the shared GameSettings.mJoystickSettings

    float mOutputControls[MAX_CHANNELS];
    float mInputControls[ControllerSettings::CONTROLLER_NUM_CONTROLS];
    float mProcessedInputControls[ControllerSettings::CONTROLLER_NUM_CONTROLS];

    enum {NUM_BUTTONS = 3};
    std::unique_ptr<ButtonOverlay> mButtonOverlay[NUM_BUTTONS];

    float mRightStickX; // -1 to 1
    float mRightStickY;
    float mLeftStickX;
    float mLeftStickY;
    float mAccelX;
    float mAccelY;

    float mStickIndicatorSize;

    // The size of each controller range in pixels
    float mControllerSizeX, mControllerSizeY;

    // The position of each controller in fractions
    float mRightControllerPosX;
    float mRightControllerPosY;
    float mLeftControllerPosX;
    float mLeftControllerPosY;

    // For when using the relative touch
    bool  mRightWasTouched;
    float mRightInitialTouchPosX;
    float mRightInitialTouchPosY;
    bool  mLeftWasTouched;
    float mLeftInitialTouchPosX;
    float mLeftInitialTouchPosY;

    float mElevatorTrim;
};

#endif
