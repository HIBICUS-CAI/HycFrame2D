#pragma once

#include "Actor_all.h"

void RegisterBowAction(ObjectFactory* _factory);

void BowActionInit(AInteractionComponent* _aitc);

void BowActionUpdate(AInteractionComponent* _aitc,
    float _deltatime);

void BowActionDestory(AInteractionComponent* _aitc);

void BowActionInput(AInputComponent* _aic, float _deltatime);

unsigned int GetBowStrength();

void TranslateBow(ActorObject* _bow, Float3 _offset);
