#include "ResultLoadingFade.h"
#include "BowLoadingFade.h"
#include "sound.h"

static bool g_CanUpdate = false;
static bool g_FadeIn = true;
static bool g_FadeOut = false;

static UiObject* g_LoadFades[2] = { nullptr,nullptr };

static bool g_LoadNextFlg = false;

static bool g_RetryBowScene = false;

void RegisterResultLoadingFade(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(ResultLoadFadeInit), ResultLoadFadeInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(ResultLoadFadeUpdate), ResultLoadFadeUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(ResultLoadFadeDestory), ResultLoadFadeDestory));
}

void ResultLoadFadeInit(AInteractionComponent* _aitc)
{
    g_CanUpdate = false;
    g_FadeIn = true;
    g_FadeOut = false;
    g_LoadNextFlg = false;
    g_RetryBowScene = false;
    g_LoadFades[0] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-left-ui");
    g_LoadFades[1] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-right-ui");

    if (GetBowSceneIsSuccess())
    {
        ((UTextComponent*)(_aitc->GetActorObjOwner()->
            GetSceneNodePtr()->GetUiObject("result-moji-ui")->
            GetUComponent("result-moji-ui-text")))->
            ChangeTextString("せいこう");
        ((ASpriteComponent*)(_aitc->GetActorObjOwner()->
            GetSceneNodePtr()->
            GetActorObject("success-bg-actor")->
            GetAComponent("success-bg-actor-sprite")))->
            SetOffsetColor(MakeFloat4(1.f, 1.f, 1.f, 1.f));
        ((ASpriteComponent*)(_aitc->GetActorObjOwner()->
            GetSceneNodePtr()->
            GetActorObject("fail-bg-actor")->
            GetAComponent("fail-bg-actor-sprite")))->
            SetOffsetColor(MakeFloat4(1.f, 1.f, 1.f, 0.f));
        StopBGM("bgm-bow");
        PlayBGM("bgm-success");
    }
    else
    {
        ((UTextComponent*)(_aitc->GetActorObjOwner()->
            GetSceneNodePtr()->GetUiObject("result-moji-ui")->
            GetUComponent("result-moji-ui-text")))->
            ChangeTextString("しっは°い");
        ((ASpriteComponent*)(_aitc->GetActorObjOwner()->
            GetSceneNodePtr()->
            GetActorObject("success-bg-actor")->
            GetAComponent("success-bg-actor-sprite")))->
            SetOffsetColor(MakeFloat4(1.f, 1.f, 1.f, 0.f));
        ((ASpriteComponent*)(_aitc->GetActorObjOwner()->
            GetSceneNodePtr()->
            GetActorObject("fail-bg-actor")->
            GetAComponent("fail-bg-actor-sprite")))->
            SetOffsetColor(MakeFloat4(1.f, 1.f, 1.f, 1.f));
        StopBGM("bgm-bow");
        PlayBGM("bgm-fail");

    }
}

void ResultLoadFadeUpdate(AInteractionComponent* _aitc,
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
                    if (g_RetryBowScene)
                    {
                        _aitc->GetActorObjOwner()->
                            GetSceneNodePtr()->
                            GetSceneManagerPtr()->
                            LoadSceneNode("bow-scene",
                                "rom:/Configs/Scenes/bow-scene.json");
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

void ResultLoadFadeDestory(AInteractionComponent* _aitc)
{

}

bool GetResultSceneCanUpdate()
{
    return g_CanUpdate;
}

void SetResultSceneEnd(bool _flag)
{
    g_CanUpdate = false;
    g_LoadNextFlg = true;
    g_FadeOut = true;
    g_RetryBowScene = _flag;
}
