#include "BowLoadingFade.h"
#include "sound.h"

static bool g_CanUpdate = false;
static bool g_FadeIn = true;
static bool g_FadeOut = false;

static UiObject* g_LoadFades[2] = { nullptr,nullptr };

static bool g_LoadNextFlg = false;

static bool g_BowSceneSuccess = false;

void RegisterBowLoadingFade(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(BowLoadFadeInit), BowLoadFadeInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(BowLoadFadeUpdate), BowLoadFadeUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(BowLoadFadeDestory), BowLoadFadeDestory));
}

void BowLoadFadeInit(AInteractionComponent* _aitc)
{
    g_CanUpdate = false;
    g_FadeIn = true;
    g_FadeOut = false;
    g_LoadNextFlg = false;
    g_BowSceneSuccess = false;
    g_LoadFades[0] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-left-ui");
    g_LoadFades[1] = _aitc->GetActorObjOwner()->
        GetSceneNodePtr()->GetUiObject("loadfade-right-ui");
}

void BowLoadFadeUpdate(AInteractionComponent* _aitc,
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
                StopBGM("bgm-success");
                StopBGM("bgm-fail");
                PlayBGM("bgm-bow");
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
                    _aitc->GetActorObjOwner()->GetSceneNodePtr()->
                        GetSceneManagerPtr()->LoadSceneNode(
                            "result-scene",
                            "rom:/Configs/Scenes/result-scene.json");
                }
            }
        }
    }
}

void BowLoadFadeDestory(AInteractionComponent* _aitc)
{

}

bool GetBowSceneCanUpdate()
{
    return g_CanUpdate;
}

void SetBowSceneEnd(bool _success)
{
    g_CanUpdate = false;
    g_LoadNextFlg = true;
    g_FadeOut = true;
    g_BowSceneSuccess = _success;
}

bool GetBowSceneIsSuccess()
{
    return g_BowSceneSuccess;
}
