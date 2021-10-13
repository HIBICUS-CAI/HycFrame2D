#pragma once

#include "Ui_all.h"

void RegisterStoryAction(ObjectFactory* _factory);

void StoryLoadFadeInit(UInteractionComponent* _uitc);

void StoryLoadFadeUpdate(UInteractionComponent* _uitc,
    float _deltatime);

void StoryLoadFadeDestory(UInteractionComponent* _uitc);

void StoryUiInput(UInputComponent* _uic, float _deltatime);

void SetStorySceneEnd();
