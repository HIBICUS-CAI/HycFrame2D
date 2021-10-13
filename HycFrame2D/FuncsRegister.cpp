//---------------------------------------------------------------
// File: FuncsRegister.h
// Proj: HycFrame2D
// Info: スクリプト関数を作成して保存するところ
// Date: 2021.10.13
// Mail: cai_genkan@outlook.com
// Comt: NULL
//---------------------------------------------------------------

#include "ObjectFactory.h"

#include "..\TempBtnCmd.h"
#include "..\TitleButton.h"
#include "..\ModeSelectButton.h"
#include "..\BowLoadingFade.h"
#include "..\BowAction.h"
#include "..\ArrowAction.h"
#include "..\AimPointAction.h"
#include "..\SunsManager.h"
#include "..\FieldAction.h"
#include "..\CloudAction.h"
#include "..\ResultLoadingFade.h"
#include "..\ResultSelectInput.h"
#include "..\ExtraLoadingFade.h"
#include "..\ExtraPlayerInput.h"
#include "..\ExtraSunManager.h"
#include "..\ExtraLife.h"
#include "..\ExtraGods.h"
#include "..\ManualAction.h"
#include "..\StoryAction.h"

void RegisterAllFuncs(ObjectFactory* _factory)
{
    RegisterTempBtnCmd(_factory);
    RegisterTitleButton(_factory);
    RegisterModeSelectButton(_factory);
    RegisterBowLoadingFade(_factory);
    RegisterBowAction(_factory);
    RegisterArrowAction(_factory);
    RegisterAimPointAction(_factory);
    RegisterSunsManager(_factory);
    RegisterFieldAction(_factory);
    RegisterCloudAction(_factory);
    RegisterResultLoadingFade(_factory);
    RegisterResultInput(_factory);
    RegisterExtraLoadingFade(_factory);
    RegisterExtraPlayerInput(_factory);
    RegisterExtraSunsManager(_factory);
    RegisterExtraLife(_factory);
    RegisterExtraGodsManager(_factory);
    RegisterManualAction(_factory);
    RegisterStoryAction(_factory);
}
