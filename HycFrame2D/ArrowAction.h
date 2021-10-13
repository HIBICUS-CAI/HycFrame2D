#pragma once

#include "Actor_all.h"

void RegisterArrowAction(ObjectFactory* _factory);

void ArrowActionInit(AInteractionComponent* _aitc);

void ArrowActionUpdate(AInteractionComponent* _aitc,
    float _deltatime);

void ArrowActionDestory(AInteractionComponent* _aitc);

void TranslateArrow(Float3 _offset);
