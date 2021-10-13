#include "ExtraLoadingFade.h"
#include "Ui_all.h"
#include "sound.h"

static bool g_CanUpdate = false;
static bool g_FadeIn = true;
static bool g_FadeOut = false;

static UiObject* g_LoadFades[2] = { nullptr,nullptr };

static bool g_LoadNextFlg = false;

static bool g_RetryExtraScene = false;

void RegisterExtraLoadingFade(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(ExtraLoadFadeInit), ExtraLoadFadeInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(ExtraLoadFadeUpdate), ExtraLoadFadeUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(ExtraLoadFadeDestory), ExtraLoadFadeDestory));
}

void ExtraLoadFadeInit(AInteractionComponent* _aitc)
{
    g_CanUpdate = false;
    g_FadeIn = true;
    g_FadeOut = false;
    g_LoadNextFlg = false;
    g_RetryExtraScene = false;
    g_LoadFades[0] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-left-ui");
    g_LoadFades[1] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-right-ui");
}

void ExtraLoadFadeUpdate(AInteractionComponent* _aitc,
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

                StopBGM("bgm-title");
                StopBGM("bgm-extra");
                PlayBGM("bgm-extra");
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
                    if (g_RetryExtraScene)
                    {
                        _aitc->GetActorObjOwner()->
                            GetSceneNodePtr()->
                            GetSceneManagerPtr()->
                            LoadSceneNode("extra-scene",
                                "rom:/Configs/Scenes/extra-scene.json");
                    }
                    else
                    {
                        _aitc->GetActorObjOwner()->
                            GetSceneNodePtr()->
                            GetSceneManagerPtr()->
                            LoadSceneNode("title-scene",
                                "rom:/Configs/Scenes/title-scene.json");
                    }
                }
            }
        }
    }
}

void ExtraLoadFadeDestory(AInteractionComponent* _aitc)
{

}

bool GetExtraSceneCanUpdate()
{
    return g_CanUpdate;
}

void SetExtraSceneEnd(bool _flag)
{
    g_CanUpdate = false;
    g_LoadNextFlg = true;
    g_FadeOut = true;
    g_RetryExtraScene = _flag;
}
