#include "ModeSelectButton.h"
#include "controller.h"
#include "sound.h"

static bool g_CanUpdate = false;
static bool g_FadeIn = true;
static bool g_FadeOut = false;

static UiObject* g_LoadFades[2] = { nullptr,nullptr };

static bool g_LoadNextFlg = false;
const static unsigned int NRMAL_MODE = 1;
const static unsigned int EXTRA_MODE = 2;
static unsigned int g_NextID = 0;

static PLAYMODE g_PlayMode = PLAYMODE::NONE;
static DIFFICULTY g_DiffiMode = DIFFICULTY::NONE;

static ASpriteComponent* g_Intros_Spr[3] =
{
    nullptr, nullptr, nullptr
};

void RegisterModeSelectButton(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();
    auto uInputPool = _factory->GetUiInputPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(SelectLoadFadeInit), SelectLoadFadeInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(SelectLoadFadeUpdate), SelectLoadFadeUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(SelectLoadFadeDestory), SelectLoadFadeDestory));
    uInputPool->insert(std::make_pair(
        FUNC_NAME(ModeSelectInput), ModeSelectInput));
}

void SelectLoadFadeInit(AInteractionComponent* _aitc)
{
    g_CanUpdate = false;
    g_FadeIn = true;
    g_FadeOut = false;
    g_LoadNextFlg = false;
    g_NextID = 0;
    g_PlayMode = PLAYMODE::NONE;
    g_DiffiMode = DIFFICULTY::NONE;
    g_LoadFades[0] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-left-ui");
    g_LoadFades[1] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-right-ui");
    g_Intros_Spr[0] = ((ASpriteComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject("joycon-intro-actor")->
        GetAComponent("joycon-intro-actor-sprite")));
    g_Intros_Spr[1] = ((ASpriteComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject("button-intro-actor")->
        GetAComponent("button-intro-actor-sprite")));
    g_Intros_Spr[2] = ((ASpriteComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject("extra-intro-actor")->
        GetAComponent("extra-intro-actor-sprite")));
    for (int i = 0; i < 3; i++)
    {
        g_Intros_Spr[i]->SetOffsetColor(
            MakeFloat4(1.f, 1.f, 1.f, 0.f));
    }
}

void SelectLoadFadeUpdate(AInteractionComponent* _aitc,
    float _deltatime)
{
    if (!g_CanUpdate)
    {
        if (g_FadeIn)
        {
            ((UTransformComponent*)(g_LoadFades[0]->
                GetUComponent("loadfade-left-ui-transform")))->
                TranslateXAsix(-15.f);
            ((UTransformComponent*)(g_LoadFades[1]->
                GetUComponent("loadfade-right-ui-transform")))->
                TranslateXAsix(15.f);

            if (((UTransformComponent*)(g_LoadFades[0]->
                GetUComponent("loadfade-left-ui-transform")))->
                GetPosition().x <= -1280.f)
            {
                ((UTransformComponent*)(g_LoadFades[0]->
                    GetUComponent(
                        "loadfade-left-ui-transform")))->
                    SetPosition(MakeFloat3(-1280.f, 0.f, 0.f));
                ((UTransformComponent*)(g_LoadFades[1]->
                    GetUComponent(
                        "loadfade-right-ui-transform")))->
                    SetPosition(MakeFloat3(1280.f, 0.f, 0.f));

                ((USpriteComponent*)(g_LoadFades[0]->
                    GetUComponent(
                        "loadfade-left-ui-sprite")))->
                    SetOffsetColor(
                        MakeFloat4(1.f, 1.f, 1.f, 0.f));
                ((USpriteComponent*)(g_LoadFades[1]->
                    GetUComponent(
                        "loadfade-right-ui-sprite")))->
                    SetOffsetColor(
                        MakeFloat4(1.f, 1.f, 1.f, 0.f));

                g_FadeIn = false;
                g_CanUpdate = true;
            }
        }
        else if (g_FadeOut)
        {
            ((USpriteComponent*)(g_LoadFades[0]->
                GetUComponent(
                    "loadfade-left-ui-sprite")))->
                SetOffsetColor(
                    MakeFloat4(1.f, 1.f, 1.f, 1.f));
            ((USpriteComponent*)(g_LoadFades[1]->
                GetUComponent(
                    "loadfade-right-ui-sprite")))->
                SetOffsetColor(
                    MakeFloat4(1.f, 1.f, 1.f, 1.f));
            ((UTransformComponent*)(g_LoadFades[0]->
                GetUComponent("loadfade-left-ui-transform")))->
                TranslateXAsix(15.f);
            ((UTransformComponent*)(g_LoadFades[1]->
                GetUComponent("loadfade-right-ui-transform")))->
                TranslateXAsix(-15.f);

            if (((UTransformComponent*)(g_LoadFades[0]->
                GetUComponent("loadfade-left-ui-transform")))->
                GetPosition().x >= 0.f)
            {
                ((UTransformComponent*)(g_LoadFades[0]->
                    GetUComponent(
                        "loadfade-left-ui-transform")))->
                    SetPosition(MakeFloat3(0.f, 0.f, 0.f));
                ((UTransformComponent*)(g_LoadFades[1]->
                    GetUComponent(
                        "loadfade-right-ui-transform")))->
                    SetPosition(MakeFloat3(0.f, 0.f, 0.f));

                g_FadeOut = false;

                if (g_LoadNextFlg && (g_NextID == NRMAL_MODE))
                {
                    _aitc->GetActorObjOwner()->GetSceneNodePtr()->
                        GetSceneManagerPtr()->LoadSceneNode(
                            "bow-scene",
                            "rom:/Configs/Scenes/bow-scene.json");
                }
                else if (g_LoadNextFlg && (g_NextID == EXTRA_MODE))
                {
                    _aitc->GetActorObjOwner()->GetSceneNodePtr()->
                        GetSceneManagerPtr()->LoadSceneNode(
                            "extra-scene",
                            "rom:/Configs/Scenes/extra-scene.json");
                }
            }
        }
    }
}

void SelectLoadFadeDestory(AInteractionComponent* _aitc)
{

}

void ModeSelectInput(UInputComponent* _uic, float _deltatime)
{
    UiObject* owner = _uic->GetUiObjOwner();

    std::string btnmapName =
        owner->GetObjectName() + "-btnmap";
    UBtnMapComponent* ubmc = nullptr;
    ubmc = (UBtnMapComponent*)owner->GetUComponent(btnmapName);

    if (g_CanUpdate && ubmc && ubmc->IsBeingSelected())
    {
        if (owner->GetObjectName() ==
            "joycon-mode-btn-ui")
        {
            for (int i = 0; i < 3; i++)
            {
                g_Intros_Spr[i]->SetOffsetColor(
                    MakeFloat4(1.f, 1.f, 1.f, 0.f));
            }
            g_Intros_Spr[0]->SetOffsetColor(
                MakeFloat4(1.f, 1.f, 1.f, 1.f));
        }
        else if (owner->GetObjectName() ==
            "button-mode-btn-ui")
        {
            for (int i = 0; i < 3; i++)
            {
                g_Intros_Spr[i]->SetOffsetColor(
                    MakeFloat4(1.f, 1.f, 1.f, 0.f));
            }
            g_Intros_Spr[1]->SetOffsetColor(
                MakeFloat4(1.f, 1.f, 1.f, 1.f));
        }
        else if (owner->GetObjectName() ==
            "extra-mode-btn-ui")
        {
            for (int i = 0; i < 3; i++)
            {
                g_Intros_Spr[i]->SetOffsetColor(
                    MakeFloat4(1.f, 1.f, 1.f, 0.f));
            }
            g_Intros_Spr[2]->SetOffsetColor(
                MakeFloat4(1.f, 1.f, 1.f, 1.f));
        }
    }

    if (GetControllerTrigger(NpadButton::Left::Index))
    {
        if (g_CanUpdate && ubmc && ubmc->IsBeingSelected())
        {
            PlaySE("se-change");
            ubmc->SelectLeftBtn();
        }
    }
    else if (GetControllerTrigger(NpadButton::Right::Index))
    {
        if (g_CanUpdate && ubmc && ubmc->IsBeingSelected())
        {
            PlaySE("se-change");
            ubmc->SelectRightBtn();
        }
    }
    else if (GetControllerTrigger(NpadButton::A::Index))
    {
        if (g_CanUpdate && ubmc && ubmc->IsBeingSelected())
        {
            PlaySE("se-choose");
            if (owner->GetObjectName() ==
                "joycon-mode-btn-ui")
            {
                /*SetPlayMode(PLAYMODE::JOYCON);
                g_NextID = NRMAL_MODE;
                ubmc->SelectDownBtn();*/
            }
            else if (owner->GetObjectName() ==
                "button-mode-btn-ui")
            {
                SetPlayMode(PLAYMODE::BUTTON);
                g_NextID = NRMAL_MODE;
                ubmc->SelectDownBtn();
            }
            else if (owner->GetObjectName() ==
                "extra-mode-btn-ui")
            {
                SetPlayMode(PLAYMODE::NONE);
                g_NextID = EXTRA_MODE;

                g_CanUpdate = false;
                g_LoadNextFlg = true;
                g_FadeOut = true;
            }
            else if (owner->GetObjectName() ==
                "easy-mode-btn-ui")
            {
                SetDifficulty(DIFFICULTY::EASY);

                g_CanUpdate = false;
                g_LoadNextFlg = true;
                g_FadeOut = true;
            }
            else if (owner->GetObjectName() ==
                "normal-mode-btn-ui")
            {
                SetDifficulty(DIFFICULTY::NORMAL);

                g_CanUpdate = false;
                g_LoadNextFlg = true;
                g_FadeOut = true;
            }
            else if (owner->GetObjectName() ==
                "hard-mode-btn-ui")
            {
                SetDifficulty(DIFFICULTY::HARD);

                g_CanUpdate = false;
                g_LoadNextFlg = true;
                g_FadeOut = true;
            }
        }
    }
}

void SetPlayMode(PLAYMODE _mode)
{
    g_PlayMode = _mode;
}

PLAYMODE GetPlayMode()
{
    return g_PlayMode;
}

void ResetPlayMode()
{
    g_PlayMode = PLAYMODE::NONE;
}

void SetDifficulty(DIFFICULTY _mode)
{
    g_DiffiMode = _mode;
}

DIFFICULTY GetDifficulty()
{
    return g_DiffiMode;
}

void ResetDifficulty()
{
    g_DiffiMode = DIFFICULTY::NONE;
}
