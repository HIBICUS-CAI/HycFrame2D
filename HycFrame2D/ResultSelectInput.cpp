#include "ResultSelectInput.h"
#include "ResultLoadingFade.h"
#include "controller.h"
#include "sound.h"

void RegisterResultInput(ObjectFactory* _factory)
{
    auto uInputPool = _factory->GetUiInputPool();

    uInputPool->insert(std::make_pair(
        FUNC_NAME(ResultSelectInput), ResultSelectInput));
}

void ResultSelectInput(UInputComponent* _uic, float _deltatime)
{
    if (!GetResultSceneCanUpdate())
    {
        return;
    }

    UiObject* owner = _uic->GetUiObjOwner();

    static std::string btnmapName = "";
    btnmapName = owner->GetObjectName() + "-btnmap";
    UBtnMapComponent* ubmc = nullptr;
    ubmc = (UBtnMapComponent*)owner->GetUComponent(btnmapName);

    if (GetControllerTrigger(NpadButton::Left::Index))
    {
        if (ubmc && ubmc->IsBeingSelected())
        {
            ubmc->SelectLeftBtn();
            PlaySE("se-change");
        }
    }
    else if (GetControllerTrigger(NpadButton::Right::Index))
    {
        if (ubmc && ubmc->IsBeingSelected())
        {
            ubmc->SelectRightBtn();
            PlaySE("se-change");
        }
    }
    else if (GetControllerTrigger(NpadButton::A::Index))
    {
        if (ubmc && ubmc->IsBeingSelected() &&
            owner->GetObjectName() ==
            "retry-btn-ui")
        {
            PlaySE("se-choose");
            SetResultSceneEnd(true);
        }
        else if (ubmc && ubmc->IsBeingSelected() &&
            owner->GetObjectName() ==
            "return-btn-ui")
        {
            PlaySE("se-choose");
            SetResultSceneEnd(false);
        }
    }
}
