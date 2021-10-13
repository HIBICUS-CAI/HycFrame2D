#pragma once

#include "Ui_all.h"

void RegisterManualAction(ObjectFactory* _factory);

void ManualLoadFadeInit(UInteractionComponent* _uitc);

void ManualLoadFadeUpdate(UInteractionComponent* _uitc,
    float _deltatime);

void ManualLoadFadeDestory(UInteractionComponent* _uitc);

void ManualUiInput(UInputComponent* _uic, float _deltatime);

void SetManualSceneEnd();
