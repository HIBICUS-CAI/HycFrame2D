#pragma once

#include "main.h"
#include "ID_Interface.h"

void InitController();

void UninitController();

void UpdateController();

bool GetControllerPress(UINT button);

bool GetControllerTrigger(UINT button);

Float2 GetControllerLeftStick();

Float2 GetControllerRightStick();

void SetControllerLeftVibration(int frame);

void SetControllerRightVibration(int frame);

Float3 GetControllerLeftAcceleration();

Float3 GetControllerRightAcceleration();

Float3 GetControllerLeftAngle();

Float3 GetControllerRightAngle();

bool GetControllerTouchScreen();

Float2 GetControllerTouchScreenPosition();