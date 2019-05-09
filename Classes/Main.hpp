//
//  Main.hpp
//  MergeNumber-mobile
//
//  Created by Tech4 on 5/6/19.
//

#ifndef Main_hpp
#define Main_hpp

#include "cocos2d.h"
#include "ItemMoveDelegate.h"
#include "Item.hpp"

USING_NS_CC;

class Main : public cocos2d::Layer,ItemMoveDelegate
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void onEventMove(int value,float x,float y,bool moved = false);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void completionCallback(const std::string& name);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Main);
    
    CC_SYNTHESIZE(cocos2d::Vec2, _originPostion, OriginPostion);
    
    void createItem(int index);
    void checkMerge(int index);
    void findListItemNeighbor(int index);
    Item* getItemOnBoard(int x,int y);
    bool isAdded(int index);
    void generateNextItemValue();
    int getCurMaxValueOnBoard();
    
    std::vector<Item*> list_item_neighbor;
    std::vector<Item*> list_item;
    std::vector<cocos2d::Sprite*> list_bg;
    
    Item* item_next;
    Item* item_select;
    cocos2d::Sprite* board;
    
    int currentValue = 2;
    int nextValue = 2;
    int maxRandomValue = 2;
};

#endif /* Main_hpp */
