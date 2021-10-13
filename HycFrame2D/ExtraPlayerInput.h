#pragma once

#include "Actor_all.h"

void RegisterExtraPlayerInput(ObjectFactory* _factory);

void Player1Input(AInputComponent* _aic, float _deltatime);

void Player2Input(AInputComponent* _aic, float _deltatime);

bool* GetReadyPickArray();
