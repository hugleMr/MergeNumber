//
//  MauBinhCardSprite.hpp
//  MyCards
//
//  Created by hungle on 8/1/16.
//
//

#ifndef Item_hpp
#define Item_hpp

#include "cocos2d.h"
#include "ItemMoveDelegate.h"

using namespace std;
using namespace cocos2d;

struct PosIndex {
    int x;
    int y;
};

class Item : public cocos2d::Sprite {
public:

    static Item* createCard(int value,ItemMoveDelegate* cardEvent);
    static Item* createItemView(int index,int value);
    virtual bool init() override;
    void onEnter() override;
    void addEvents();
    void setEvent(ItemMoveDelegate *cardEvent);
    void touchEvent(cocos2d::Touch* touch);
    
    void addNumber();
    void upgradeValue();
    void updateItem();
    
    void appear(bool isAppear);
    
    void update();
    
    CREATE_FUNC(Item);
    
    CC_SYNTHESIZE(PosIndex, _posIndex, PosIndex);
    CC_SYNTHESIZE(int, _value, Value);
    CC_SYNTHESIZE(int, _index, Index);
    
private:
    bool isAppear = false;
    Label* number;
    ItemMoveDelegate *cardEvent;
    cocos2d::Point touchOffset;
    
public:
    
    float getWidth();
    float getHeight();
    cocos2d::Size getSize();
};

#endif /* Item_hpp */
