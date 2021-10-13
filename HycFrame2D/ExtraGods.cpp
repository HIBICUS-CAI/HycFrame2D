#include "ExtraGods.h"
#include <stdlib.h>
#include <time.h>

static ATransformComponent* g_Icon_Trans[4] =
{
    nullptr, nullptr, nullptr, nullptr
};
static ATransformComponent* g_Name_Trans[4] =
{
    nullptr, nullptr, nullptr, nullptr
};
static ATransformComponent* g_Player_Trans[2] =
{
    nullptr, nullptr
};

static unsigned int g_Left = 0;
static unsigned int g_Right = 0;

void RegisterExtraGodsManager(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(GodsInit), GodsInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(GodsUpdate), GodsUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(GodsDestory), GodsDestory));
}

void GodsInit(AInteractionComponent* _aitc)
{
    std::string name = "";
    std::string comp = "";

    name = "houou-actor";
    comp = name + "-transform";
    g_Icon_Trans[0] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));
    name = "houou-name-actor";
    comp = name + "-transform";
    g_Name_Trans[0] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));

    name = "ra-actor";
    comp = name + "-transform";
    g_Icon_Trans[1] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));
    name = "ra-name-actor";
    comp = name + "-transform";
    g_Name_Trans[1] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));

    name = "heriosu-actor";
    comp = name + "-transform";
    g_Icon_Trans[2] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));
    name = "heriosu-name-actor";
    comp = name + "-transform";
    g_Name_Trans[2] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));

    name = "amatera-actor";
    comp = name + "-transform";
    g_Icon_Trans[3] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));
    name = "amatera-name-actor";
    comp = name + "-transform";
    g_Name_Trans[3] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));

    name = "player-1-actor";
    comp = name + "-transform";
    g_Player_Trans[0] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));
    name = "player-2-actor";
    comp = name + "-transform";
    g_Player_Trans[1] = (ATransformComponent*)(_aitc->
        GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject(name)->GetAComponent(comp));

    srand((unsigned int)time(NULL) + rand());
    g_Left = rand() % 4;
    srand((unsigned int)time(NULL) + rand());
    g_Right = rand() % 4;
    while (g_Left == g_Right)
    {
        srand((unsigned int)time(NULL) + rand());
        g_Right = rand() % 4;
    }

    g_Name_Trans[g_Left]->SetPosition(
        MakeFloat3(-900.f, 0.f, 0.f));
    g_Name_Trans[g_Left]->SetRotation(
        MakeFloat3(0.f, 0.f, 90.f));
    g_Name_Trans[g_Right]->SetPosition(
        MakeFloat3(900.f, 0.f, 0.f));
    g_Name_Trans[g_Right]->SetRotation(
        MakeFloat3(0.f, 0.f, -90.f));

    g_Icon_Trans[g_Left]->SetRotation(
        MakeFloat3(0.f, 0.f, 90.f));
    g_Icon_Trans[g_Right]->SetRotation(
        MakeFloat3(0.f, 0.f, -90.f));
}

void GodsUpdate(AInteractionComponent* _aitc, float _deltatime)
{
    g_Icon_Trans[g_Left]->SetPosition(
        g_Player_Trans[0]->GetPosition());
    g_Icon_Trans[g_Right]->SetPosition(
        g_Player_Trans[1]->GetPosition());
}

void GodsDestory(AInteractionComponent* _aitc)
{

}
