#pragma once

#include "Actor_all.h"
#include "Ui_all.h"

void RegisterResultLoadingFade(ObjectFactory* _factory);

void ResultLoadFadeInit(AInteractionComponent* _aitc);

void ResultLoadFadeUpdate(AInteractionComponent* _aitc,
    float _deltatime);

void ResultLoadFadeDestory(AInteractionComponent* _aitc);

bool GetResultSceneCanUpdate();

void SetResultSceneEnd(bool _flag);
