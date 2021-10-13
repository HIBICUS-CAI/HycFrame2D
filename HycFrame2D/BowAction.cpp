#include "BowAction.h"
#include "ModeSelectButton.h"
#include "BowLoadingFade.h"
#include "AimPointAction.h"
#include "controller.h"
#include <math.h>

static unsigned int g_Strength = 0;

static unsigned int g_Flag = 0;
static float g_TimeUp = 0.f;
static float g_TimeMiddle = 0.f;
static float g_TimeDown = 0.f;
static float g_Time = 0.2f;
static const float PI = 3.1415926535f;

void RunBowActionWithJoycon(AInteractionComponent* _aitc,
    float _deltatime)
{
    int level = (int)((double)g_Strength /
        ((double)550 / (double)8));
    if (level < 0) { level = 0; }
    if (level > 7) { level = 7; }
    ((AAnimateComponent*)(_aitc->
        GetActorObjOwner()->GetAComponent("bow-actor-animate")))->
        ChangeAnimateTo(std::to_string(level));
}

void RunBowActionWithButton(AInteractionComponent* _aitc,
    float _deltatime)
{
    int level = (int)((double)g_Strength /
        ((double)100 / (double)8));
    if (level < 0) { level = 0; }
    if (level > 7) { level = 7; }
    ((AAnimateComponent*)(_aitc->
        GetActorObjOwner()->GetAComponent("bow-actor-animate")))->
        ChangeAnimateTo(std::to_string(level));
}

void InputBowActionWithJoycon(AInputComponent* _aic,
    float _deltatime)
{
    if (!CanReadyToShoot() ||
        !GetControllerPress(NpadButton::ZR::Index))
    {
        g_Strength = 0;

        g_TimeUp = 0.f;
        g_TimeMiddle = 0.f;
        g_TimeDown = 0.f;
        SetControllerLeftVibration(5);
        SetControllerRightVibration(5);
    }
    else
    {
        float rightAcc = GetControllerRightAcceleration().y;
        if (rightAcc > 0.4f || rightAcc < -0.4f)
        {
            rightAcc = -rightAcc;
            int rightAccInt = (int)(rightAcc * 100.f);

            if (rightAccInt >= 40 && rightAccInt <= 150)
            {
                g_Strength +=
                    ((rightAccInt - 40) / 20 + 1) * 5;
            }
            else if (rightAccInt > 150)
            {
                g_Strength += 45;
            }
        }

        if ((g_Flag++ / ((int)(60.f * g_Time)) % 3) == 0)
        {
            g_TimeMiddle = 0.f;
            g_TimeDown = 0.f;
            int frameCount = (int)(60.f * g_Time);
            g_TimeUp += PI / (float)frameCount;
            float factor = sin(g_TimeUp) * 
                (float)g_Strength / (float)550;
            SetControllerLeftVibration(5);
            SetControllerRightVibration(5);
        }
        else if ((g_Flag / ((int)(60.f * g_Time)) % 3) == 1)
        {
            g_TimeUp = 0.f;
            g_TimeDown = 0.f;
            int frameCount = (int)(60.f * g_Time);
            g_TimeMiddle += PI / (float)frameCount;
            float factor = sin(g_TimeMiddle) *
                (float)g_Strength / (float)550;
            SetControllerLeftVibration(5);
            SetControllerRightVibration(5);
        }
        else
        {
            g_TimeUp = 0.f;
            g_TimeMiddle = 0.f;
            int frameCount = (int)(60.f * g_Time);
            g_TimeDown += PI / (float)frameCount;
            float factor = sin(g_TimeDown) *
                (float)g_Strength / (float)550;
            factor = (factor < 0.f) ? -factor : factor;
            SetControllerLeftVibration(5);
            SetControllerRightVibration(5);
        }
    }
}

void InputBowActionWithButton(AInputComponent* _aic,
    float _deltatime)
{
    if (!CanReadyToShoot() ||
        !GetControllerPress(NpadButton::ZR::Index))
    {
        g_Strength = 0;

        g_TimeUp = 0.f;
        g_TimeMiddle = 0.f;
        g_TimeDown = 0.f;
    }
    else
    {
        float rightOffset = GetControllerRightStick().y;
        int strength = (int)(-rightOffset * 100.f);
        if (strength < 0)
        {
            strength = 0;
        }
        g_Strength = strength;

        if ((g_Flag++ / ((int)(60.f * g_Time)) % 3) == 0)
        {
            g_TimeMiddle = 0.f;
            g_TimeDown = 0.f;
            int frameCount = (int)(60.f * g_Time);
            g_TimeUp += PI / (float)frameCount;
            float factor = sin(g_TimeUp) *
                (float)g_Strength / (float)100;
            SetControllerLeftVibration(5);
            SetControllerRightVibration(5);
        }
        else if ((g_Flag / ((int)(60.f * g_Time)) % 3) == 1)
        {
            g_TimeUp = 0.f;
            g_TimeDown = 0.f;
            int frameCount = (int)(60.f * g_Time);
            g_TimeMiddle += PI / (float)frameCount;
            float factor = sin(g_TimeMiddle) *
                (float)g_Strength / (float)100;
            SetControllerLeftVibration(5);
            SetControllerRightVibration(5);
        }
        else
        {
            g_TimeUp = 0.f;
            g_TimeMiddle = 0.f;
            int frameCount = (int)(60.f * g_Time);
            g_TimeDown += PI / (float)frameCount;
            float factor = sin(g_TimeDown) *
                (float)g_Strength / (float)100;
            factor = (factor < 0.f) ? -factor : factor;
            SetControllerLeftVibration(5);
            SetControllerRightVibration(5);
        }
    }
}

void RegisterBowAction(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();
    auto aInputPool = _factory->GetActorInputPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(BowActionInit), BowActionInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(BowActionUpdate), BowActionUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(BowActionDestory), BowActionDestory));
    aInputPool->insert(std::make_pair(
        FUNC_NAME(BowActionInput), BowActionInput));
}

void BowActionInit(AInteractionComponent* _aitc)
{
    g_Strength = 0;
}

void BowActionUpdate(AInteractionComponent* _aitc,
    float _deltatime)
{
    if (GetBowSceneCanUpdate())
    {
        switch (GetPlayMode())
        {
        case PLAYMODE::JOYCON:
            RunBowActionWithJoycon(_aitc, _deltatime);
            break;

        case PLAYMODE::BUTTON:
            RunBowActionWithButton(_aitc, _deltatime);
            break;

        default:
            break;
        }
    }
}

void BowActionDestory(AInteractionComponent* _aitc)
{
    g_Strength = 0;
}

void BowActionInput(AInputComponent* _aic, float _deltatime)
{
    if (GetBowSceneCanUpdate())
    {
        switch (GetPlayMode())
        {
        case PLAYMODE::JOYCON:
            InputBowActionWithJoycon(_aic, _deltatime);
            break;

        case PLAYMODE::BUTTON:
            InputBowActionWithButton(_aic, _deltatime);
            break;

        default:
            break;
        }
    }
}

unsigned int GetBowStrength()
{
    return g_Strength;
}

void TranslateBow(ActorObject* _bow, Float3 _offset)
{
    ((ATransformComponent*)(_bow->
        GetAComponent("bow-actor-transform")))->
        Translate(_offset);
}
