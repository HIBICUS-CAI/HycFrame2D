#include "ExtraPlayerInput.h"
#include "Actor_all.h"
#include "ExtraLoadingFade.h"
#include "ExtraSunManager.h"
#include "controller.h"

static bool g_ReadyToPick[2] = { false,false };
static bool g_ReadyToShoot[2] = { false,false };

void RegisterExtraPlayerInput(ObjectFactory* _factory)
{
    auto aInputPool = _factory->GetActorInputPool();

    aInputPool->insert(std::make_pair(
        FUNC_NAME(Player1Input), Player1Input));
    aInputPool->insert(std::make_pair(
        FUNC_NAME(Player2Input), Player2Input));
}

void Player1Input(AInputComponent* _aic, float _deltatime)
{
    if (!GetExtraSceneCanUpdate())
    {
        return;
    }

    static Float2 stick = MakeFloat2(0.f, 0.f);
    static Float3 offset = MakeFloat3(0.f, 0.f, 0.f);
    stick = GetControllerLeftStick();
    offset.x = stick.x * 10.f;
    offset.y = -stick.y * 10.f;
    offset.z = 0.f;

    if (!g_ReadyToShoot[0])
    {
        ((ATransformComponent*)(_aic->GetActorObjOwner()->
            GetAComponent("player-1-actor-transform")))->
            Translate(offset);
    }

    offset = ((ATransformComponent*)(_aic->GetActorObjOwner()->
        GetAComponent("player-1-actor-transform")))->
        GetPosition();
    static bool flag = false;
    flag = false;
    if (offset.x > 700.f)
    {
        offset.x = 700.f;
        flag = true;
    }
    else if (offset.x < -700.f)
    {
        offset.x = -700.f;
        flag = true;
    }
    if (offset.y > 300.f)
    {
        offset.y = 300.f;
        flag = true;
    }
    else if (offset.y < -300.f)
    {
        offset.y = -300.f;
        flag = true;
    }
    if (flag)
    {
        ((ATransformComponent*)(_aic->GetActorObjOwner()->
            GetAComponent("player-1-actor-transform")))->
            SetPosition(offset);
    }

    if (GetControllerTrigger(NpadButton::Down::Index))
    {
        g_ReadyToPick[0] = true;
    }
    else
    {
        g_ReadyToPick[0] = false;
    }

    if (GetControllerPress(NpadButton::Right::Index))
    {
        g_ReadyToShoot[0] = true;
    }
    if (g_ReadyToShoot[0] &&
        !GetControllerPress(NpadButton::Right::Index))
    {
        g_ReadyToShoot[0] = false;
        stick = GetControllerLeftStick();
        offset.x = stick.x * 100.f;
        offset.y = -stick.y * 100.f;
        offset.z = 0.f;
        SetSunShoot(_aic->GetActorObjOwner(), offset);
    }
}

void Player2Input(AInputComponent* _aic, float _deltatime)
{
    if (!GetExtraSceneCanUpdate())
    {
        return;
    }

    static Float2 stick = MakeFloat2(0.f, 0.f);
    static Float3 offset = MakeFloat3(0.f, 0.f, 0.f);
    stick = GetControllerRightStick();
    offset.x = stick.x * 10.f;
    offset.y = -stick.y * 10.f;
    offset.z = 0.f;

    if (!g_ReadyToShoot[1])
    {
        ((ATransformComponent*)(_aic->GetActorObjOwner()->
            GetAComponent("player-2-actor-transform")))->
            Translate(offset);
    }
    offset = ((ATransformComponent*)(_aic->GetActorObjOwner()->
        GetAComponent("player-2-actor-transform")))->
        GetPosition();
    static bool flag = false;
    flag = false;
    if (offset.x > 700.f)
    {
        offset.x = 700.f;
        flag = true;
    }
    else if (offset.x < -700.f)
    {
        offset.x = -700.f;
        flag = true;
    }
    if (offset.y > 300.f)
    {
        offset.y = 300.f;
        flag = true;
    }
    else if (offset.y < -300.f)
    {
        offset.y = -300.f;
        flag = true;
    }
    if (flag)
    {
        ((ATransformComponent*)(_aic->GetActorObjOwner()->
            GetAComponent("player-2-actor-transform")))->
            SetPosition(offset);
    }

    if (GetControllerTrigger(NpadButton::X::Index))
    {
        g_ReadyToPick[1] = true;
    }
    else
    {
        g_ReadyToPick[1] = false;
    }

    if (GetControllerPress(NpadButton::Y::Index))
    {
        g_ReadyToShoot[1] = true;
    }
    if (g_ReadyToShoot[1] &&
        !GetControllerPress(NpadButton::Y::Index))
    {
        g_ReadyToShoot[1] = false;
        stick = GetControllerRightStick();
        offset.x = stick.x * 100.f;
        offset.y = -stick.y * 100.f;
        offset.z = 0.f;
        SetSunShoot(_aic->GetActorObjOwner(), offset);
    }
}

bool* GetReadyPickArray()
{
    return g_ReadyToPick;
}
