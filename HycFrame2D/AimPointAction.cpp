#include "AimPointAction.h"
#include "Ui_all.h"
#include "ModeSelectButton.h"
#include "BowLoadingFade.h"
#include "BowAction.h"
#include "ArrowAction.h"
#include "controller.h"
#include <math.h>
#include "sound.h"

static float g_AimSpdFactor = 1.f;

static bool g_ArrowCanUse[3] = { true,true,true };
static Float3 g_AimPos[3] =
{
    MakeFloat3(0.f,0.f,0.f),
    MakeFloat3(0.f,0.f,0.f),
    MakeFloat3(0.f,0.f,0.f)
};
static float g_Speed[3] = { 0.f,0.f,0.f };
static ActorObject* g_Star[3] = { nullptr,nullptr,nullptr };

static unsigned int g_BowHp = 0;
static unsigned int g_BowHpMax = 0;
static UiObject* g_BowHpBar = nullptr;

void JoyconAimPointInput(AInputComponent* _aic, float _deltatime)
{
    Camera* cam = _aic->GetActorObjOwner()->
        GetSceneNodePtr()->GetCamera();

    Float3 angleSpeed = MakeFloat3(0.f, 0.f, 0.f);
    if (!GetControllerPress(NpadButton::L::Index))
    {
        angleSpeed = GetControllerLeftAngle();
    }
    Float2 camOffset = MakeFloat2(0.f, 0.f);
    if (angleSpeed.y > 0.05f || angleSpeed.y < -0.05f)
    {
        camOffset.y = angleSpeed.y * g_AimSpdFactor * -50.f;
    }
    if (angleSpeed.z > 0.05f || angleSpeed.z < -0.05f)
    {
        camOffset.x = angleSpeed.z * g_AimSpdFactor * 50.f;
    }

    Float2 oldCamPos = cam->GetCameraPosition();
    oldCamPos.x += camOffset.x;
    oldCamPos.y += camOffset.y;
    if (oldCamPos.x < -1920.f)
    {
        float delta = -1920.f - oldCamPos.x;
        camOffset.x += delta;
    }
    else if (oldCamPos.x > 1920.f)
    {
        float delta = oldCamPos.x - 1920.f;
        camOffset.x -= delta;
    }
    if (oldCamPos.y > 0.f)
    {
        float delta = oldCamPos.y - 0.f;
        camOffset.y -= delta;
    }
    else if (oldCamPos.y < -2160.f)
    {
        float delta = -2160.f - oldCamPos.y;
        camOffset.y += delta;
    }

    cam->TranslateCameraPos(camOffset);

    ((ATransformComponent*)(_aic->GetActorObjOwner()->
        GetAComponent("aim-point-actor-transform")))->
        Translate(
            MakeFloat3(camOffset.x, camOffset.y, 0.f));

    TranslateArrow(MakeFloat3(camOffset.x, camOffset.y, 0.f));
    TranslateBow(_aic->GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject("bow-actor"),
        MakeFloat3(camOffset.x, camOffset.y, 0.f));
}

void ButtonAimPointInput(AInputComponent* _aic, float _deltatime)
{
    Camera* cam = _aic->GetActorObjOwner()->
        GetSceneNodePtr()->GetCamera();

    Float2 stickOffset = GetControllerLeftStick();
    stickOffset.x = stickOffset.x * g_AimSpdFactor * 1.f;
    stickOffset.y = stickOffset.y * g_AimSpdFactor * -1.f;

    Float2 oldCamPos = cam->GetCameraPosition();
    oldCamPos.x += stickOffset.x;
    oldCamPos.y += stickOffset.y;
    if (oldCamPos.x < -1920.f)
    {
        float delta = -1920.f - oldCamPos.x;
        stickOffset.x += delta;
    }
    else if (oldCamPos.x > 1920.f)
    {
        float delta = oldCamPos.x - 1920.f;
        stickOffset.x -= delta;
    }
    if (oldCamPos.y > 0.f)
    {
        float delta = oldCamPos.y - 0.f;
        stickOffset.y -= delta;
    }
    else if (oldCamPos.y < -2160.f)
    {
        float delta = -2160.f - oldCamPos.y;
        stickOffset.y += delta;
    }
    cam->TranslateCameraPos(stickOffset);

    ((ATransformComponent*)(_aic->GetActorObjOwner()->
        GetAComponent("aim-point-actor-transform")))->
        Translate(
            MakeFloat3(stickOffset.x, stickOffset.y, 0.f));

    TranslateArrow(MakeFloat3(stickOffset.x, stickOffset.y, 0.f));
    TranslateBow(_aic->GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject("bow-actor"),
        MakeFloat3(stickOffset.x, stickOffset.y, 0.f));
}

void RegisterAimPointAction(ObjectFactory* _factory)
{
    auto aInputPool = _factory->GetActorInputPool();
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInputPool->insert(std::make_pair(
        FUNC_NAME(AimPointInput), AimPointInput));
    aInitPool->insert(std::make_pair(
        FUNC_NAME(AimPointInit), AimPointInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(AimPointUpdate), AimPointUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(AimPointDestory), AimPointDestory));
}

void AimPointInput(AInputComponent* _aic, float _deltatime)
{
    if (GetBowSceneCanUpdate())
    {
        if (GetControllerPress(NpadButton::ZL::Index))
        {
            g_AimSpdFactor = 1.f;
        }
        else
        {
            g_AimSpdFactor = 10.f;
        }

        switch (GetPlayMode())
        {
        case PLAYMODE::JOYCON:
            JoyconAimPointInput(_aic, _deltatime);
            break;

        case PLAYMODE::BUTTON:
            ButtonAimPointInput(_aic, _deltatime);
            break;

        default:
            break;
        }
    }
}

void AimPointInit(AInteractionComponent* _aitc)
{
    std::string name = "";
    for (int i = 0; i < 3; i++)
    {
        g_ArrowCanUse[i] = true;
        g_AimPos[i] = MakeFloat3(0.f, 0.f, 0.f);
        g_Speed[i] = 0.f;
        name = "star-" + std::to_string(i + 1) + "-actor";
        g_Star[i] = _aitc->GetActorObjOwner()->GetSceneNodePtr()->
            GetActorObject(name);
    }

    g_BowHp = 100;
    g_BowHpMax = 100;
    g_BowHpBar = _aitc->GetActorObjOwner()->GetSceneNodePtr()->
        GetUiObject("bow-hp-ui");
}

void ShowHowManyArrowCanUse(AInteractionComponent* _aitc)
{
    for (int i = 0; i < 3; i++)
    {
        std::string objName =
            "arrow-" + std::to_string(i + 1) + "-ui";
        std::string compName = objName + "-sprite";
        ((USpriteComponent*)(_aitc->
            GetActorObjOwner()->GetSceneNodePtr()->
            GetUiObject(objName)->GetUComponent(compName)))->
            SetOffsetColor(MakeFloat4(1.f, 1.f, 1.f, 0.3f));
    }

    unsigned int canUse = 0;
    for (int i = 0; i < 3; i++)
    {
        if (g_ArrowCanUse[i])
        {
            ++canUse;
        }
    }

    for (unsigned int i = 0; i < canUse; i++)
    {
        std::string objName =
            "arrow-" + std::to_string(i + 1) + "-ui";
        std::string compName = objName + "-sprite";
        ((USpriteComponent*)(_aitc->
            GetActorObjOwner()->GetSceneNodePtr()->
            GetUiObject(objName)->GetUComponent(compName)))->
            SetOffsetColor(MakeFloat4(1.f, 1.f, 1.f, 1.f));
    }
}

void UpdateActiveArrowStar()
{
    std::string compName = "";
    for (int i = 0; i < 3; i++)
    {
        if (!g_ArrowCanUse[i])
        {
            compName = g_Star[i]->GetObjectName() + "-transform";
            auto atc = (ATransformComponent*)(g_Star[i]->
                GetAComponent(compName));
            if (!atc)
            {
                continue;
            }
            Float3 now = atc->GetPosition();
            {
                float deltaX = now.x - g_AimPos[i].x;
                float deltaY = now.y - g_AimPos[i].y;
                if (deltaX * deltaX + deltaY * deltaY < 100.f)
                {
                    ResetActiveArrowStar(g_Star[i]);
                    continue;
                }
            }
            {
                Float3 delta = MakeFloat3(
                    g_AimPos[i].x - now.x,
                    g_AimPos[i].y - now.y,
                    g_AimPos[i].z - now.z);

                double dx = (double)delta.x;
                double dy = (double)delta.y;
                double dz = (double)delta.z;

                float factor = (float)sqrt(
                    dx * dx + dy * dy + dz * dz);
                delta.x /= factor;
                delta.y /= factor;
                delta.z /= factor;
                delta.x *= g_Speed[i];
                delta.y *= g_Speed[i];
                atc->Translate(delta);
            }
        }
    }
}

void AimPointUpdate(AInteractionComponent* _aitc,
    float _deltatime)
{
    ShowHowManyArrowCanUse(_aitc);

    UpdateActiveArrowStar();

    if (!CanReadyToShoot())
    {
        return;
    }

    static bool readyToShoot = false;
    if (!readyToShoot &&
        GetControllerTrigger(NpadButton::ZR::Index))
    {
        readyToShoot = true;
    }
    if (readyToShoot &&
        !GetControllerPress(NpadButton::ZR::Index))
    {
        readyToShoot = false;
        int index = -1;
        for (int i = 0; i < 3; i++)
        {
            if (g_ArrowCanUse[i])
            {
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            return;
        }

        PlaySE("se-choose");

        int level = 0;
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
            level = (int)((double)GetBowStrength() /
                ((double)max / (double)8));
            if (level < 0) { level = 0; }
            if (level > 7) { level = 7; }
            ++level;
        }

        if (!g_BowHp)
        {
            _aitc->GetActorObjOwner()->
                GetSceneNodePtr()->GetActorObject("bow-actor")->
                GetAComponent<AAnimateComponent>(
                    COMP_TYPE::AANIMATE)->
                ChangeAnimateTo("broke");
            SetBowSceneEnd(false);
            return;
        }

        g_BowHp -= level / 2 + 1;
        if (g_BowHp > 100)
        {
            g_BowHp = 0;
        }

        static float ratio = 1.f;
        static Float3 newPos = MakeFloat3(0.f, 0.f, 0.f);
        UTransformComponent* utc = (UTransformComponent*)
            (g_BowHpBar->GetUComponent("bow-hp-ui-transform"));
        if (!utc)
        {
            return;
        }
        utc->SetScale(MakeFloat3(1.f, 1.f, 1.f));
        ratio = (float)g_BowHp / (float)g_BowHpMax;
        if (ratio > 1.f || ratio < -1.f)
        {
            ratio = 0.f;
        }
        utc->ScaleXAsix(ratio);
        newPos = utc->GetPosition();
        ratio = 190.f * (1.f - ratio);
        newPos.x = 700.f - ratio;
        utc->SetPosition(newPos);

        Float3 bowPos = ((ATransformComponent*)(_aitc->
            GetActorObjOwner()->
            GetSceneNodePtr()->GetActorObject("bow-actor")->
            GetAComponent("bow-actor-transform")))->GetPosition();
        std::string star = g_Star[index]->GetObjectName();
        ((ATransformComponent*)(g_Star[index]->
            GetAComponent(star + "-transform")))->
            SetPosition(bowPos);
        g_Speed[index] = (float)level;
        g_AimPos[index] = ((ATransformComponent*)(_aitc->
            GetActorObjOwner()->
            GetAComponent("aim-point-actor-transform")))->
            GetPosition();
        g_ArrowCanUse[index] = false;
    }
}

void AimPointDestory(AInteractionComponent* _aitc)
{
    for (int i = 0; i < 3; i++)
    {
        g_ArrowCanUse[i] = true;
        g_AimPos[i] = MakeFloat3(0.f, 0.f, 0.f);
        g_Speed[i] = 0.f;
        g_Star[i] = nullptr;
    }
}

bool CanReadyToShoot()
{
    for (int i = 0; i < 3; i++)
    {
        if (g_ArrowCanUse[i])
        {
            return true;
        }
    }

    return false;
}

void ResetActiveArrowStar(ActorObject* _star)
{
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        if (_star == g_Star[i])
        {
            break;
        }
    }
    g_ArrowCanUse[i] = true;
    ((ATransformComponent*)(_star->GetAComponent(_star->
        GetObjectName() + "-transform")))->SetPosition(MakeFloat3(
            0.f, 2200.f, 0.f));
}
