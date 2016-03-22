#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include <vector>

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
    : GameWorld(assetDir){}
    
    virtual ~ StudentWorld();

    virtual int init();

    virtual int move();

    virtual void cleanUp();
    
    void setDisplayText();
    
    
    ///////////////////////////////    position functions /////////////////
    bool clearDirt(int x, int y);
    // cleaned dirt within (x,y),(x+3,y+3),
    // return true if dirt exists and cleaned, false if no dirt exist
    
    int is_covered(int const x, int const y);
    // return 1 if all covered by dirts;
    // return 0 if clean;
    // return 2 if else
    
    bool DirtUnder(int const x, int const y);
    
    bool is_frankMan_near(int const x, int const y, int const dis);
    
    /////////////////////////////  change private function status /////////////////
    void AddPlayerOil(){player->AddOil();}
    void changePlayerGold(int n){player->changeGold(n);}

    
    
    
    
    
    
    
    
    /////////////////////////// debug functions ///////////////
    void debug_dirt();
    void debug_frankman();
 

private:
    FrackMan* player;
    Dirt* dirtArray[64][64];
    std::vector<BaseObject*> m_actor;
    int nOilStart;
    // store distributed object
    
    
    std::string formatString(int sc, int le, int li, int heal, int sq, int go, int son, int barleft)
    {
        //used by setDisplayText()
        std::string status = "Scr:" + std::to_string(sc)+ "Lvl: " + std::to_string(le)+ "Lives: " + std::to_string(li) + "Hlth: " + std::to_string(heal) + "Water: " + std::to_string(sq) +  "Gold: " + std::to_string(go)+ "Sonar: " + std::to_string(son) + "Oil Left: " + std::to_string(barleft);
        return status;
    }
    /////////////////////////get status ////////////////////
    
    int getHealth() const
    {
        return (10 - player->getHitPoints());
    }
    int getSquirtsLeftInSquirtGun() const {return player->getSquirt();}
    int getPlayerGoldCount() const {return player->getGold();}
    int getPlayerSonarChargeCount() const {return player->getSonar();}
    int getNumberOfBarrelsRemainingToBePickedUp() const
    {
        return (nOilStart - player->getMyOil());
    }

//    int getnOil() const {return nOilStart;}
    
    ///////////////////////position functions
    
    int randInt(int lowest, int highest);
    
    //generate valid x,y and add to vector
    // 0 for oil;
    void distributeObject(char category);
    
    bool is_near(int x,int y);
    // return true of there is distributed object within 6 distance of (x,y)
    // only for distribution
   
    int is_mineShaft(int x, int y);
    //return 0 if oil
    //return 1 if minshaft
    //return 2 if upper space
    
    bool withinDis(int const x1, int const y1, int const x2, int const y2, int limit);
    // return false if ourside radius of limit
    // return true if within limit
    
    void removeDeadObject();
    
};

#endif // STUDENTWORLD_H_
