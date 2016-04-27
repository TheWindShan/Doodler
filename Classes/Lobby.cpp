//
//  Lobby.cpp
//  Doodler
//
//  Created by AimSpike on 4/26/16.
//
//

#include "Lobby.h"
#include "DrawingCanvas.h"
#include "Constants.h"
#include "SceneManager.h"

using namespace cocos2d;

bool Lobby::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    LayerColor* background = LayerColor::create(Color4B(COLOR_WHITE));
    this->addChild(background);
    
    return true;
}

void Lobby::onEnter()
{
    Node::onEnter();
    
    setupUI();
}

void Lobby::setupUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    CCLOG("%f %f", visibleSize.width, visibleSize.height);
    CCLOG("%f %f", origin.x, origin.y);
    
    /////////////////////////////////////////////////////
    // logo
    Sprite* logo = Sprite::create("doodlerLogo.png");
    logo->setAnchorPoint(Vec2(0.5f, 0.5f));
    logo->setPosition(Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.75f + origin.y));
    
    this->addChild(logo);
    
    /////////////////////////////////////////////////////
    // solo button
    ui::Button* soloButton = ui::Button::create();
    soloButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    soloButton->setPosition(Vec2(visibleSize.width / 2.0f + origin.x, visibleSize.height * 0.4f + origin.y));
    soloButton->loadTextures("soloButton.png", "soloButtonPressed.png");
    soloButton->addTouchEventListener(CC_CALLBACK_2(Lobby::soloPressed, this));
    
    this->addChild(soloButton);

    /////////////////////////////////////////////////////
    // duo button
    ui::Button* duoButton = ui::Button::create();
    duoButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    duoButton->setPosition(Vec2(visibleSize.width / 2.0f + origin.x, visibleSize.height * 0.25f + origin.y));
    duoButton->loadTextures("duoButton.png", "duoButtonPressed.png");
    duoButton->addTouchEventListener(CC_CALLBACK_2(Lobby::duoPressed, this));
    
    this->addChild(duoButton);

}

void Lobby::soloPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->enterSoloGame();

    }
}

void Lobby::duoPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->connectAndEnterNetworkedGame();
    }
}