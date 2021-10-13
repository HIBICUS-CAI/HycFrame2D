#pragma once

#include "Actor_all.h"
#include "Ui_all.h"

void RegisterBowLoadingFade(ObjectFactory* _factory);

void BowLoadFadeInit(AInteractionComponent* _aitc);

void BowLoadFadeUpdate(AInteractionComponent* _aitc,
    float _deltatime);

void BowLoadFadeDestory(AInteractionComponent* _aitc);

bool GetBowSceneCanUpdate();

void SetBowSceneEnd(bool _success);

bool GetBowSceneIsSuccess();
