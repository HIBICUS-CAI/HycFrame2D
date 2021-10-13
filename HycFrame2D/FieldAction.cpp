#include "FieldAction.h"
#include "BowLoadingFade.h"
#include "SunsManager.h"
#include "Ui_all.h"

static UiObject* g_FieldHpBar = nullptr;
static ActorObject* g_Field = nullptr;
static float g_FieldHp = 0.f;
static float g_MaxFieldHp = 0.f;
static Float4 g_FinalColor = MakeFloat4(0.f, 0.f, 0.f, 0.f);

void RegisterFieldAction(ObjectFactory* _factory)
{
    auto aInitPool = _factory->GetActorInterInitPool();
    auto aUpdatePool = _factory->GetActorInterUpdatePool();
    auto aDestoryPool = _factory->GetActorInterDestoryPool();

    aInitPool->insert(std::make_pair(
        FUNC_NAME(FieldInit), FieldInit));
    aUpdatePool->insert(std::make_pair(
        FUNC_NAME(FieldUpdate), FieldUpdate));
    aDestoryPool->insert(std::make_pair(
        FUNC_NAME(FieldDestory), FieldDestory));
}

void FieldInit(AInteractionComponent* _aitc)
{
    g_FieldHpBar = _aitc->GetActorObjOwner()->GetSceneNodePtr()->
        GetUiObject("grass-hp-ui");
    g_Field = _aitc->GetActorObjOwner();
    g_FieldHp = 1200.f;
    g_MaxFieldHp = 1200.f;
    ((UTransformComponent*)(g_FieldHpBar->
        GetUComponent("grass-hp-ui-transform")))->
        SetPosition(MakeFloat3(700.f, -400.f, 0.f));
    g_FinalColor = MakeFloat4(0.75f, 0.5f, 0.f, 1.f);
    ((ASpriteComponent*)(g_Field->
        GetAComponent("field-only-actor-sprite")))->
        SetOffsetColor(MakeFloat4(1.f, 1.f, 1.f, 1.f));
}

void FieldUpdate(AInteractionComponent* _aitc, float _deltatime)
{
    if (!GetBowSceneCanUpdate())
    {
        return;
    }
    static float ratio = 1.f;
    static Float3 newPos = MakeFloat3(0.f, 0.f, 0.f);
    static Float4 newColor = MakeFloat4(1.f, 1.f, 1.f, 1.f);
    static float factor = 1.f;
    UTransformComponent* utc = (UTransformComponent*)
        (g_FieldHpBar->GetUComponent("grass-hp-ui-transform"));
    if (!utc)
    {
        return;
    }
    g_FieldHp -= GetSunsRatio();

    if (g_FieldHp < 0.f)
    {
        SetBowSceneEnd(false);
    }

    utc->SetScale(MakeFloat3(1.f, 1.f, 1.f));
    ratio = g_FieldHp / g_MaxFieldHp;
    factor = 1.f - ratio;
    utc->ScaleXAsix(ratio);
    newPos = utc->GetPosition();
    ratio = 190.f * (1.f - ratio);
    newPos.x = 700.f - ratio;
    utc->SetPosition(newPos);

    newColor.x = 1.f - factor * (1.f - g_FinalColor.x);
    newColor.y = 1.f - factor * (1.f - g_FinalColor.y);
    newColor.z = 1.f - factor * (1.f - g_FinalColor.z);
    ((ASpriteComponent*)(g_Field->
        GetAComponent("field-only-actor-sprite")))->
        SetOffsetColor(newColor);
}

void FieldDestory(AInteractionComponent* _aitc)
{

}
