#include "ControllerHelper.h"
#include <iostream>

void InitController()
{
    if (!InputInterface::StartUp())
    {
        P_LOG(LOG_ERROR, "failed to start up input system\n");
    }
}

void UninitController()
{
    InputInterface::CleanAndStop();
}

void UpdateController()
{
    InputInterface::PollDevices();
}

bool GetControllerPress(UINT button)
{
    return InputInterface::IsKeyDownInSingle(button);
}

bool GetControllerTrigger(UINT button)
{
    return InputInterface::IsKeyPushedInSingle(button);
}

Float2 GetControllerLeftStick()
{
    Float2 stick = { 0.f,0.f };
    STICK_OFFSET ls = InputInterface::LeftStickOffset();
    stick.x = (float)ls.x / 1000.f;
    stick.y = -(float)ls.y / 1000.f;

    return stick;
}

Float2 GetControllerRightStick()
{
    Float2 stick = { 0.f,0.f };
    STICK_OFFSET ls = InputInterface::RightStickOffset();
    stick.x = (float)ls.x / 1000.f;
    stick.y = (float)ls.y / 1000.f;

    return stick;
}

void SetControllerLeftVibration(int frame)
{
    P_LOG(LOG_WARNING, "did not support vibration here\n");
}

void SetControllerRightVibration(int frame)
{
    P_LOG(LOG_WARNING, "did not support vibration here\n");
}

Float3 GetControllerLeftAcceleration()
{
    return Float3{};
}

Float3 GetControllerRightAcceleration()
{
    return Float3{};
}

Float3 GetControllerLeftAngle()
{
    return Float3 {};
}

Float3 GetControllerRightAngle()
{
    return Float3 {};
}

bool GetControllerTouchScreen()
{
    return false;
}

Float2 GetControllerTouchScreenPosition()
{
    Float2 pos = { 0.f,0.f };

    return pos;
}
