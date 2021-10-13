#include "TitleButton.h"
#include "controller.h"
#include "sound.h"

static UiObject* g_Btns[4] =
{
    nullptr,nullptr,nullptr,nullptr
};

static ActorObject* g_Anis[4] =
{
    nullptr,nullptr,nullptr,nullptr
};

static bool g_CanUpdate = false;
static bool g_FadeIn = true;
static bool g_FadeOut = false;

static bool g_IsFirstOrAgainToCheckObj = true;

static UiObject* g_LoadFades[2] = { nullptr,nullptr };

static int g_NextScene = -1;

void RegisterTitleButton(ObjectFactory* _factory)
{
    auto uInputPool = _factory->GetUiInputPool();
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    uInputPool->insert(std::make_pair(
        FUNC_NAME(ButtonSelectInput), ButtonSelectInput));
    aInitPool->insert(std::make_pair(
        FUNC_NAME(TitleLoadFadeInit), TitleLoadFadeInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(TitleLoadFadeUpdate), TitleLoadFadeUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(TitleLoadFadeDestory), TitleLoadFadeDestory));
}

void TitleLoadFadeInit(AInteractionComponent* _aitc)
{
    g_CanUpdate = false;
    g_FadeIn = true;
    g_FadeOut = false;
    g_IsFirstOrAgainToCheckObj = true;
    g_NextScene = -1;
    g_LoadFades[0] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-left-ui");
    g_LoadFades[1] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-right-ui");
}

void TitleLoadFadeUpdate(AInteractionComponent* _aitc,
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

                StopBGM("bgm-success");
                StopBGM("bgm-fail");
                StopBGM("bgm-extra");
                PlayBGM("bgm-title");
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

                switch (g_NextScene)
                {
                case 0:
                    _aitc->GetActorObjOwner()->GetSceneNodePtr()->
                        GetSceneManagerPtr()->LoadSceneNode(
                            "mode-select-scene",
                            "rom:/Configs/Scenes/mode-select-scene.json");
                    break;

                case 1:
                    _aitc->GetActorObjOwner()->GetSceneNodePtr()->
                        GetSceneManagerPtr()->LoadSceneNode(
                            "manual-scene",
                            "rom:/Configs/Scenes/manual-scene.json");
                    break;

                case 2:
                    _aitc->GetActorObjOwner()->GetSceneNodePtr()->
                        GetSceneManagerPtr()->LoadSceneNode(
                            "story-scene",
                            "rom:/Configs/Scenes/story-scene.json");
                    break;

                case 3:
                    _aitc->GetActorObjOwner()->GetSceneNodePtr()->
                        GetSceneManagerPtr()->
                        SetShouldTurnOff(true);
                    break;

                default:
                    break;
                }
            }
        }
    }
}

void TitleLoadFadeDestory(AInteractionComponent* _aitc)
{

}

void ButtonSelectInput(UInputComponent* _uic, float _deltatime)
{
    if (g_IsFirstOrAgainToCheckObj)
    {
        g_IsFirstOrAgainToCheckObj = false;

        g_Btns[0] = _uic->GetUiObjOwner()->
            GetSceneNodePtr()->
            GetUiObject("start-btn-ui");
        g_Btns[1] = _uic->GetUiObjOwner()->
            GetSceneNodePtr()->
            GetUiObject("manual-btn-ui");
        g_Btns[2] = _uic->GetUiObjOwner()->
            GetSceneNodePtr()->
            GetUiObject("story-btn-ui");
        g_Btns[3] = _uic->GetUiObjOwner()->
            GetSceneNodePtr()->
            GetUiObject("quit-btn-ui");

        g_Anis[0] = _uic->GetUiObjOwner()->
            GetSceneNodePtr()->
            GetActorObject("btn-start-ani-actor");
        g_Anis[1] = _uic->GetUiObjOwner()->
            GetSceneNodePtr()->
            GetActorObject("btn-manual-ani-actor");
        g_Anis[2] = _uic->GetUiObjOwner()->
            GetSceneNodePtr()->
            GetActorObject("btn-story-ani-actor");
        g_Anis[3] = _uic->GetUiObjOwner()->
            GetSceneNodePtr()->
            GetActorObject("btn-quit-ani-actor");
    }

    if (g_CanUpdate)
    {
        static bool firstSelectedFinished = false;
        if (!firstSelectedFinished)
        {
            ((AAnimateComponent*)(_uic->GetUiObjOwner()->
                GetSceneNodePtr()->
                GetActorObject("btn-start-ani-actor")->
                GetAComponent("btn-start-ani-actor-animate")))->
                ChangeAnimateTo("selected");
            firstSelectedFinished = true;
        }

        UiObject* owner = _uic->GetUiObjOwner();

        std::string btnmapName =
            owner->GetObjectName() + "-btnmap";
        UBtnMapComponent* ubmc = nullptr;
        ubmc = (UBtnMapComponent*)owner->GetUComponent(btnmapName);

        static bool canResetAni = false;

        if (GetControllerTrigger(NpadButton::Up::Index))
        {
            if (ubmc && ubmc->IsBeingSelected())
            {
                ubmc->SelectUpBtn();
                if (ubmc->GetUpBtn())
                {
                    canResetAni = true;
                    PlaySE("se-change");
                }
            }
        }
        else if (GetControllerTrigger(NpadButton::Down::Index))
        {
            if (ubmc && ubmc->IsBeingSelected())
            {
                ubmc->SelectDownBtn();
                if (ubmc->GetDownBtn())
                {
                    canResetAni = true;
                    PlaySE("se-change");
                }
            }
        }
        else if (GetControllerTrigger(NpadButton::A::Index))
        {
            if (ubmc && ubmc->IsBeingSelected())
            {
                PlaySE("se-choose");

                for (int i = 0; i < 4; i++)
                {
                    if (ubmc->GetUiObjOwner() == g_Btns[i])
                    {
                        g_NextScene = i;
                        g_CanUpdate = false;
                        g_FadeOut = true;
                        g_IsFirstOrAgainToCheckObj = true;
                        firstSelectedFinished = false;

                        ubmc->SetIsSelected(false);
                        ((UBtnMapComponent*)(g_Btns[0]->
                            GetUComponent("start-btn-ui-btnmap")))
                            ->SetIsSelected(true);

                        break;
                    }
                }
            }
        }

        if (ubmc && ubmc->IsBeingSelected() && canResetAni)
        {
            for (int i = 0; i < 4; i++)
            {
                std::string compName = "";
                compName = g_Anis[i]->GetObjectName();
                compName += "-animate";
                ((AAnimateComponent*)(g_Anis[i]->
                    GetAComponent(compName)))->
                    ChangeAnimateTo("default");
            }
            for (int i = 0; i < 4; i++)
            {
                if (ubmc->GetUiObjOwner() == g_Btns[i])
                {
                    std::string compName = "";
                    compName = g_Anis[i]->GetObjectName();
                    compName += "-animate";
                    ((AAnimateComponent*)(g_Anis[i]->
                        GetAComponent(compName)))->
                        ChangeAnimateTo("selected");
                    break;
                }
            }
            canResetAni = false;
        }
    }
}
