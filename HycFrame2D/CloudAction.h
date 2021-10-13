#pragma once

#include "Actor_all.h"

void RegisterCloudAction(ObjectFactory* _factory);

void CloudInit(AInteractionComponent* _aitc);

void CloudUpdate(AInteractionComponent* _aitc, float _deltatime);

void CloudDestory(AInteractionComponent* _aitc);
