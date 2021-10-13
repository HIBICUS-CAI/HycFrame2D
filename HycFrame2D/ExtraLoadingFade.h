#pragma once

#include "Actor_all.h"

void RegisterExtraLoadingFade(ObjectFactory* _factory);

void ExtraLoadFadeInit(AInteractionComponent* _aitc);

void ExtraLoadFadeUpdate(AInteractionComponent* _aitc,
    float _deltatime);

void ExtraLoadFadeDestory(AInteractionComponent* _aitc);

bool GetExtraSceneCanUpdate();

void SetExtraSceneEnd(bool _flag);
