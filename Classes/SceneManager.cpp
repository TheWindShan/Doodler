//
//  SceneManager.cpp
//  Doodler
//
//  Created by AimSpike on 4/27/16.
//
//

#include "SceneManager.h"

using namespace cocos2d;

// 
SceneManager::SceneManager()
{
    drawingCanvas = nullptr;
    
    networkingWrapper = new NetworkingWrapper();
    networkingWrapper->setDelegate(this);
}


SceneManager::~SceneManager()
{
    delete networkingWrapper;
}

void SceneManager::connectAndEnterNetworkedGame()
{
    networkingWrapper->attemptToJoinGame();
}

void SceneManager::sendData(const void* data, unsigned long length)
{
    networkingWrapper->sendData(data, length);
}


void SceneManager::receivedData(const void* data, unsigned long length)
{
    if (drawingCanvas)
    {
        drawingCanvas->receivedData(data, length);
    }
}

void SceneManager::stateChanged(ConnectionState state)
{
    switch (state)
    {
        case ConnectionState::NOT_CONNECTED:
            CCLOG("Not Connected");
            break;
            
        case ConnectionState::CONNECTING:
            CCLOG("Connecting...");
            break;
            
        case ConnectionState::CONNECTED:
            CCLOG("Connected!");
            if (!drawingCanvas)
            {
                this->loadDrawingScene(true);
            }
            
            break;
    }
}

static SceneManager* sharedSceneManager = nullptr;

SceneManager* SceneManager::getInstance()
{
    if (! sharedSceneManager)
    {
        sharedSceneManager = new SceneManager();
    }
    
    return sharedSceneManager;
}


void SceneManager::loadDrawingScene(bool networked)
{
    auto scene = Scene::create();
    
    drawingCanvas = DrawingCanvas::create();
    drawingCanvas->setNetworkedSession(networked);
    
    scene->addChild(drawingCanvas);
    
    Director::getInstance()->pushScene(scene);
}


void SceneManager::enterSoloGame()
{
    this->loadDrawingScene(false);
}


void SceneManager::returnToLobby()
{
    if (drawingCanvas)
    {
        Director::getInstance()->popScene();
        drawingCanvas = nullptr;
    }
}