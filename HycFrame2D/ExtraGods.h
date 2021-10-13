#pragma once

#include "Actor_all.h"

void RegisterExtraGodsManager(ObjectFactory* _factory);

void GodsInit(AInteractionComponent* _aitc);

void GodsUpdate(AInteractionComponent* _aitc, float _deltatime);

void GodsDestory(AInteractionComponent* _aitc);
