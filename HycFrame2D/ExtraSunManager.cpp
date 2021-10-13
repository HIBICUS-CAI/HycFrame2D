#include "ExtraSunManager.h"
#include "ExtraLoadingFade.h"
#include "ExtraPlayerInput.h"
#include "ExtraLife.h"
#include <stdlib.h>
#include <time.h>

static ActorObject* g_Players[2] = { nullptr,nullptr };

static ActorObject* g_Suns[10] =
{
    nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr
};
static ATransformComponent* g_Suns_Trans[10] =
{
    nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr
};
static ACollisionComponent* g_Suns_Colli[10] =
{
    nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr
};

static ActorObject* g_Suns_CarryBy[10] =
{
    nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr
};

static Float3 g_Suns_SpdVec[10] =
{
    MakeFloat3(0.f, 0.f, 0.f), MakeFloat3(0.f, 0.f, 0.f),
    MakeFloat3(0.f, 0.f, 0.f), MakeFloat3(0.f, 0.f, 0.f),
    MakeFloat3(0.f, 0.f, 0.f), MakeFloat3(0.f, 0.f, 0.f),
    MakeFloat3(0.f, 0.f, 0.f), MakeFloat3(0.f, 0.f, 0.f),
    MakeFloat3(0.f, 0.f, 0.f), MakeFloat3(0.f, 0.f, 0.f)
};
static bool g_Suns_SpdFlg[10] =
{
    false,false,false,false,false,false,false,false,false,false
};

void RegisterExtraSunsManager(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(ExtraSunsInit), ExtraSunsInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(ExtraSunsUpdate), ExtraSunsUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(ExtraSunsDestory), ExtraSunsDestory));
}

void ExtraSunsInit(AInteractionComponent* _aitc)
{
    g_Players[0] = _aitc->GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject("player-1-actor");
    g_Players[1] = _aitc->GetActorObjOwner()->GetSceneNodePtr()->
        GetActorObject("player-2-actor");

    std::string name = "";
    std::string comp = "";
    int basic = 0;
    int range = 0;
    Float3 pos = MakeFloat3(0.f, 0.f, 0.f);

    for (int i = 0; i < 10; i++)
    {
        name = "sun-" + std::to_string(i + 1) + "-actor";
        g_Suns[i] = _aitc->GetActorObjOwner()->GetSceneNodePtr()->
            GetActorObject(name);

        comp = name + "-transform";
        g_Suns_Trans[i] = (ATransformComponent*)(g_Suns[i]->
            GetAComponent(comp));
        basic = -700;
        range = 1400;
        srand((unsigned int)time(NULL) + rand());
        pos.x = (float)(rand() % range + basic);
        basic = -300;
        range = 600;
        srand((unsigned int)time(NULL) + rand());
        pos.y = (float)(rand() % range + basic);
        g_Suns_Trans[i]->SetPosition(pos);

        comp = name + "-collision";
        g_Suns_Colli[i] = (ACollisionComponent*)(g_Suns[i]->
            GetAComponent(comp));

        g_Suns_CarryBy[i] = nullptr;
    }
}

void ExtraSunsUpdate(AInteractionComponent* _aitc,
    float _deltatime)
{
    if (!GetExtraSceneCanUpdate())
    {
        return;
    }

    if (GetReadyPickArray()[0])
    {
        for (int i = 0; i < 10; i++)
        {
            if (g_Suns_Colli[i]->CheckCollisionWith(g_Players[0]))
            {
                int count = 0;
                for (int j = 0; j < 10; j++)
                {
                    if (g_Suns_CarryBy[j] == g_Players[0])
                    {
                        ++count;
                        break;
                    }
                }
                if (!count)
                {
                    g_Suns_CarryBy[i] = g_Players[0];
                }
                break;
            }
        }
    }
    if (GetReadyPickArray()[1])
    {
        for (int i = 0; i < 10; i++)
        {
            if (g_Suns_Colli[i]->CheckCollisionWith(g_Players[1]))
            {
                int count = 0;
                for (int j = 0; j < 10; j++)
                {
                    if (g_Suns_CarryBy[j] == g_Players[1])
                    {
                        ++count;
                        break;
                    }
                }
                if (!count)
                {
                    g_Suns_CarryBy[i] = g_Players[1];
                }
                break;
            }
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (g_Suns_CarryBy[i] && !g_Suns_SpdFlg[i])
        {
            g_Suns_Trans[i]->SetPosition(
                ((ATransformComponent*)g_Suns_CarryBy[i]->
                    GetAComponent(g_Suns_CarryBy[i]->
                        GetObjectName() + "-transform"))->
                GetPosition());
        }

        if (g_Suns_SpdFlg[i])
        {
            static ActorObject* another = nullptr;
            another = g_Suns_CarryBy[i];
            if (another == g_Players[0])
            {
                if (g_Suns_Colli[i]->
                    CheckCollisionWith(g_Players[1]))
                {
                    SubPlayerRightHp();
                    g_Suns_SpdFlg[i] = false;
                    g_Suns_SpdVec[i].x = 0.f;
                    g_Suns_SpdVec[i].y = 0.f;
                    g_Suns_CarryBy[i] = nullptr;
                }
            }
            else if (another == g_Players[1])
            {
                if (g_Suns_Colli[i]->
                    CheckCollisionWith(g_Players[0]))
                {
                    SubPlayerLeftHp();
                    g_Suns_SpdFlg[i] = false;
                    g_Suns_SpdVec[i].x = 0.f;
                    g_Suns_SpdVec[i].y = 0.f;
                    g_Suns_CarryBy[i] = nullptr;
                }
            }

            g_Suns_Trans[i]->Translate(g_Suns_SpdVec[i]);

            static Float3 pos = MakeFloat3(0.f, 0.f, 0.f);
            pos = g_Suns_Trans[i]->GetPosition();
            bool flag = false;
            if (pos.x < -650.f)
            {
                pos.x = -650.f;
                flag = true;
            }
            else if (pos.x > 650.f)
            {
                pos.x = 650.f;
                flag = true;
            }
            if (pos.y < -250.f)
            {
                pos.y = -250.f;
                flag = true;
            }
            else if (pos.y > 250.f)
            {
                pos.y = 250.f;
                flag = true;
            }
            if (flag)
            {
                g_Suns_SpdFlg[i] = false;
                g_Suns_SpdVec[i].x = 0.f;
                g_Suns_SpdVec[i].y = 0.f;
                g_Suns_CarryBy[i] = nullptr;

                continue;
            }

            g_Suns_SpdVec[i].x = (g_Suns_SpdVec[i].x < 0.f) ?
                g_Suns_SpdVec[i].x + 0.05f :
                g_Suns_SpdVec[i].x - 0.05f;
            g_Suns_SpdVec[i].y = (g_Suns_SpdVec[i].y < 0.f) ?
                g_Suns_SpdVec[i].y + 0.05f :
                g_Suns_SpdVec[i].y - 0.05f;
            if (g_Suns_SpdVec[i].x * g_Suns_SpdVec[i].x < 0.1f ||
                g_Suns_SpdVec[i].y * g_Suns_SpdVec[i].y < 0.1f)
            {
                g_Suns_SpdFlg[i] = false;
                g_Suns_SpdVec[i].x = 0.f;
                g_Suns_SpdVec[i].y = 0.f;
                g_Suns_CarryBy[i] = nullptr;
            }
        }
    }
}

void ExtraSunsDestory(AInteractionComponent* _aitc)
{

}

void SetSunShoot(ActorObject* _player, Float3 _spdVec)
{
    if ((_spdVec.x > -5.f && _spdVec.x < 5.f) &&
        (_spdVec.y > -5.f && _spdVec.y < 5.f))
    {
        return;
    }

    for (int i = 0; i < 10; i++)
    {
        if (g_Suns_CarryBy[i] == _player)
        {
            g_Suns_SpdFlg[i] = true;
            g_Suns_SpdVec[i] = _spdVec;
            break;
        }
    }
}
