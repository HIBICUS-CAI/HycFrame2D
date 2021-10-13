#include "ExtraLife.h"
#include "ExtraLoadingFade.h"
#include "controller.h"
#include "sound.h"

static bool g_ShowResult = false;

static unsigned int g_PlayersHp[2] = { 3,3 };
static USpriteComponent* g_PlayersHp_Spr[2][3] =
{
    {nullptr, nullptr, nullptr}, {nullptr, nullptr, nullptr}
};

static UTransformComponent* g_Boards_Trans[2] =
{
    nullptr, nullptr
};
static float g_Boards_SpdVec[2] = { 0.f, 0.f };

void RegisterExtraLife(ObjectFactory* _factory)
{
    auto uInitPool = _factory->GetUiInterInitPool();
    auto uUpdatePool = _factory->GetUiInterUpdatePool();
    auto uDestoryPool = _factory->GetUiInterDestoryPool();
    auto uInputPool = _factory->GetUiInputPool();

    uInitPool->insert(std::make_pair(
        FUNC_NAME(LifeUiInit), LifeUiInit));
    uUpdatePool->insert(std::make_pair(
        FUNC_NAME(LifeUiUpdate), LifeUiUpdate));
    uDestoryPool->insert(std::make_pair(
        FUNC_NAME(LifeUiDestory), LifeUiDestory));
    uInputPool->insert(std::make_pair(
        FUNC_NAME(LifeUiInput), LifeUiInput));
}

void LifeUiInit(UInteractionComponent* _uitc)
{
    g_ShowResult = false;
    g_PlayersHp[0] = 3;
    g_PlayersHp[1] = 3;

    std::string name = "";
    std::string comp = "";
    for (int i = 0; i < 3; i++)
    {
        name = "heart-left-" + std::to_string(i + 1) + "-ui";
        comp = name + "-sprite";
        g_PlayersHp_Spr[0][i] = (USpriteComponent*)(_uitc->
            GetUiObjOwner()->GetSceneNodePtr()->
            GetUiObject(name)->GetUComponent(comp));
    }
    for (int i = 0; i < 3; i++)
    {
        name = "heart-right-" + std::to_string(i + 1) + "-ui";
        comp = name + "-sprite";
        g_PlayersHp_Spr[1][i] = (USpriteComponent*)(_uitc->
            GetUiObjOwner()->GetSceneNodePtr()->
            GetUiObject(name)->GetUComponent(comp));
    }

    name = "victory-board-ui";
    comp = name + "-transform";
    g_Boards_Trans[0] = (UTransformComponent*)(_uitc->
        GetUiObjOwner()->GetSceneNodePtr()->
        GetUiObject(name)->GetUComponent(comp));
    name = "lose-board-ui";
    comp = name + "-transform";
    g_Boards_Trans[1] = (UTransformComponent*)(_uitc->
        GetUiObjOwner()->GetSceneNodePtr()->
        GetUiObject(name)->GetUComponent(comp));
}

void SetPlayersHp()
{
    static unsigned int index = 0;
    static Float4 color = MakeFloat4(1.f, 1.f, 1.f, 1.f);

    index = 0;
    color.w = 1.f;
    if (g_PlayersHp[0] > 3)
    {
        return;
    }
    for (unsigned int i = 0; i < g_PlayersHp[0]; i++)
    {
        g_PlayersHp_Spr[0][i]->SetOffsetColor(color);
        ++index;
    }
    color.w = 0.3f;
    for (unsigned int i = index; i < 3; i++)
    {
        g_PlayersHp_Spr[0][i]->SetOffsetColor(color);
    }

    index = 0;
    color.w = 1.f;
    if (g_PlayersHp[1] > 3)
    {
        return;
    }
    for (unsigned int i = 0; i < g_PlayersHp[1]; i++)
    {
        g_PlayersHp_Spr[1][i]->SetOffsetColor(color);
        ++index;
    }
    color.w = 0.3f;
    for (unsigned int i = index; i < 3; i++)
    {
        g_PlayersHp_Spr[1][i]->SetOffsetColor(color);
    }
}

void TranslateBoards()
{
    for (int i = 0; i < 2; i++)
    {
        g_Boards_Trans[i]->
            TranslateYAsix(10.f * g_Boards_SpdVec[i]);
        if ((g_Boards_Trans[i]->GetPosition().y /
            g_Boards_SpdVec[i]) > 0.f)
        {
            Float3 pos = g_Boards_Trans[i]->GetPosition();
            pos.y = 0.f;
            g_Boards_Trans[i]->SetPosition(pos);
            g_Boards_SpdVec[i] = 0.f;
        }
    }
}

void LifeUiUpdate(UInteractionComponent* _uitc,
    float _deltatime)
{
    SetPlayersHp();

    if (!g_ShowResult && !g_PlayersHp[0])
    {
        g_ShowResult = true;
        g_Boards_Trans[1]->SetPosition(
            MakeFloat3(400.f, -540.f, 0.f));
        g_Boards_Trans[1]->SetRotation(
            MakeFloat3(0.f, 0.f, -90.f));
        g_Boards_SpdVec[1] = 1.f;
        g_Boards_Trans[0]->SetPosition(
            MakeFloat3(-400.f, 540.f, 0.f));
        g_Boards_Trans[0]->SetRotation(
            MakeFloat3(0.f, 0.f, 90.f));
        g_Boards_SpdVec[0] = -1.f;
    }
    else if (!g_ShowResult && !g_PlayersHp[1])
    {
        g_ShowResult = true;
        g_Boards_Trans[0]->SetPosition(
            MakeFloat3(400.f, -540.f, 0.f));
        g_Boards_Trans[0]->SetRotation(
            MakeFloat3(0.f, 0.f, -90.f));
        g_Boards_SpdVec[0] = 1.f;
        g_Boards_Trans[1]->SetPosition(
            MakeFloat3(-400.f, 540.f, 0.f));
        g_Boards_Trans[1]->SetRotation(
            MakeFloat3(0.f, 0.f, 90.f));
        g_Boards_SpdVec[1] = -1.f;
    }

    if (g_ShowResult)
    {
        TranslateBoards();
    }
}

void LifeUiDestory(UInteractionComponent* _uitc)
{

}

void LifeUiInput(UInputComponent* _uic, float _deltatime)
{
    if (!g_ShowResult)
    {
        return;
    }

    if (GetControllerTrigger(NpadButton::Plus::Index))
    {
        PlaySE("se-choose");
        SetExtraSceneEnd(true);
    }
    else if (GetControllerTrigger(NpadButton::Minus::Index))
    {
        PlaySE("se-choose");
        SetExtraSceneEnd(false);
    }
}

void SubPlayerLeftHp()
{
    if (g_PlayersHp[0])
    {
        --g_PlayersHp[0];
    }
}

void SubPlayerRightHp()
{
    if (g_PlayersHp[1])
    {
        --g_PlayersHp[1];
    }
}
