#include "StudentWorld.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld:: ~StudentWorld()
{
    delete player;
    
    //for dirt
    for (int x = 0; x <= 63; x++) {
        for (int y = 0; y <= 63; y++) {
            delete dirtArray[x][y];
            }
        }

    for (int i = 0; i < m_actor.size(); i++) {
        BaseObject* temp = m_actor[i];
        m_actor[i] = m_actor[m_actor.size()-1];
        m_actor.pop_back();
        delete temp;
    }
}

int StudentWorld:: init()
{
    //Initialize the data structures dirt and frackman
    player = new FrackMan(this);
    
    for (int x = 0; x <= 63; x++) {
        for (int y = 0; y <= 63; y++) {
            if (!(is_mineShaft(x, y)==0)) {
                dirtArray[x][y] = nullptr;
            }
            else
                dirtArray[x][y] = new Dirt(x,y,this);
        }
    }
    
    //Initialize ds for distributed object
    int level = getLevel();
    int nBouldersStart = min(level/ 2 + 2, 6);
    int nGoldStart = max(5 - level/ 2, 2);
    nOilStart =  min(2 + level, 20);
    
    for (int i = 0; i < nOilStart + nBouldersStart + nGoldStart; i++)
    {
        if (i < nOilStart)
        {
            distributeObject('o');
        }
        else if (i>= nOilStart && i < nOilStart + nBouldersStart)
        {
            distributeObject('b');
        }
        else
        {
            distributeObject('g');
        }
    }
    
//    cerr << "finish init" << endl;
//    cerr << m_actor.size() << endl;
    
    return GWSTATUS_CONTINUE_GAME;
}




int StudentWorld:: move()
{
    //update the status text
    setDisplayText();
    
    //do something
    if (player->is_alive()) {
        player->doSomething();
        
        for (int i = 0; i < m_actor.size(); i++) {
            m_actor[i]->doSomething();
            
            if (!player->is_alive()) {
                break;
            }
            
            if (nOilStart == player->getMyOil()) {
                break;
                
            }
        }
//
//        // remove newly dead object
        removeDeadObject();
        
        if (!player->is_alive()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        
        if (player->getMyOil()==nOilStart) {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
        
        return GWSTATUS_CONTINUE_GAME;
    }
    else
    {
        //player dead
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
}

void StudentWorld::removeDeadObject()
{
    int i = 0;
    while (i < m_actor.size()) {
        
        if (!m_actor[i]->is_alive()) {
            delete m_actor[i];
            m_actor[i] = m_actor[m_actor.size()-1];
            m_actor.pop_back();
        }
        else
            i++; // important!
    }
}

void StudentWorld:: cleanUp()
{
    delete player;
    player = nullptr;
    
    //for dirt
    for (int x = 0; x <= 63; x++) {
        for (int y = 0; y <= 63; y++) {
            if (dirtArray[x][y]!= nullptr) {
                delete dirtArray[x][y];
                dirtArray[x][y] = nullptr;
            }
        }
    }
    
    // for m_actor
    int i = 0;
    while (i<m_actor.size()) {
        delete m_actor[i];
        m_actor[i] = m_actor[m_actor.size()-1];
        m_actor.pop_back();
        
    }
//    cerr << "after clean up m_acotr size: "<< m_actor.size() <<endl;
    
}




////////////////////// position function /////////////////////////

bool StudentWorld::clearDirt(int x, int y){
    // cleaned dirt within (x,y),(x+3,y+3),
    // return true if dirt exists and cleaned, false if no dirt exist
    int n_erase = 0;
    
    for (int i = x; i <= x+3; i++) {
        for (int j = y; j <= y+3; j++) {
            if (dirtArray[i][j]!=nullptr) {
                delete dirtArray[i][j];
                dirtArray[i][j] = nullptr;
                n_erase++;
            }
        }
    }
    
    if (n_erase>0) {
        return true;
    }
    else return false;
}


int StudentWorld::is_covered(int const x, int const y){
    int count = 0;
    
    for (int i = x; i<=x+3; i++) {
        for (int j= y; j<=y+3; j++) {
            if(dirtArray[i][j]!=nullptr)
                count++;
        }
    }
    if (count==0) {
        return 0;
    }
    else if (count == 16){
        return 1;
    }
    else
        return 2;
}


bool StudentWorld::DirtUnder(const int x, const int y)
{
    for (int i = x; i < x + 4; i++) {
        if (dirtArray[i][y-1]!=nullptr) {
            return true;
        }
    }
    return false;
}



bool StudentWorld::is_frankMan_near(int const x, int const y, int const dis)
{
    int fx = player->getX();
    int fy = player->getY();

    return withinDis(x, y, fx, fy, dis);
}


//////// auxilary function
void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = getHealth();
    int squirts = getSquirtsLeftInSquirtGun();
    int gold = getPlayerGoldCount();
    int sonar = getPlayerSonarChargeCount();
    int barrelsLeft = getNumberOfBarrelsRemainingToBePickedUp();
    // Next, create a string from your statistics, of the form:
    // “Scr: 0321000 Lvl: 52 Lives: 3 Hlth: 80% Water: 20 Gld: 3 Sonar: 1 Oil Left: 2”
    string s = formatString(score, level, lives, health, squirts, gold, sonar, barrelsLeft);
    // Finally, update the display text at the top of the screen with your // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}



////////////////////////private function//////////////////

bool StudentWorld::withinDis(int const x1, int const y1, int const x2, int const y2, int limit)
{
    int distanceSquare = (pow(x1-x2,2)+ pow(y1-y2, 2));
    int limitSqaure = pow(limit,2);
    if (distanceSquare > limitSqaure) {
        return false;
    }
    else return true;
}


bool StudentWorld::is_near(int x,int y)
{
    //loop through m_actor
    for (int i = 0; i<m_actor.size(); i++) {
        int m_x = m_actor[i]->getX();
        int m_y = m_actor[i]->getY();
        int distance = (pow(m_x-x,2)+ pow(m_y-y, 2));
        if (distance <= 36) {
            return true;
        }
    }
    return false;
}


int StudentWorld::is_mineShaft(int x, int y)
{
    if (x>=30 && x<=33 && y>=4 && y<=59)
        return 1;
    else if (y>=60)
        return 2;
    else
        return 0;
}


int StudentWorld::randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest); //organize the numbers
    return lowest + (rand() % (highest - lowest + 1)); //return the corresponding random integer
}

void StudentWorld::distributeObject(char category)
{
    int validx = 0;
    int validy = 0;
    bool is_valid = false;
    
    while (is_valid == false) {
        int tempx = randInt(0, 60);
        int tempy = randInt(0, 56);
        
        int in_oil = is_covered(tempx, tempy);
        if (in_oil==1) {
            if (is_near(tempx, tempy)==false) {
                is_valid = true;
                validx = tempx;
                validy = tempy;
            }
        }
    }
    
    if (category == 'o') {
        BaseObject* p = new Oil(this,validx, validy);
        m_actor.push_back(p);
    }
    if (category == 'b') {
        BaseObject* p = new Boulder(this,validx, validy);
        m_actor.push_back(p);
    }
    if (category == 'g') {
        BaseObject* p = new Gold(this,validx, validy);
        m_actor.push_back(p);
    }
    
}



//////////////////////// debug function /////////////////////
void StudentWorld::debug_dirt()
{
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 60; y++) {
            if (!(x>=30 && x<=33 && y>=4 && y<=59)) {
                Dirt * p= dirtArray[x][y];
                cerr << p->getX() << p->getY() << 'l' << endl;
            }
        }
    }
}

void StudentWorld::debug_frankman()
{
    FrackMan * f= player;
    cerr << f->getX() << endl;
    cerr << f->getY() << endl;
    
    cerr << f->is_alive() << endl;
    cerr << f->getDirection() << endl;
    cerr << f->getSize() << endl;
    
    
}
