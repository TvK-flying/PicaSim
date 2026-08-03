#include "PicaJoystick.h"
#include "Trace.h"
#include "GameSettings.h"
#include "../Platform/S3ECompat.h"
#include <cstring>

// Was a single global JoystickData that every id read from, regardless of
// which id was asked for - meaning only one physical device could ever be
// tracked at a time, no matter how many were plugged in. Now one slot per
// device id, so two (or more) joysticks can be polled and read independently
// in the same frame - this is what split-screen's two transmitters need.
static JoystickData sJoystickData[MAX_JOYSTICK_DEVICES];

//======================================================================================================================
JoystickData::JoystickData() 
{
    memset(this, 0, sizeof(*this));
}

//======================================================================================================================
bool JoystickAvailable()
{
    return gamepadAvailable() && gamepadGetNumDevices() > 0;
}

//======================================================================================================================
s3eResult GetJoystickStatus(JoystickData& joystick, int id)
{
    TRACE_FUNCTION_ONLY(ONCE_2);

    if (!JoystickAvailable())
        return S3E_RESULT_ERROR;

    if (id < 0 || id >= MAX_JOYSTICK_DEVICES)
        return S3E_RESULT_ERROR;

    joystick = sJoystickData[id];

    return S3E_RESULT_SUCCESS;
}

//======================================================================================================================
void CalibrateJoystick()
{
    gamepadCalibrate();
}

//======================================================================================================================
void UpdateJoystick(int id)
{
    TRACE_FILE_IF(FRAME_1) TRACE("Updating joystick %d", id);

    gamepadUpdate();

    if (id < 0 || id >= MAX_JOYSTICK_DEVICES)
        return;

    JoystickData& data = sJoystickData[id];
    data = JoystickData();
    if (JoystickAvailable() && id >= 0 && id < (int) gamepadGetNumDevices())
    {
        TRACE_FILE_IF(FRAME_2) TRACE("Copying gamepad results");
        // Map [-4096,4096] to [0,65535]
        uint32 numAxes = gamepadGetNumAxes(id);
        for (uint32 i = 0 ; i < numAxes && i < JoystickData::MAX_ANALOGUEINPUTS ; ++i)
            data.mAnalogueInputs[i] = (long) ((4096 + gamepadGetAxis(id, i)) * 65535.0f/8192.0f);

        if (gamepadIsPointOfViewAvailable(id))
            data.mPOVDirections[0] = gamepadGetPointOfViewAngle(id);
  
        uint32 buttons = gamepadGetButtons(id);
        for (uint32_t i = 0 ; i != JoystickData::MAX_BUTTONS ; ++i)
            data.mButtons[i] = (buttons & 1 << i) ? 128 : 0;

#if 0
        char txt[64];
        sprintf(txt, "buttons = %u", buttons);
        IwGxPrintString(10, 10, txt);
#endif

        sprintf(data.mName, "Joystick %d", id);
    }
    else
    {
        sprintf(data.mName, "Joystick %d unavailable", id);
        TRACE_FILE_IF(FRAME_2) TRACE("No gamepad available");
    }
}

//======================================================================================================================
void ResetJoystick()
{
    gamepadReset();
}

//======================================================================================================================
bool ShowJoystickInGame(const GameSettings& gameSettings)
{
    if (gameSettings.mOptions.mFrameworkSettings.isIOS())
        return false;

    // Note: Previously checked for Android API level < 11, but that's ancient (Android 3.0)
    // and no longer relevant - all supported Android devices are newer

    return true;
}

