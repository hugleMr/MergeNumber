//
//  ItemMoveDelegate.h
//
//
//  Created by hungle on 8/2/19.
//
//

#ifndef ItemMoveDelegate_h
#define ItemMoveDelegate_h

class ItemMoveDelegate{
    
public:
    virtual void onEventMove(int value,float x,float y,bool moved = false) = 0;
};


#endif /* ItemMoveDelegate_h */
