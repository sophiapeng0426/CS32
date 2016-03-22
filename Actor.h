#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//#include "GameConstants.h"

class StudentWorld;

///////////////////////////////////////base class
class BaseObject : public GraphObject
{
public:
    BaseObject(int sID, int sx, int sy, Direction sdir, double ssize, unsigned int sdepth,
               StudentWorld* sw);

    virtual ~ BaseObject(){}
    
    virtual void doSomething()=0;
    
    // need to implement: string for protesters/boulders/etc.
    virtual int getMyID(){return -1;}
    
    bool is_alive(){return m_alive;}
    
    void setDead(){m_alive = false;}
    
    StudentWorld* getWorld() const{return m_sw;}
    
    
private:
    StudentWorld* m_sw;
    bool m_alive;
//    int m_id;
};

///////////////////////////////////// Dirt class
class Dirt : public BaseObject
{
public:
    Dirt(int startX, int startY, StudentWorld* sw);
    ~Dirt(){};
    
    virtual void doSomething(){};
    
private:
};


/////////////////////////////////// non_distributed class

//class MovingObject:public BaseObject
//{
//public:
//    MovingObject(int IID, StudentWorld* sw,int startX, int StartY, Direction dir, int hitPoints);
//    virtual ~ MovingObject(){};
//    
//    virtual void doSomething(){};
//    
//    int getHitPoints(){return m_hitPoints;}
//    
//private:
//    int m_hitPoints;
//    
//    virtual bool is_valid_move(int x, int y, Direction dir);
//    
//};

class FrackMan : public BaseObject
{
public:
    FrackMan(StudentWorld* sw);
    ~FrackMan(){};
    
    virtual void doSomething();
    
    int getSquirt() const {return m_squirt;}
    int getSonar() const {return m_sonar;}
    int getGold() const {return m_gold;}
    int getHitPoints() const {return m_hitPoints;}
    int getMyOil() const {return m_oil;}
    
    void AddOil() {m_oil++;}
    void changeGold(int const a) {m_gold += a;}
    void changeSonar(int const a) {m_sonar += a;}
    void changeSquirt(int const a){m_squirt += a;}
    void changePoints(int const a) {m_hitPoints += a;}

    
private:
    int m_hitPoints;
    int m_squirt;
    int m_sonar;
    int m_gold;
    int m_oil;
    
    virtual bool is_valid_move(int x, int y, Direction dir);
};

//////////////////////Distributed class

//class StaticObject: public BaseObject
//{
//public:
//    StaticObject(int IID, StudentWorld* sw, int StartX, int StartY);
//    
//private:
//    
//};

class Oil: public BaseObject
{
public:
    
    Oil(StudentWorld* sw, int sx, int sy);
    virtual void doSomething();
    
private:
    bool is_found;
};

class Gold: public BaseObject
{
public:
    Gold(StudentWorld* sw, int sx, int sy);
    virtual void doSomething();

private:
    bool is_found;
//    bool is_dropped;
    
};

class Boulder: public BaseObject
{
public:
    Boulder(StudentWorld* sw, int sx, int sy);
    virtual void doSomething();

private:
    bool is_stable;
    int tickCount;
        
};
//class Gold: public StaticObject
//{
//public:
//    Gold(StudentWorld* sw, int sx, int sy, bool dropped);
//private:
//    bool is_drop; //dropped is visible by creation, pickup able by protester
//    
//};



#endif // ACTOR_H_
