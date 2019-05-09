//
//  TextureBlur.hpp
//  MergeNumber
//
//  Created by Tech4 on 5/6/19.
//

#ifndef TextureBlur_hpp
#define TextureBlur_hpp

#pragma once

#include "cocos2d.h"

class TextureBlur
{
public:
    static void create(cocos2d::Texture2D* target, const int radius, const std::string& fileName, std::function<void()> callback, const int step = 1);
    
private:
    static void calculateGaussianWeights(const int points, float* weights);
    static cocos2d::GLProgram* getBlurShader(cocos2d::Vec2 pixelSize, cocos2d::Vec2 direction, const int radius, float* weights);
};

#endif /* TextureBlur_hpp */
