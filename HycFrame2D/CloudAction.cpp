#include "CloudAction.h"
#include "BowLoadingFade.h"
#include <stdlib.h>
#include <time.h>

const static int LEFT = -1;
const static int STOP = 0;
const static int RIGHT = 1;
static int g_Dest[5] = { STOP,STOP,STOP,STOP,STOP };

static ActorObject* g_Clouds[5] =
{
    nullptr,nullptr,nullptr,nullptr,nullptr
};
static ATransformComponent* g_Clouds_Atc[5] =
{
    nullptr,nullptr,nullptr,nullptr,nullptr
};
static ASpriteComponent* g_Clouds_Asc[5] =
{
    nullptr,nullptr,nullptr,nullptr,nullptr
};
static ATimerComponent* g_Clouds_Atic[5] =
{
    nullptr,nullptr,nullptr,nullptr,nullptr
};
static AAnimateComponent* g_Clouds_Aac[5] =
{
    nullptr,nullptr,nullptr,nullptr,nullptr
};
static Float4 g_Clouds_Color[5] =
{
    MakeFloat4(1.f,1.f,1.f,0.f),
    MakeFloat4(1.f,1.f,1.f,0.f),
    MakeFloat4(1.f,1.f,1.f,0.f),
    MakeFloat4(1.f,1.f,1.f,0.f),
    MakeFloat4(1.f,1.f,1.f,0.f)
};

void RegisterCloudAction(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(CloudInit), CloudInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(CloudUpdate), CloudUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(CloudDestory), CloudDestory));
}

void CloudInit(AInteractionComponent* _aitc)
{
    std::string name = "";
    std::string comp = "";
    SceneNode* node = _aitc->
        GetActorObjOwner()->GetSceneNodePtr();
    int basic = 0;
    int range = 0;
    float fv1 = 0.f;
    float fv2 = 0.f;
    if (!node)
    {
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        name = "cloud-" + std::to_string(i + 1) + "-actor";
        g_Clouds[i] = node->GetActorObject(name);

        srand((unsigned int)time(NULL) + rand());
        range = 3;
        basic = -1;
        g_Dest[i] = rand() % range + basic;

        comp = name + "-transform";
        g_Clouds_Atc[i] = (ATransformComponent*)(g_Clouds[i]->
            GetAComponent(comp));
        srand((unsigned int)time(NULL) + rand());
        range = 1920 * 2;
        basic = -1920;
        fv1 = (float)(rand() % range + basic);
        srand((unsigned int)time(NULL) + rand());
        range = 1080 * 3;
        basic = -1080 * 2 - 540;
        fv2 = (float)(rand() % range + basic);
        g_Clouds_Atc[i]->SetPosition(MakeFloat3(fv1, fv2, 0.f));

        comp = name + "-sprite";
        g_Clouds_Asc[i] = (ASpriteComponent*)(g_Clouds[i]->
            GetAComponent(comp));
        g_Clouds_Color[i].w = 0.f;
        g_Clouds_Asc[i]->SetOffsetColor(g_Clouds_Color[i]);

        comp = name + "-timer";
        g_Clouds_Atic[i] = (ATimerComponent*)(g_Clouds[i]->
            GetAComponent(comp));
        g_Clouds_Atic[i]->StartTimer("keep");

        comp = name + "-animate";
        g_Clouds_Aac[i] = (AAnimateComponent*)(g_Clouds[i]->
            GetAComponent(comp));
        g_Clouds_Aac[i]->ChangeAnimateTo("active");
    }
}

void CloudUpdate(AInteractionComponent* _aitc, float _deltatime)
{
    static int basic = 0;
    static int range = 0;
    static float fv1 = 0.f;
    static float fv2 = 0.f;

    for (int i = 0; i < 5; i++)
    {
        if (g_Clouds_Atic[i]->GetTimer("keep")->
            IsGreaterThan(4.5f))
        {
            g_Clouds_Atic[i]->StartTimer("wait");
            g_Clouds_Atic[i]->ResetTimer("keep");
            g_Clouds_Atic[i]->PauseTimer("keep");

            srand((unsigned int)time(NULL) + rand());
            range = 1920 * 2;
            basic = -1920;
            fv1 = (float)(rand() % range + basic);
            srand((unsigned int)time(NULL) + rand());
            range = 1080 * 3;
            basic = -1080 * 2 - 540;
            fv2 = (float)(rand() % range + basic);
            g_Clouds_Atc[i]->SetPosition(
                MakeFloat3(fv1, fv2, 0.f));

            g_Clouds_Color[i].w = 0.f;
            g_Clouds_Asc[i]->SetOffsetColor(g_Clouds_Color[i]);
        }

        if (g_Clouds_Atic[i]->GetTimer("wait")->Active)
        {
            if (g_Clouds_Atic[i]->GetTimer("wait")->
                IsGreaterThan(3.f))
            {
                g_Clouds_Atic[i]->StartTimer("keep");
                g_Clouds_Atic[i]->ResetTimer("wait");
                g_Clouds_Atic[i]->PauseTimer("wait");
                g_Clouds_Aac[i]->ChangeAnimateTo("active");
            }
            else
            {
                continue;
            }
        }

        g_Clouds_Atc[i]->TranslateXAsix(5.f * (float)g_Dest[i]);
        
        if (g_Clouds_Color[i].w < 1.f)
        {
            g_Clouds_Color[i].w += 0.01f;
        }
        g_Clouds_Asc[i]->SetOffsetColor(g_Clouds_Color[i]);
    }
}

void CloudDestory(AInteractionComponent* _aitc)
{

}
