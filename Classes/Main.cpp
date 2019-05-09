//
//  Main.cpp
//  MergeNumber-mobile
//
//  Created by Tech4 on 5/6/19.
//

#include "Main.hpp"
#include "TextureBlur.hpp"

USING_NS_CC;

Scene* Main::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Main::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Main::init()
{
    //////////////////////////////
    // 1. super init first
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
    
    auto lbl_score = Label::createWithTTF("0", "fonts/Arial.ttf", padding_score*0.5);
    lbl_score->enableOutline(Color4B::YELLOW,2);
    lbl_score->setPosition(origin + Vec2(padding_score,visibleSize.height/2));
    this->addChild(lbl_score);
    
    auto lbl_h_score = Label::createWithTTF("0", "fonts/Arial.ttf", home_item_bg->getContentSize().height);
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
        item->setPosition(board->getPosition() + Vec2((x - 1.5)*item->getContentSize().width + (x - 1.5)*padding,
                                                      size.height*0.1 + (y - 1.5)*item->getContentSize().height + (y - 1.5)*padding));
        this->addChild(item);
        list_item.push_back(item);
    }
    
    this->setOriginPostion(item_select->getPosition());
    
    return true;
}

void Main::onEventMove(int value,float x,float y,bool isMoveEnded){
    Vec2 pt = Vec2(x,y);
    int current_index = -1;
    for(Sprite* item : list_bg){
        if(item->getBoundingBox().containsPoint(pt)){
            item->setSpriteFrame("item_bg_highlight.png");
            current_index = item->getTag();
        }else{
            item->setSpriteFrame("item_bg.png");
        }
    }
    
    if(isMoveEnded){
        if(current_index < 0){
            item_select->runAction(MoveTo::create(0.2, this->getOriginPostion()));
        }else{
            createItem(current_index);
            item_select->setPosition(this->getOriginPostion());
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
        
        if(item1 != nullptr) list_item_neighbor.push_back(item1);
        if(item2 != nullptr) list_item_neighbor.push_back(item2);
        if(item3 != nullptr) list_item_neighbor.push_back(item3);
        if(item4 != nullptr) list_item_neighbor.push_back(item4);
        if(item5 != nullptr) list_item_neighbor.push_back(item4);
        if(item6 != nullptr) list_item_neighbor.push_back(item6);
        if(item7 != nullptr) list_item_neighbor.push_back(item7);
        if(item8 != nullptr) list_item_neighbor.push_back(item8);
    }
}

void Main::checkMerge(int index){
    findListItemNeighbor(index);
}

void Main::createItem(int index){
    if(isAdded(index)){
        return;
    }
    
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
    vector<int> list_index;
    for(Item* item : list_item){
        if(item->getValue() != -1){
            list_index.push_back(item->getValue());
        }
    }
    if(std::find(list_index.begin(), list_index.end(), index) != list_index.end()){
        return true;
    }
    return false;
}

void Main::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void Main::completionCallback(const std::string& name) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create(name);
    background->setScaleX(visibleSize.width/background->getContentSize().width);
    background->setScaleY(visibleSize.height/background->getContentSize().height);
    background->setContentSize(visibleSize);
    background->setPosition(origin + visibleSize/2);
    this->addChild(background);
}
