#include "SunsManager.h"
#include "BowLoadingFade.h"
#include "ModeSelectButton.h"
#include "AimPointAction.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "sound.h"

static std::vector<unsigned int> g_SunIndex = {};
static std::vector<ACollisionComponent*> g_Suns_Colli = {};
static std::vector<ATransformComponent*> g_Smal_Trans = {};
static std::vector<Float3> g_Suns_Speed = {};
static ActorObject* g_ArrowStars[3] =
{
    nullptr, nullptr, nullptr
};
static ActorObject* g_Explos[3] =
{
    nullptr, nullptr, nullptr
};

void SetExploTo(Float3 _pos)
{
    PlaySE("se-boom");

    static unsigned int index = 0;
    static std::string compName = "";
    index = (index + 1) % 3;
    compName = g_Explos[index]->GetObjectName() + "-transform";
    ((ATransformComponent*)g_Explos[index]->
        GetAComponent(compName))->SetPosition(_pos);
    compName = g_Explos[index]->GetObjectName() + "-animate";
    ((AAnimateComponent*)g_Explos[index]->
        GetAComponent(compName))->ChangeAnimateTo("explosion");
}

void RegisterSunsManager(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(SunsInit), SunsInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(SunsUpdate), SunsUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(SunsDestory), SunsDestory));
}

void SunsInit(AInteractionComponent* _aitc)
{
    g_SunIndex.clear();
    g_Suns_Colli.clear();
    g_Smal_Trans.clear();
    g_Suns_Speed.clear();

    {
        int basic = 0;
        int range = 0;
        DIFFICULTY diffi = GetDifficulty();
        switch (diffi)
        {
        case DIFFICULTY::EASY:
            basic = 3;
            range = 3;
            break;
        case DIFFICULTY::NORMAL:
            basic = 5;
            range = 7;
            break;
        case DIFFICULTY::HARD:
            basic = 8;
            range = 12;
            break;
        default:
            return;
        }

        srand((unsigned int)time(NULL) + rand());
        int sunCount = rand() % range + basic;
        for (int i = 0; i < sunCount; i++)
        {
            g_SunIndex.push_back((unsigned int)i + 1);
        }
    }

    std::string name = "";
    std::string comp = "";
    Float3 value = MakeFloat3(0.f, 0.f, 0.f);

    for (auto& index : g_SunIndex)
    {
        name = "small-sun-" + std::to_string(index) + "-actor";
        comp = name + "-transform";
        ATransformComponent* atc = (ATransformComponent*)(_aitc->
            GetActorObjOwner()->GetSceneNodePtr()->
            GetActorObject(name)->GetAComponent(comp));
        g_Smal_Trans.push_back(atc);

        name = "sun-" + std::to_string(index) + "-actor";
        comp = name + "-collision";
        ACollisionComponent* acc = (ACollisionComponent*)(_aitc->
            GetActorObjOwner()->GetSceneNodePtr()->
            GetActorObject(name)->GetAComponent(comp));
        g_Suns_Colli.push_back(acc);

        srand((unsigned int)time(NULL) + rand());
        int startX = rand() % 3840 - 1920;
        value = MakeFloat3((float)startX, 540.f, 0.f);
        comp = name + "-transform";
        ((ATransformComponent*)(_aitc->GetActorObjOwner()->
            GetSceneNodePtr()->GetActorObject(name)->
            GetAComponent(comp)))->SetPosition(value);

        srand((unsigned int)time(NULL) + rand());
        int deltaX = (rand() % 3840 - 1920) - startX;
        float speedX = ((float)deltaX) /
            sqrtf((float)(deltaX * deltaX + 7290000));
        float speedY = ((float)-2700) /
            sqrtf((float)(deltaX * deltaX + 7290000));
        DIFFICULTY diffi = GetDifficulty();
        int basic = 0;
        int range = 0;
        switch (diffi)
        {
        case DIFFICULTY::EASY:
            basic = 1;
            range = 2;
            break;
        case DIFFICULTY::NORMAL:
            basic = 1;
            range = 3;
            break;
        case DIFFICULTY::HARD:
            basic = 3;
            range = 3;
            break;
        default:
            break;
        }
        srand((unsigned int)time(NULL) + rand());
        int speedFactor = rand() % range + basic;
        value.x = speedX * (float)speedFactor;
        value.y = speedY * (float)speedFactor;
        g_Suns_Speed.push_back(value);
    }

    for (int i = 0; i < 3; i++)
    {
        name = "star-" + std::to_string(i + 1) + "-actor";
        g_ArrowStars[i] = _aitc->GetActorObjOwner()->
            GetSceneNodePtr()->GetActorObject(name);

        name = "sun-explo-" + std::to_string(i + 1) + "-actor";
        g_Explos[i] = _aitc->GetActorObjOwner()->
            GetSceneNodePtr()->GetActorObject(name);
    }
}

void SunsUpdate(AInteractionComponent* _aitc, float _deltatime)
{
    if (!GetBowSceneCanUpdate())
    {
        return;
    }

    {
        bool isFinished = true;
        for (auto& acc : g_Suns_Colli)
        {
            if (acc->IsCompActive() == STATUS::ACTIVE)
            {
                isFinished = false;
                break;
            }
        }
        if (isFinished)
        {
            SetBowSceneEnd(true);
            return;
        }
    }

    int sunSize = (int)g_SunIndex.size();
    for (int i = 0; i < sunSize; i++)
    {
        if (g_Suns_Colli[i]->IsCompActive() == STATUS::PAUSE)
        {
            continue;
        }

        static std::string transName = "";
        transName = g_Suns_Colli[i]->GetActorObjOwner()->
            GetObjectName() + "-transform";
        static Float3 sunPos = MakeFloat3(0.f, 0.f, 0.f);
        sunPos = ((ATransformComponent*)g_Suns_Colli[i]->
            GetActorObjOwner()->GetAComponent(transName))->
            GetPosition();
        static Float2 camPos = MakeFloat2(0.f, 0.f);
        camPos = g_Suns_Colli[i]->GetActorObjOwner()->
            GetSceneNodePtr()->GetCamera()->GetCameraPosition();
        static float deltaX = 0.f;
        static float deltaY = 0.f;
        static float vPosY = 0.f;
        static float hPosX = 0.f;
        deltaX = sunPos.x - camPos.x;
        deltaY = sunPos.y - camPos.y;
        vPosY = deltaY * 960.f / deltaX;
        hPosX = deltaX * 540.f / deltaY;
        vPosY = (vPosY > 0.f) ? vPosY : -vPosY;
        hPosX = (hPosX > 0.f) ? hPosX : -hPosX;
        if (deltaX * deltaX > 960.f * 960.f ||
            deltaY * deltaY > 540.f * 540.f)
        {
            if (vPosY > 540.f)
            {
                if (deltaX < 0.f)
                {
                    hPosX = -hPosX;
                }
                if (deltaY < 0.f)
                {
                    vPosY = -540.f;
                }
                else
                {
                    vPosY = 540.f;
                }
                hPosX += camPos.x;
                vPosY += camPos.y;
                g_Smal_Trans[i]->SetPosition(MakeFloat3(
                    hPosX, vPosY, 0.f));
            }
            else
            {
                if (deltaX < 0.f)
                {
                    hPosX = -960.f;
                }
                else
                {
                    hPosX = 960.f;
                }
                if (deltaY < 0.f)
                {
                    vPosY = -vPosY;
                }
                hPosX += camPos.x;
                vPosY += camPos.y;
                g_Smal_Trans[i]->SetPosition(MakeFloat3(
                    hPosX, vPosY, 0.f));
            }
        }
        else
        {
            g_Smal_Trans[i]->SetPosition(MakeFloat3(
                0.f, 2200.f, 0.f));
        }

        for (int j = 0; j < 3; j++)
        {
            if (g_Suns_Colli[i]->
                CheckCollisionWith(g_ArrowStars[j]))
            {
                ResetActiveArrowStar(g_ArrowStars[j]);
                g_Suns_Colli[i]->SetCompActive(STATUS::PAUSE);
                g_Suns_Colli[i]->GetActorObjOwner()->
                    SetObjectActive(STATUS::PAUSE);
                g_Smal_Trans[i]->SetPosition(MakeFloat3(
                    0.f, 2200.f, 0.f));
                SetExploTo(sunPos);
            }
        }
    }

    static std::string name = "";
    static std::string comp = "";
    static Float3 position = MakeFloat3(0.f, 0.f, 0.f);
    for (int i = 0; i < sunSize; i++)
    {
        name = "sun-" + std::to_string(g_SunIndex[i]) + "-actor";

        comp = name + "-timer";
        if (((ATimerComponent*)(_aitc->
            GetActorObjOwner()->GetSceneNodePtr()->
            GetActorObject(name)->GetAComponent(comp)))->
            GetTimer("wait")->Active)
        {
            if (!((ATimerComponent*)(_aitc->
                GetActorObjOwner()->GetSceneNodePtr()->
                GetActorObject(name)->GetAComponent(comp)))->
                GetTimer("wait")->IsGreaterThan(3.f))
            {
                continue;
            }
            else
            {
                ((ATimerComponent*)(_aitc->
                    GetActorObjOwner()->GetSceneNodePtr()->
                    GetActorObject(name)->GetAComponent(comp)))->
                    ResetTimer("wait");
                ((ATimerComponent*)(_aitc->
                    GetActorObjOwner()->GetSceneNodePtr()->
                    GetActorObject(name)->GetAComponent(comp)))->
                    PauseTimer("wait");
            }
        }

        comp = name + "-transform";
        ((ATransformComponent*)(_aitc->
            GetActorObjOwner()->GetSceneNodePtr()->
            GetActorObject(name)->GetAComponent(comp)))->
            Translate(g_Suns_Speed[i]);

        position = ((ATransformComponent*)(_aitc->
            GetActorObjOwner()->GetSceneNodePtr()->
            GetActorObject(name)->GetAComponent(comp)))->
            GetPosition();

        if (position.y < -2900.f)
        {
            srand((unsigned int)time(NULL) + rand());
            int startX = rand() % 3840 - 1920;
            position = MakeFloat3((float)startX, 540.f, 0.f);
            comp = name + "-transform";
            ((ATransformComponent*)(_aitc->GetActorObjOwner()->
                GetSceneNodePtr()->GetActorObject(name)->
                GetAComponent(comp)))->SetPosition(position);

            srand((unsigned int)time(NULL) + rand());
            int deltaX = (rand() % 3840 - 1920) - startX;
            float speedX = ((float)deltaX) /
                sqrtf((float)(deltaX * deltaX + 7290000));
            float speedY = ((float)-2700) /
                sqrtf((float)(deltaX * deltaX + 7290000));
            DIFFICULTY diffi = GetDifficulty();
            int basic = 0;
            int range = 0;
            switch (diffi)
            {
            case DIFFICULTY::EASY:
                basic = 1;
                range = 2;
                break;
            case DIFFICULTY::NORMAL:
                basic = 1;
                range = 3;
                break;
            case DIFFICULTY::HARD:
                basic = 3;
                range = 3;
                break;
            default:
                break;
            }
            srand((unsigned int)time(NULL) + rand());
            int speedFactor = rand() % range + basic;
            g_Suns_Speed[i].x = speedX * (float)speedFactor;
            g_Suns_Speed[i].y = speedY * (float)speedFactor;

            comp = name + "-timer";
            ((ATimerComponent*)(_aitc->
                GetActorObjOwner()->GetSceneNodePtr()->
                GetActorObject(name)->GetAComponent(comp)))->
                StartTimer("wait");
        }
    }
}

void SunsDestory(AInteractionComponent* _aitc)
{
    g_SunIndex.clear();
    g_Suns_Colli.clear();
    g_Smal_Trans.clear();
    g_Suns_Speed.clear();
}

float GetSunsRatio()
{
    int active = 0;
    for (auto& acc : g_Suns_Colli)
    {
        if (acc->IsCompActive() == STATUS::ACTIVE)
        {
            ++active;
        }
    }

    return (float)active / 20.f;
}
