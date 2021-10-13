#include "ManualAction.h"
#include "controller.h"
#include "sound.h"

static bool g_CanUpdate = false;
static bool g_FadeIn = true;
static bool g_FadeOut = false;

static UiObject* g_LoadFades[2] = { nullptr,nullptr };

static bool g_LoadNextFlg = false;

void RegisterManualAction(ObjectFactory* _factory)
{
    auto uInitPool = _factory->GetUiInterInitPool();
    auto uUpdatePool = _factory->GetUiInterUpdatePool();
    auto uDestoryPool = _factory->GetUiInterDestoryPool();
    auto uInputPool = _factory->GetUiInputPool();

    uInitPool->insert(std::make_pair(
        FUNC_NAME(ManualLoadFadeInit), ManualLoadFadeInit));
    uUpdatePool->insert(std::make_pair(
        FUNC_NAME(ManualLoadFadeUpdate), ManualLoadFadeUpdate));
    uDestoryPool->insert(std::make_pair(
        FUNC_NAME(ManualLoadFadeDestory), ManualLoadFadeDestory));
    uInputPool->insert(std::make_pair(
        FUNC_NAME(ManualUiInput), ManualUiInput));
}

void ManualLoadFadeInit(UInteractionComponent* _uitc)
{
    g_CanUpdate = false;
    g_FadeIn = true;
    g_FadeOut = false;
    g_LoadNextFlg = false;
    g_LoadFades[0] = _uitc->GetUiObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-left-ui");
    g_LoadFades[1] = _uitc->GetUiObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-right-ui");
}

void ManualLoadFadeUpdate(UInteractionComponent* _uitc,
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

                if (g_LoadNextFlg)
                {
                    _uitc->GetUiObjOwner()->
                        GetSceneNodePtr()->
                        GetSceneManagerPtr()->
                        LoadSceneNode("title-scene",
                            "rom:/Configs/Scenes/title-scene.json");
                }
            }
        }
    }
}

void ManualLoadFadeDestory(UInteractionComponent* _uitc)
{

}

void ManualUiInput(UInputComponent* _uic, float _deltatime)
{
    if (!g_CanUpdate)
    {
        return;
    }

    if (GetControllerTrigger(NpadButton::A::Index))
    {
        PlaySE("se-choose");
        SetManualSceneEnd();
    }
}

void SetManualSceneEnd()
{
    g_CanUpdate = false;
    g_LoadNextFlg = true;
    g_FadeOut = true;
}
