//---------------------------------------------------------------
// File: SceneManager.cpp
// Proj: HycFrame2D
// Info: シーン管理する機能の対応
// Date: 2021.06.09
// Mail: cai_genkan@outlook.com
// Comt: NULL
//---------------------------------------------------------------

#include "SceneManager.h"
#include "SceneNode.h"
#include "PropertyManager.h"
#include "ObjectFactory.h"
#include <thread>
#include "controller.h"

SceneManager::SceneManager() :
    mPropertyManagerPtr(nullptr), mObjectFactoryPtr(nullptr),
    mLoadingScenePtr(nullptr), mCurrentScenePtr(nullptr),
    mNextScenePtr(nullptr), mLoadSceneFlg(false),
    mLoadSceneInfo({ "","" }), mScenePool({ nullptr,nullptr }),
    mReleaseScenePtr(nullptr), mLoadFinishFlg(true),
    mNeedToLoadSize(0), mHasLoadedSize(0), mShouldTurnOff(false)
{

}

SceneManager::~SceneManager()
{

}

bool SceneManager::StartUp()
{
    bool result = true;

    return result;
}

void SceneManager::PostStartUp(PropertyManager* _pmPtr,
    ObjectFactory* _ofPtr)
{
    mPropertyManagerPtr = _pmPtr;
    mObjectFactoryPtr = _ofPtr;

    LoadLoadingScene();

    std::string name = "";
    std::string path = "";
    {
        JsonFile jf = {};
        LoadJsonFile(&jf, "rom:/Configs/entry-scene.json");
        if (!jf["entry-scene-name"].IsString())
        {
            P_LOG(LOG_ERROR, "invalid entry scene name\n");
        }
        if (!jf["entry-scene-path"].IsString())
        {
            P_LOG(LOG_ERROR, "invalid entry scene path\n");
        }
        name = jf["entry-scene-name"].GetString();
        path = jf["entry-scene-path"].GetString();
    }
    LoadSceneNode(name, path);
}

void SceneManager::CleanAndStop()
{
    mCurrentScenePtr->ReleaseScene();
    delete mCurrentScenePtr;

    if (mNextScenePtr)
    {
        mNextScenePtr->ReleaseScene();
        delete mNextScenePtr;
    }

    if (mCurrentScenePtr != mLoadingScenePtr)
    {
        ReleaseLoadingScene();
        delete mLoadingScenePtr;
    }
}

void SceneManager::UpdateSceneManager(float _deltatime)
{
    if (mLoadSceneFlg)
    {
        mLoadSceneFlg = false;

        mCurrentScenePtr = mLoadingScenePtr;
        std::thread loadThread(
            &SceneManager::LoadNextScene, this);
        loadThread.detach();
    }

    if (mReleaseScenePtr)
    {
        mReleaseScenePtr->ReleaseScene();
        delete mReleaseScenePtr;
        mReleaseScenePtr = nullptr;
    }

    if (mNextScenePtr && mLoadFinishFlg)
    {
        mCurrentScenePtr = mNextScenePtr;
        mNextScenePtr = nullptr;
    }

    mCurrentScenePtr->UpdateScene(_deltatime);
    mCurrentScenePtr->DrawScene();
}

PropertyManager* SceneManager::GetPropertyManager() const
{
    return mPropertyManagerPtr;
}

ObjectFactory* SceneManager::GetObjectFactory() const
{
    return mObjectFactoryPtr;
}

void SceneManager::LoadSceneNode(
    std::string _name, std::string _path)
{
    mLoadSceneFlg = true;
    mLoadSceneInfo = { _name,_path };

    {
        JsonFile config = {};
        LoadJsonFile(&config, _path);
        unsigned int actorSize = config["actor"].Size();
        unsigned int uiSize = config["ui"].Size();
        mNeedToLoadSize = actorSize + uiSize;
        mHasLoadedSize = 0;
    }
}

unsigned int SceneManager::GetNeedToLoad() const
{
    return mNeedToLoadSize;
}

unsigned int SceneManager::GetHasLoaded() const
{
    return mHasLoadedSize;
}

void SceneManager::PlusHasLoaded()
{
    ++mHasLoadedSize;
#ifdef SHOW_LOADING
#ifdef HYC_FRAME_2D
    Sleep(10);
#endif // HYC_FRAME_2D
#endif // SHOW_LOADING
}

bool SceneManager::GetLoadFinishedFlag() const
{
    return mLoadFinishFlg;
}

void SceneManager::SetLoadFinishedFlag(bool _value)
{
    mLoadFinishFlg = _value;
}

void SceneManager::SetShouldTurnOff(bool _value)
{
    mShouldTurnOff = _value;
}

bool SceneManager::GetShoudTurnOff() const
{
    return mShouldTurnOff;
}

void SceneManager::LoadLoadingScene()
{
    mLoadingScenePtr = mObjectFactoryPtr->CreateNewScene(
        "load-scene",
        "rom:/Configs/Scenes/load-scene.json");
}

void SceneManager::ReleaseLoadingScene()
{
    mLoadingScenePtr->ReleaseScene();
}

void SceneManager::LoadNextScene()
{
    P_LOG(LOG_MESSAGE, "ready to load next scene\n");

    for (int i = 0; i < 2; i++)
    {
        if (mScenePool[i] &&
            mScenePool[i]->GetSceneName() == mLoadSceneInfo[0])
        {
            SceneNode* node = mScenePool[i];
            node->ResetSceneNode();
            if (i)
            {
                SceneNode* temp = mScenePool[0];
                mScenePool[0] = mScenePool[1];
                mScenePool[1] = temp;
            }
            mNextScenePtr = node;
            return;
        }
    }

    SceneNode* node = mObjectFactoryPtr->CreateNewScene(
        mLoadSceneInfo[0], mLoadSceneInfo[1]);

    if (!mScenePool[0])
    {
        mScenePool[0] = node;
    }
    else if (!mScenePool[1])
    {
        mScenePool[1] = mScenePool[0];
        mScenePool[0] = node;
    }
    else
    {
        SceneNode* needRelease = mScenePool[1];
        mScenePool[1] = mScenePool[0];
        mScenePool[0] = node;
        mReleaseScenePtr = needRelease;
    }
    mNextScenePtr = node;
}
