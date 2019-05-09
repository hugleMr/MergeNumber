//
//  MauBinhMauBinhCardSprite.cpp
//  MyCards
//
//  Created by hungle on 8/1/16.
//
//

#include "Item.hpp"

using namespace cocos2d;

Item* Item::createCard(int value,ItemMoveDelegate* cardEvent) {
    Item* cardSprite = Item::create();
    string name = value < 0 ? "item.png" : value%7 == 0 ? "item_7.png" : StringUtils::format("item_%d.png",value%7);
    cardSprite->setSpriteFrame(name);
    cardSprite->setEvent(cardEvent);
    cardSprite->addEvents();
    cardSprite->setValue(value);
    cardSprite->addNumber();
    return cardSprite;
}

Item* Item::createItemView(int index,int value) {
    Item* cardSprite = Item::create();
    string name = value < 0 ? "item.png" : value%7 == 0 ? "item_7.png" : StringUtils::format("item_%d.png",value%7);
    cardSprite->setSpriteFrame(name);
    cardSprite->setIndex(index);
    cardSprite->setValue(value);
    cardSprite->addNumber();
    return cardSprite;
}

void Item::appear(bool isAppear){
    this->isAppear = isAppear;
    this->setOpacity(isAppear ? 255 : 0);
}

void Item::addNumber(){
    number = Label::createWithTTF("0" ,"fonts/Arial_Bold.ttf",this->getHeight()*0.65);
    this->addChild(number);
    
    this->updateItem();
}

void Item::upgradeValue(){
    this->_value += 1;
    this->updateItem();
}

void Item::updateItem(){
    
    if(this->getValue() > 9){
        number->setPosition(Vec2(this->getSize()/2) - Vec2(this->getWidth()*0.05,0));
    }else{
        number->setPosition(Vec2(this->getSize()/2) - Vec2(this->getWidth()*0.02,0));
    }
    int value = this->getValue();
    auto txt_number = StringUtils::format("%d",value);
    if(value < 0){
        txt_number = "";
        this->appear(false);
    }else{
        this->appear(true);
    }
    
    string name = value < 0 ? "item.png" : value%7 == 0 ? "item_7.png" : StringUtils::format("item_%d.png",value%7);
    this->setSpriteFrame(name);
    this->number->setString(txt_number);
    this->getValue()%7 != 0 ? number->setColor(Color3B(50,68,97)) : number->setColor(Color3B::WHITE);
}

float Item::getWidth(){
    return this->getContentSize().width;
}

float Item::getHeight(){
    return this->getContentSize().height;
}

cocos2d::Size Item::getSize(){
    return this->getContentSize();
}

bool Item::init() {
    if (!Sprite::init()) {
        return false;
    }
    
    return true;
}

void Item::onEnter() {
    Sprite::onEnter();
}

Point touchToPoint(Touch* touch){
    return Director::getInstance()->convertToGL(touch->getLocationInView());
}

void Item::addEvents(){
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();
        
        if(rect.containsPoint(p)) {
            this->touchOffset  = this->getPosition() - touchToPoint(touch);
            this->setLocalZOrder(2);
            return true;
        }
        return false;
    };

    listener->onTouchMoved = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
        this->setPosition(touch->getLocation() + this->touchOffset);
        
        cocos2d::Vec2 p = touch->getLocation() + this->touchOffset;
        this->cardEvent->onEventMove(this->getValue(),p.x,p.y);
        
    };
    
    listener->onTouchEnded = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
        cocos2d::Vec2 p = touch->getLocation() + this->touchOffset;
        this->cardEvent->onEventMove(this->getValue(),p.x,p.y,true);
        this->setLocalZOrder(1);
    };
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Item::setEvent(ItemMoveDelegate *cardEvent){
    this->cardEvent = cardEvent;
}
// Touch Event!

void Item::touchEvent(cocos2d::Touch* touch)
{
    CCLOG("touched : %s",getName().c_str());
}
