//
//  DrawingCanvas.h
//  Doodler
//
//  Created by AimSpike on 4/25/16.
//
//

#ifndef DrawingCanvas_h
#define DrawingCanvas_h

#include "cocos2d.h"
#include "CocosGUI.h"

class DrawingCanvas : public cocos2d::Node
{
public:
    CREATE_FUNC(DrawingCanvas);
    
    void setNetworkedSession(bool networked);
    bool getNetworkedSession();
    
    void receivedData(const void* data, unsigned long length);

protected:

    cocos2d::DrawNode* drawNode;
    
    cocos2d::LayerColor* background;
    
    cocos2d::Sprite* check;

    cocos2d::Color4F selectedColor;

    bool networkedSession;
    
    bool init() override;
    void onEnter() override;
    
    void setupMenus();
    void clearPressed(Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void backPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    
    void colorChangePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void setupTouchHandling();
    
    void sendStrokeOverNetwork(cocos2d::Vec2 startPoint, cocos2d::Vec2 endPoint, float radius, cocos2d::Color4F color);

};

#endif /* DrawingCanvas_hpp */
