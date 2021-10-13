#pragma once

#include "Actor_all.h"
#include "Ui_all.h"

enum class PLAYMODE
{
    NONE,
    JOYCON,
    BUTTON
};

enum class DIFFICULTY
{
    NONE,
    EASY,
    NORMAL,
    HARD
};

void RegisterModeSelectButton(ObjectFactory* _factory);

void SelectLoadFadeInit(AInteractionComponent* _aitc);

void SelectLoadFadeUpdate(AInteractionComponent* _aitc,
    float _deltatime);

void SelectLoadFadeDestory(AInteractionComponent* _aitc);

void ModeSelectInput(UInputComponent* _uic, float _deltatime);

void SetPlayMode(PLAYMODE _mode);

PLAYMODE GetPlayMode();

void ResetPlayMode();

void SetDifficulty(DIFFICULTY _mode);

DIFFICULTY GetDifficulty();

void ResetDifficulty();
