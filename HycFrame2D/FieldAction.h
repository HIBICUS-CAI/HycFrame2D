#pragma once

#include "Actor_all.h"

void RegisterFieldAction(ObjectFactory* _factory);

void FieldInit(AInteractionComponent* _aitc);

void FieldUpdate(AInteractionComponent* _aitc, float _deltatime);

void FieldDestory(AInteractionComponent* _aitc);
