#pragma once

#include "Actor_all.h"

void RegisterAimPointAction(ObjectFactory* _factory);

void AimPointInput(AInputComponent* _aic, float _deltatime);

void AimPointInit(AInteractionComponent* _aitc);

void AimPointUpdate(AInteractionComponent* _aitc, 
    float _deltatime);

void AimPointDestory(AInteractionComponent* _aitc);

bool CanReadyToShoot();

void ResetActiveArrowStar(ActorObject* _star);
