#include "TempBtnCmd.h"
#include "ExtraLoadingFade.h"
#include "controller.h"

void RegisterTempBtnCmd(ObjectFactory* _factory)
{
    auto uInputPool = _factory->GetUiInputPool();

    uInputPool->insert(std::make_pair(
        FUNC_NAME(TitleTempInput), TitleTempInput));
    uInputPool->insert(std::make_pair(
        FUNC_NAME(BowTempInput), BowTempInput));
    uInputPool->insert(std::make_pair(
        FUNC_NAME(ResultTempInput), ResultTempInput));
    uInputPool->insert(std::make_pair(
        FUNC_NAME(ExtraTempInput), ExtraTempInput));
}

void TitleTempInput(UInputComponent* _uic, float _deltatime)
{
    if (GetControllerTrigger(NpadButton::Minus::Index))
    {
        _uic->GetUiObjOwner()->GetSceneNodePtr()->
            GetSceneManagerPtr()->LoadSceneNode(
                "bow-scene",
                "rom:/Configs/Scenes/bow-scene.json");
    }
}

void BowTempInput(UInputComponent* _uic, float _deltatime)
{
    if (GetControllerTrigger(NpadButton::Minus::Index))
    {
        _uic->GetUiObjOwner()->GetSceneNodePtr()->
            GetSceneManagerPtr()->LoadSceneNode(
                "result-scene",
                "rom:/Configs/Scenes/result-scene.json");
    }
}

void ResultTempInput(UInputComponent* _uic, float _deltatime)
{
    if (GetControllerTrigger(NpadButton::Minus::Index))
    {
        _uic->GetUiObjOwner()->GetSceneNodePtr()->
            GetSceneManagerPtr()->LoadSceneNode(
                "title-scene",
                "rom:/Configs/Scenes/title-scene.json");
    }
}

void ExtraTempInput(UInputComponent* _uic, float _deltatime)
{
    if (GetControllerTrigger(NpadButton::Minus::Index))
    {
        _uic->GetUiObjOwner()->GetSceneNodePtr()->
            GetSceneManagerPtr()->LoadSceneNode(
                "title-scene",
                "rom:/Configs/Scenes/title-scene.json");
    }
    else if (GetControllerTrigger(NpadButton::Plus::Index))
    {
        _uic->GetUiObjOwner()->GetSceneNodePtr()->
            GetSceneManagerPtr()->LoadSceneNode(
                "extra-scene",
                "rom:/Configs/Scenes/extra-scene.json");
    }
}
