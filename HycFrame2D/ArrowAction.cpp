#include "ArrowAction.h"
#include "BowAction.h"
#include "ModeSelectButton.h"
#include "AimPointAction.h"
#include "controller.h"

static float g_VPos[8] =
{
    0.f, 70.f, 120.f, 180.f, 220.f, 280.f, 310.f, 320.f
};

static Float3 g_DefaultPos = MakeFloat3(0.f, 0.f, 0.f);

void RegisterArrowAction(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(ArrowActionInit), ArrowActionInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(ArrowActionUpdate), ArrowActionUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(ArrowActionDestory), ArrowActionDestory));
}

void ArrowActionInit(AInteractionComponent* _aitc)
{
    g_DefaultPos = ((ATransformComponent*)(_aitc->
        GetActorObjOwner()->
        GetSceneNodePtr()->GetActorObject("bow-actor")->
        GetAComponent("bow-actor-transform")))->GetPosition();
    g_DefaultPos.x += 5.f;
    g_DefaultPos.y -= 380.f;

    ((ATransformComponent*)(_aitc->
        GetActorObjOwner()->
        GetAComponent("arrow-actor-transform")))->
        SetPosition(g_DefaultPos);
}

void ArrowActionUpdate(AInteractionComponent* _aitc,
    float _deltatime)
{
    if (CanReadyToShoot() &&
        GetControllerPress(NpadButton::ZR::Index))
    {
        unsigned int max = 0;
        if (GetPlayMode() == PLAYMODE::JOYCON)
        {
            max = 550;
        }
        else if (GetPlayMode() == PLAYMODE::BUTTON)
        {
            max = 100;
        }
        int level = (int)((double)GetBowStrength() /
            ((double)max / (double)8));
        if (level < 0) { level = 0; }
        if (level > 7) { level = 7; }

        Float3 nowPos = g_DefaultPos;
        nowPos.y += g_VPos[level];

        ((ATransformComponent*)(_aitc->
            GetActorObjOwner()->
            GetAComponent("arrow-actor-transform")))->
            SetPosition(nowPos);

        ((ASpriteComponent*)(_aitc->
            GetActorObjOwner()->
            GetAComponent("arrow-actor-sprite")))->
            SetVisible(true);
    }
    else
    {
        ((ASpriteComponent*)(_aitc->
            GetActorObjOwner()->
            GetAComponent("arrow-actor-sprite")))->
            SetVisible(false);

        ((ATransformComponent*)(_aitc->
            GetActorObjOwner()->
            GetAComponent("arrow-actor-transform")))->
            SetPosition(g_DefaultPos);
    }
}

void ArrowActionDestory(AInteractionComponent* _aitc)
{

}

void TranslateArrow(Float3 _offset)
{
    g_DefaultPos.x += _offset.x;
    g_DefaultPos.y += _offset.y;
    g_DefaultPos.z += _offset.z;
}
