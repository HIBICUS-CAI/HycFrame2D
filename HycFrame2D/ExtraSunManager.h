#pragma once

#include "Actor_all.h"

void RegisterExtraSunsManager(ObjectFactory* _factory);

void ExtraSunsInit(AInteractionComponent* _aitc);

void ExtraSunsUpdate(AInteractionComponent* _aitc,
    float _deltatime);

void ExtraSunsDestory(AInteractionComponent* _aitc);

void SetSunShoot(ActorObject* _player, Float3 _spdVec);
