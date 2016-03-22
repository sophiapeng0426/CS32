#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
using namespace std;

///////////////////////base class

BaseObject::BaseObject(int sID, int sx, int sy, Direction sdir, double ssize, unsigned int sdepth,StudentWorld* sw):
GraphObject(sID,sx,sy,sdir,ssize,sdepth)
{
    setVisible(true);
    m_sw = sw;
    m_alive = true;
}

///////////////////////dirt class
Dirt::Dirt(int x, int y, StudentWorld* sw):
BaseObject(IID_DIRT,x,y,right,0.25,3,sw)
{
    setVisible(true);
}

//////////////////////Moving class
//MovingObject::MovingObject(int IID, StudentWorld* sw, int StartX, int StartY, Direction dir,
//                           int hitPoints):
//BaseObject(IID,StartX,StartY,dir,1.0,0,sw)
//{
//    m_hitPoints = hitPoints;
//}
//
//bool MovingObject::is_valid_move(int x, int y, Direction dir) //private
//{
//    return true;
//}

FrackMan::FrackMan(StudentWorld* sw):
BaseObject(IID_PLAYER,30,60,right,1.0,0,sw)
{
    m_squirt = 5;
    m_sonar = 1;
    m_gold = 0;
    m_hitPoints = 10;
    m_oil = 0;
}


bool FrackMan::is_valid_move(int x, int y, Direction dir) //private
{
    switch (dir) {
        case up:
            if (y+1<=60) return true;
            else return false;
            break;
        case right:
            if (x+1<=60) return true;
            else return false;
            break;
        case down:
            if (y-1>=0) return true;
            else return false;
            break;
        case left:
            if (x-1>=0) return true;
            else return false;
            break;
        default:
            return false;
            break;
    }
    return false;
}

void FrackMan::doSomething()
{
    if (!is_alive()) {
        return;
    }
    int cx,cy;
    cx = getX();
    cy = getY();
    
    if(!getWorld()->clearDirt(cx,cy)){
        int ch;
        Direction c_dir = getDirection();
        
        if (getWorld()->getKey(ch) == true){
            switch (ch) {
                case KEY_PRESS_ESCAPE:
                    setDead();
                    break;
                case KEY_PRESS_UP:
                    if (c_dir == up) {
                        if (is_valid_move(cx, cy, up)) {
                            moveTo(cx,cy+1);
                        }
                    }
                    else setDirection(up);
                    break;
                case KEY_PRESS_RIGHT:
                    if (c_dir == right) {
                        if (is_valid_move(cx, cy, right)) {
                            moveTo(cx+1,cy);
                        }
                    }
                    else setDirection(right);
                    break;
                    
                case KEY_PRESS_DOWN:
                    if (c_dir == down) {
                        if (is_valid_move(cx, cy, down)) {
                            moveTo(cx,cy-1);
                        }
                    }
                    else setDirection(down);
                    break;
                case KEY_PRESS_LEFT:
                    if (c_dir == left) {
                        if (is_valid_move(cx, cy, left)) {
                            moveTo(cx-1,cy);
                        }
                    }
                    else setDirection(left);
                    break;
                    
                default:
                    break;
            }
        }
        
        
    }
    else
        getWorld()->playSound(SOUND_DIG);
}

//////////////////////Distributed class

//StaticObject::StaticObject(int IID, StudentWorld* sw, int StartX, int StartY)
//:BaseObject(IID,StartX,StartY,right,1.0,2,sw)
//{
//}

Oil::Oil(StudentWorld* sw, int sx, int sy)
:BaseObject(IID_BARREL,sx,sy,right,1.0,2,sw)
{
    setVisible(false);
}


void Oil::doSomething()
{
    if (!is_alive()) {
        return;
    }
    
    int cx = getX();
    int cy = getY();
    
    if (getWorld()->is_frankMan_near(cx, cy, 3))
    {
        setDead();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->AddPlayerOil();
        return;
    }
    
    if (getWorld()->is_frankMan_near(cx, cy, 4))
    {
       //set visible
        setVisible(true);
        return;
    }
}




Gold::Gold(StudentWorld* sw, int sx, int sy)
:BaseObject(IID_GOLD,sx,sy,right,1.0,2,sw)
{
    setVisible(false);
}


void Gold::doSomething()
{
    if (!is_alive()) {
        return;
    }
    
    int cx = getX();
    int cy = getY();
    
    if (getWorld()->is_frankMan_near(cx, cy, 3))
    {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
        getWorld()->changePlayerGold(1);
        return;
        
    }
    
    if (getWorld()->is_frankMan_near(cx, cy, 4))
    {
        //visible
        setVisible(true);
        return;
    }
}






Boulder::Boulder(StudentWorld* sw, int sx, int sy)
:BaseObject(IID_BOULDER,sx,sy,down,1.0,1,sw)
{
    setVisible(true);
    getWorld()->clearDirt(sx, sy);
    is_stable = true;
    tickCount = 30;
}

void Boulder::doSomething()
{
//    if (!is_alive()) {
//        return;
//    }
//    
//    if (is_stable == true) {
//        int cx = getX();
//        int cy = getY();
//        if(!getWorld()->DirtUnder(cx, cy))
//            is_stable = false;
//    }
//    
//    if (is_stable == false && tickCount > 0) {
//        tickCount -= 1;
//        return;
//    }
    
    

}





























