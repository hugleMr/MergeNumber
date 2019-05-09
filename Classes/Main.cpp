//
//  Main.cpp
//  MergeNumber-mobile
//
//  Created by Tech4 on 5/6/19.
//

#include "Main.hpp"
#include "TextureBlur.hpp"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

Scene* Main::createScene()
{
    auto scene = Scene::create();
    auto layer = Main::create();
    scene->addChild(layer);
    return scene;
}

bool Main::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprites.plist");
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    std::string fileName = "background.png";
    auto background = Sprite::create(fileName);
    background->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    background->setScaleX(visibleSize.width/background->getContentSize().width);
    background->setScaleY(visibleSize.height/background->getContentSize().height);
    background->setContentSize(visibleSize);
    background->setPosition(origin);
    this->addChild(background);
    
    board = Sprite::create("board.png");
    board->setPosition(origin + visibleSize/2);
    this->addChild(board);
    
    auto size = board->getContentSize();
    for(int i =0 ;i < 16;i ++){
        auto x = i%4;
        auto y = i/4;
        auto item_bg = Sprite::createWithSpriteFrameName("item_bg.png");
        item_bg->setTag(i);
        float padding = size.width/5 - 4*item_bg->getContentSize().width/5;
        item_bg->setPosition(board->getPosition() + Vec2((x - 1.5)*item_bg->getContentSize().width + (x - 1.5)*padding,
                                  size.height*0.1 + (y - 1.5)*item_bg->getContentSize().height + (y - 1.5)*padding));
        this->addChild(item_bg);
        list_bg.push_back(item_bg);
    }
    
    auto home_item_bg = Sprite::create("home_item.png");
    home_item_bg->setPosition(origin + Vec2(visibleSize.width/2,home_item_bg->getContentSize().height*0.6));
    this->addChild(home_item_bg);
    
    float padding_score = visibleSize.width/4 - board->getContentSize().width/4;
    
    if(padding_score*0.5 > home_item_bg->getContentSize().height){
        padding_score = home_item_bg->getContentSize().height*2;
    }
    
    lbl_score = Label::createWithTTF("0", "fonts/Arial.ttf", padding_score*0.5);
    lbl_score->enableOutline(Color4B::YELLOW,2);
    lbl_score->setPosition(origin + Vec2(padding_score,visibleSize.height/2));
    this->addChild(lbl_score);
    
    lbl_h_score = Label::createWithTTF(StringUtils::format("%d",getHighScore()), "fonts/Arial.ttf", home_item_bg->getContentSize().height);
    lbl_h_score->enableOutline(Color4B::YELLOW,2);
    lbl_h_score->setPosition(origin + Vec2(visibleSize.width - padding_score,visibleSize.height/2));
    this->addChild(lbl_h_score);
    
    item_next = Item::createItemView(-1,nextValue);
    item_next->setScale(0.8);
    item_next->setPosition(home_item_bg->getPosition() - Vec2(home_item_bg->getContentSize().width,0));
    this->addChild(item_next);
    
    item_select = Item::createCard(currentValue,this);
    item_select->setPosition(home_item_bg->getPosition());
    this->addChild(item_select);
    
    for(int i = 0; i < 16; i ++){
        int x = i%4;
        int y = i/4;
        auto item = Item::createItemView(i,-1);
        item->setTag(i + 100);
        float padding = size.width/5 - 4*item->getContentSize().width/5;
        item->setPosition(board->getPosition() +
                          Vec2((x - 1.5)*item->getContentSize().width + (x - 1.5)*padding,
                               size.height*0.1 + (y - 1.5)*item->getContentSize().height + (y - 1.5)*padding));
        this->addChild(item);
        list_item.push_back(item);
    }
    
    this->setOriginPostion(item_select->getPosition());
    
    return true;
}

int Main::getHighScore(){
    return UserDefault::getInstance()->getIntegerForKey("h_score",0);
}

void Main::updateHighScore(){
    if(score > this->getHighScore()){
        lbl_h_score->setString(StringUtils::format("%d",score));
        UserDefault::getInstance()->setIntegerForKey("h_score",score);
    }
}

void Main::onEventMove(int value,float x,float y,bool isMoveEnded){
    Vec2 pt = Vec2(x,y);
    int current_index = -1;
    for(Sprite* item : list_bg){
        if(item->getBoundingBox().containsPoint(pt)){
            item->setSpriteFrame("item_bg_highlight.png");
            current_index = item->getTag();
            break;
        }else{
            item->setSpriteFrame("item_bg.png");
        }
    }
    
    if(isMoveEnded){
        countMerge = 0;
        if(current_index < 0){
            item_select->runAction(MoveTo::create(0.2, this->getOriginPostion()));
        }else{
            CCLOG("current index : %d",current_index);
            item_select->setPosition(this->getOriginPostion());
            
            if(isAdded(current_index)){
                return;
            }
            createItem(current_index);
            checkMerge(current_index);
        }
    }
}

Item* Main::getItemOnBoard(int x,int y){
    return x >= 4 || x < 0 ? nullptr : y >= 4 || y < 0 ? nullptr : this->list_item.at(y*4 + x);
}

void Main::findListItemNeighbor(int index){
    Item* item = nullptr;
    for(Item* _item : list_item){
        if(_item->getIndex() == index){
            item = _item;
            break;
        }
    }
    
    list_item_neighbor.clear();
    
    if(item != nullptr){
        //====TODO
        
        int x = item->getIndex()%4;
        int y = item->getIndex()/4;
        
        auto item1 = getItemOnBoard(x + 1,y);
        auto item2 = getItemOnBoard(x - 1,y);
        auto item3 = getItemOnBoard(x,y + 1);
        auto item4 = getItemOnBoard(x,y - 1);
        auto item5 = getItemOnBoard(x + 1,y - 1);
        auto item6 = getItemOnBoard(x + 1,y + 1);
        auto item7 = getItemOnBoard(x - 1,y - 1);
        auto item8 = getItemOnBoard(x - 1,y + 1);
        
        if(item1 != nullptr && item1->getValue() >= 0) list_item_neighbor.push_back(item1);
        if(item2 != nullptr && item2->getValue() >= 0) list_item_neighbor.push_back(item2);
        if(item3 != nullptr && item3->getValue() >= 0) list_item_neighbor.push_back(item3);
        if(item4 != nullptr && item4->getValue() >= 0) list_item_neighbor.push_back(item4);
        if(item5 != nullptr && item5->getValue() >= 0) list_item_neighbor.push_back(item5);
        if(item6 != nullptr && item6->getValue() >= 0) list_item_neighbor.push_back(item6);
        if(item7 != nullptr && item7->getValue() >= 0) list_item_neighbor.push_back(item7);
        if(item8 != nullptr && item8->getValue() >= 0) list_item_neighbor.push_back(item8);
    }
}

void Main::onMergeNeighborToCurItemCompleted(int index){
    auto item = list_item.at(index);
    item->stopAllActions();
    item->setScale(1);
    
    auto scale = ScaleBy::create(0.15, 1.075);
    item->runAction(Sequence::create(scale,scale->reverse(), NULL));
    
    countMerge++;
    if(countMerge > 3) countMerge = 3;
    SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("sounds/merge_%d.mp3",countMerge).c_str(),
                                                 false, 1.0f, 1.0f, 1.0f);
    
    item->upgradeValue();
    checkMerge(index);
}

void Main::checkMerge(int index){
    findListItemNeighbor(index);
    vector<Item*> list_merge;
    int value = list_item.at(index)->getValue();
    for(Item* item : list_item_neighbor){
        if(item->getValue() == value){
            list_merge.push_back(item);
        }
    }
    
    if(list_merge.size() == 0){
        return;
    }
    
    for(Item* item : list_merge){
        auto move = MoveTo::create(0.2, list_item.at(index)->getPosition());
        
        auto new_item = Item::createItemView(item->getIndex(), item->getValue());
        new_item->setPosition(item->getPosition());
        this->addChild(new_item);
        
        new_item->runAction(Sequence::create(move,RemoveSelf::create(), NULL));
        
        item->setValue(-1);
        item->updateItem();
    }
    
    auto score_bonus = ((int)list_merge.size())*value;
    score += score_bonus;
    lbl_score->setString(StringUtils::format("%d",score));
    this->updateHighScore();
    
    SimpleAudioEngine::getInstance()->playEffect("sounds/move.wav", false, 1.0f, 1.0f, 1.0f);
    
    auto delay = DelayTime::create(0.2 + 0.075);
    int index_ = index;
    auto callfunc = CallFunc::create([=](){
        this->onMergeNeighborToCurItemCompleted(index_);
    });
    this->runAction(Sequence::create(delay,callfunc, NULL));
    
    CCLOG("======");
}

void Main::createItem(int index){
    CCLOG("created!");
    
    list_item.at(index)->setValue(item_select->getValue());
    list_item.at(index)->updateItem();
    
    this->generateNextItemValue();
}

void Main::generateNextItemValue(){
    currentValue = nextValue;
    nextValue = RandomHelper::random_int(1, this->getCurMaxValueOnBoard());
    item_select->setValue(currentValue);
    item_select->updateItem();
    item_next->setValue(nextValue);
    item_next->updateItem();
}

int Main::getCurMaxValueOnBoard(){
    int max = 2;
    for(Item* item : list_item){
        if(item->getValue() > max){
            max = item->getValue();
        }
    }
    return max;
}

bool Main::isAdded(int index){
    if(list_item.at(index)->getValue() < 0){
        return false;
    }
    return true;
}
