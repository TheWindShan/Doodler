//
//  DrawingCanvas.cpp
//  Doodler
//
//  Created by AimSpike on 4/25/16.
//
//

#include "DrawingCanvas.h"
#include "Constants.h"
#include "SceneManager.h"
#include "JSONPacker.h"

using namespace cocos2d;

bool DrawingCanvas::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    drawNode = DrawNode::create();
    
    background = LayerColor::create(Color4B(COLOR_WHITE));

    this->addChild(background);
    this->addChild(drawNode);

    selectedColor = COLOR_GREEN;
    
    networkedSession = false;
    
    return true;
}

/////////////////////////////////////////////////////////
// netwoked session methods
void DrawingCanvas::setNetworkedSession(bool networked)
{
    this->networkedSession = networked;
}

bool DrawingCanvas::getNetworkedSession()
{
    return this->networkedSession;
}

void DrawingCanvas::receivedData(const void* data, unsigned long length)
{
    const char* cstr = reinterpret_cast<const char*>(data);

    std::string json = std::string(cstr, length);
    
    JSONPacker::LineData lineData = JSONPacker::unpackLineDataJSON(json);

    drawNode->drawSegment(lineData.startPoint, lineData.endPoint, lineData.radius, lineData.color);

}

void DrawingCanvas::sendStrokeOverNetwork(Vec2 startPoint, Vec2 endPoint, float radius, Color4F color)
{
    JSONPacker::LineData lineData;
    lineData.startPoint = startPoint;
    lineData.endPoint = endPoint;
    lineData.radius = radius;
    lineData.color = color;
    
    std::string json = JSONPacker::packLineData(lineData);
    
    SceneManager::getInstance()->sendData(json.c_str(), json.length());
}

void DrawingCanvas::onEnter()
{
    Node::onEnter();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setContentSize(visibleSize);
    drawNode->setContentSize(visibleSize);
    
    setupMenus();
    setupTouchHandling();
}

void DrawingCanvas::setupMenus()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //
    ui::Button* clearButton = ui::Button::create();
    clearButton->setAnchorPoint(Vec2(1.0f, 1.0f));
    clearButton->setPosition(Vec2(visibleSize.width + origin.x, visibleSize.height + origin.y));
    clearButton->loadTextures("clearButton.png", "clearButtonPressed.png");
    clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::clearPressed, this));
    this->addChild(clearButton);
    
    //
    ui::Button* backButton = ui::Button::create();
    backButton->setAnchorPoint(Vec2(0.0f, 1.0f));
    backButton->setPosition(Vec2(0.0f  + origin.x, visibleSize.height + origin.y));
    backButton->loadTextures("backButton.png", "backButtonPressed.png");
    backButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::backPressed, this));
    this->addChild(backButton);
    
    
    check = Sprite::create("checkMark.png");
    check->setAnchorPoint(Vec2(0.5f, 0.5f));
    check->setNormalizedPosition(Vec2(0.5f, 0.5f));
    
    Node* colorButtonLayout = Node::create();
    colorButtonLayout->setContentSize(Size(visibleSize.width, visibleSize.height * 0.2f));
    colorButtonLayout->setAnchorPoint(Vec2(0.5f, 0.0f));
    colorButtonLayout->setPosition(Vec2(visibleSize.width / 2.0f + origin.x, 0.0f));
    this->addChild(colorButtonLayout);
    
    for (int i = 1; i <= 5; ++i)
    {
        ui::Button* colorButton = ui::Button::create();
        colorButton->setAnchorPoint(Vec2(0.5f, 0.0f));
        colorButton->setPosition(Vec2(visibleSize.width * i * (1.0f/6.0f), 0.0f));
        colorButton->loadTextures("colorSwatch.png", "colorSwatch.png");
        colorButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::colorChangePressed, this));
        if (i == 1)  colorButton->setColor(Color3B(COLOR_RED));
        else if (i == 2)  colorButton->setColor(Color3B(COLOR_YELLOW));
        else if (i == 3)  colorButton->setColor(Color3B(COLOR_GREEN));
        else if (i == 4)  colorButton->setColor(Color3B(COLOR_BLUE));
        else if (i == 5)  colorButton->setColor(Color3B(COLOR_PURPLE));
        colorButtonLayout->addChild(colorButton);
        
        if (i == 3) colorButton->addChild(this->check);
    }
    
}

void DrawingCanvas::clearPressed(Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        drawNode->clear();
    }
}

void DrawingCanvas::backPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->returnToLobby();
    }
}


void DrawingCanvas::colorChangePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    this->check->retain();
    this->check->removeFromParent();
    pressedButton->addChild(this->check);
    selectedColor = Color4F(check->getParent()->getColor());
    //this->check->release();
}

void DrawingCanvas::setupTouchHandling()
{
    static Vec2 lastTouchPos;
    static float lastRadius = INITIAL_RADIUS;
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
        lastRadius = INITIAL_RADIUS;
        return true;
    };
    
    touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        Vec2 touchPos = drawNode->convertTouchToNodeSpace(touch);
        
        float distance = lastTouchPos.distance(touchPos);
        
        float dt = 1.0f / 60.0f;
        float rc = 1.0f;
        float alpha = dt / (rc + dt);
        float radius = (alpha * distance) + (1.0f - alpha) * lastRadius;
        
        drawNode->drawSegment(lastTouchPos, touchPos, radius, selectedColor);
        
        if (this->networkedSession)
        {
            this->sendStrokeOverNetwork(lastTouchPos, touchPos, radius, selectedColor);
        }
        
        lastRadius = radius;
        lastTouchPos = touchPos;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}