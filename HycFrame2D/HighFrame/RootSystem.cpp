//---------------------------------------------------------------
// File: RootSystem.cpp
// Proj: HycFrame2D
// Info: システムを全体的に管理する
// Date: 2021.06.09
// Mail: cai_genkan@outlook.com
// Comt: NULL
//---------------------------------------------------------------

#include "RootSystem.h"
#include "SceneManager.h"
#include "PropertyManager.h"
#include "ObjectFactory.h"
#include "main.h"
#include "controller.h"
#include "sound.h"

RootSystem::RootSystem() :
    mSceneManagerPtr(nullptr), mPropertyManagerPtr(nullptr),
    mObjectFactoryPtr(nullptr), mLastTime(0.f), mDeltaTime(0.16f)
{

}

RootSystem::~RootSystem()
{

}

bool RootSystem::StartUp(HINSTANCE hInstance, int cmdShow)
{
    P_LOG(LOG_MESSAGE,
        "[START UP] : starting up ROOT SYSTEM\n");

    mSceneManagerPtr = new SceneManager();
    mPropertyManagerPtr = new PropertyManager();
    mObjectFactoryPtr = new ObjectFactory();

    bool result1 = InitSystem(hInstance, cmdShow);
    bool result2 = InitSound();
    bool result3 = mSceneManagerPtr->StartUp();
    bool result4 = mPropertyManagerPtr->StartUp();
    bool result5 = mObjectFactoryPtr->StartUp(
        mPropertyManagerPtr, mSceneManagerPtr);
    mSceneManagerPtr->PostStartUp(
        mPropertyManagerPtr, mObjectFactoryPtr);

    InitController();

    bool result = result1 && result2 &&
        result3 && result4 && result5;
    if (result)
    {
        P_LOG(LOG_MESSAGE,
            "[START UP] : start up ROOT SYSTEM successed\n");
    }
    else
    {
        P_LOG(LOG_ERROR,
            "[START UP] : start up ROOT SYSTEM failed\n");
    }

    return result;
}

void RootSystem::ClearAndStop()
{
    if (mSceneManagerPtr)
    {
        mSceneManagerPtr->CleanAndStop();
        delete mSceneManagerPtr;
        mSceneManagerPtr = nullptr;
    }
    if (mPropertyManagerPtr)
    {
        mPropertyManagerPtr->CleanAndStop();
        delete mPropertyManagerPtr;
        mPropertyManagerPtr = nullptr;
    }
    if (mObjectFactoryPtr)
    {
        mObjectFactoryPtr->CleanAndStop();
        delete mObjectFactoryPtr;
        mObjectFactoryPtr = nullptr;
    }

    UninitController();
    UninitSound();
    UninitSystem();

    P_LOG(LOG_MESSAGE,
        "[CLEAN STOP] : stop ROOT SYSTEM successed\n");
}

void RootSystem::RunGameLoop()
{
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            GetDxHelperPtr()->ClearBuffer();

            UpdateController();

            mSceneManagerPtr->UpdateSceneManager(mDeltaTime);

            SwapAndClacDeltaTime();

            if (ShouldQuit() || 
                mSceneManagerPtr->GetShoudTurnOff())
            {
                PostQuitMessage(0);
            }
        }
    }
}

void RootSystem::SwapAndClacDeltaTime()
{
    float time = 0.f;
#ifdef HYC_FRAME_2D
    time = SwapBuffers();
#else
    SwapBuffers();
    nn::os::Tick t = nn::os::GetSystemTick();
    time = (float)(t.ToTimeSpan().GetMilliSeconds()) / 1000.f;
#endif // HYC_FRAME_2D
    mDeltaTime = time - mLastTime;
    mLastTime = time;
    time = MAX_DELTA - mDeltaTime;
    //NN_LOG("current delta : %f , delta with max %f\n",
        //mDeltaTime, time);
    if (time > 0.001f)
    {
        mLastTime += time;
#ifdef HYC_FRAME_2D
        Sleep((int)(1000.f * time));
#else

#endif // HYC_FRAME_2D
        mDeltaTime = MAX_DELTA;
    }
    else if (time < 0.f)
    {
        mDeltaTime = MAX_DELTA;
    }
    //NN_LOG("final delta : %f\n", mDeltaTime);
}
