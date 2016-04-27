//
//  JSONPacker.hpp
//  Doodler
//
//  Created by AimSpike on 4/27/16.
//
//

#ifndef JSONPacker_h
#define JSONPacker_h

#include "cocos2d.h"

namespace JSONPacker
{
    
    struct LineData
    {
        cocos2d::Vec2 startPoint;
        cocos2d::Vec2 endPoint;
        cocos2d::Color4F color;
        float radius;
    };
    
    LineData unpackLineDataJSON(std::string json);
    std::string packLineData(LineData lineData);
    
}

#endif /* JSONPacker_h */
