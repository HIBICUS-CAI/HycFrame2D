#pragma once

#include "Actor_all.h"
#include "Ui_all.h"

void RegisterTitleButton(ObjectFactory* _factory);

void ButtonSelectInput(UInputComponent* _uic, float _deltatime);

void TitleLoadFadeInit(AInteractionComponent* _aitc);

void TitleLoadFadeUpdate(AInteractionComponent* _aitc, 
    float _deltatime);

void TitleLoadFadeDestory(AInteractionComponent* _aitc);
