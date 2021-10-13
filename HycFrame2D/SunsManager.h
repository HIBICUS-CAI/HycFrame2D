#pragma once

#include "Actor_all.h"

void RegisterSunsManager(ObjectFactory* _factory);

void SunsInit(AInteractionComponent* _aitc);

void SunsUpdate(AInteractionComponent* _aitc, float _deltatime);

void SunsDestory(AInteractionComponent* _aitc);

float GetSunsRatio();
