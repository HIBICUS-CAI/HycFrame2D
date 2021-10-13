#pragma once

#include "Ui_all.h"

void RegisterExtraLife(ObjectFactory* _factory);

void LifeUiInit(UInteractionComponent* _uitc);

void LifeUiUpdate(UInteractionComponent* _uitc,
    float _deltatime);

void LifeUiDestory(UInteractionComponent* _uitc);

void LifeUiInput(UInputComponent* _uic, float _deltatime);

void SubPlayerLeftHp();

void SubPlayerRightHp();
