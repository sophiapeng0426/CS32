//
//  Map.cpp
//  Project2
//
//  Created by Cheng Peng on 1/23/16.
//  Copyright © 2016 Cheng Peng. All rights reserved.
//

#include "Map.h"
#include <iostream>

using namespace std;

//contructor
Map::Map()
{
    head = nullptr;
    tail = nullptr;
}

Map::Map(const Map&src)
{
    if (src.head == nullptr) {
        head = nullptr;
        tail = nullptr;
        return;
    }
    
    head = nullptr;
    tail = nullptr;
    
    Node *p; //iterator
    p = src.head;
    
    while (p!=nullptr) {
        insert(p->Key, p->Value);
        p = p->next;
    }
    
}

Map& Map::operator=(const Map &src)
{
    if (&src == this) {
        return(*this);
    }
    
    if (head !=nullptr) {
        Node *p;
        Node *n;
        p = head;
        
        while (p!=nullptr) {
            n = p->next;
            delete p;
            p = n;
        }
    }
    
    head = nullptr;
    tail = nullptr;
    
    Node *p; //iterator
    p = src.head;
    
    while (p!=nullptr) {
        insert(p->Key, p->Value);
        p = p->next;
    }
    return(*this);
}

Map::~Map()
{
    Node *p;
    Node *n;
    p = head;
    
    while (p!=nullptr) {
        n = p->next;
        delete p;
        p = n;
    }
}

bool Map::empty() const
{
    return size()==0;
}

int Map::size() const
{
    int i=0;
    Node *p;
    p = head;
    while (p != nullptr) {
        p = p->next;
        i ++;
    }
    return i;
}

bool Map::insert(const KeyType &key, const ValueType &value)
{
    // If key is not equal to any key currently in the map, then do so and return true.
    // Otherwise, make no change to the map and return false.
    
    if (contains(key)) {
        return false;
    }
    
    // always insert to tail
    Node *newNode = new Node;
    newNode->Key = key;
    newNode->Value = value;
    newNode->next = nullptr;
    
    if (size() ==0) {
        head = newNode;
        tail = newNode;
        head->prev = nullptr;  // don't forget
        tail->next = nullptr;  // don't forget
        return true;
    }
    
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
    return true;
}

bool Map::update(const KeyType &key, const ValueType &value)
{
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    if (size()==0) {
        return false;
    }
    
    Node * p;
    p = head;
    
    while (p!=nullptr) {
        if (p->Key == key) {
            p->Value = value;
            return true;
        }
        p = p->next;
    }
    return false;
}


bool Map::insertOrUpdate(const KeyType &key, const ValueType &value)
{
    if (update(key, value))
        return true;
    
    insert(key,value);
    return true;
}



bool Map::contains(const KeyType &key) const
{
    // Return true if key is equal to a key currently in the map,
    // otherwise false.

    Node *p;
    p = head;
    
    if (p==nullptr) {
        return false;
    }
    
    while (p!=nullptr) {
        if (p->Key == key) {
            return true;
        }
        p = p->next;
    }
    
    return false;
}

bool Map::erase(const KeyType &key)
{
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
  
    if (size()==0) {
        return false;
    }
    
    
    if (head->Key == key && size()!=1) {
        Node *p;
        p = head;
        head = head->next;
        delete p;
        head->prev = nullptr;
        
        return true;
    }
    
    if (tail->Key == key && size()!=1) {
        Node *p;
        p = tail;
        
        tail = tail->prev;
        delete p;
        tail->next = nullptr;
        
        return true;
    }
    
    if (head->Key==key && size()==1) {
        Node *p;
        p = head;
        head = nullptr;
        tail = nullptr;
        delete p;
        
        return true;
    }
    
    
    Node *p;
    p = head;
    
    while (p->next!=nullptr) {
       
        if (p->Key==key) {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
            return true;
        }
        p = p->next;
    }
    
    return false;
}



bool Map::get(const KeyType& key, ValueType& value) const
{
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    
    Node *p;
    p = head;
    
    while (p!= nullptr){
        if (p->Key == key) {
            value = p->Value;
            return true;
        }
        p = p->next;
    }
   
    return false;
}


bool Map::get(int i, KeyType& key, ValueType& value) const
{
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.
    
    if (i > size()-1) {
        return false;
    }
    
    if(size()==0)
        return false;
    
    int count = 0;
    Node *p;
    p = head;
    
    while (count < i) {
        p = p->next;
        count ++;
    }
    
    key = p->Key;
    value = p->Value;
    return true;
    
}


//void Map::dump() const{
//    Node *p;
//    p = head;
//    
//    while (p!=nullptr) {
//        cerr<< p->Key << ' ' << p->Value << endl;
//        p = p->next;
//    }
//}



void Map::swap(Map &other)
{
    // Exchange the contents of this map with the other one.
    Node *temp_h;
    temp_h = other.head;
    other.head = head;
    head = temp_h;
    
    Node *temp_t;
    temp_t = other.tail;
    other.tail = tail;
    tail = temp_t;
}


// non member functions

bool combine(const Map& m1, const Map& m2, Map& result)
{
    Map temp2(m2);//get a copy of m2
    
    result = m1;
    int key_conflict = 0;
    
    for (int i=0; i < temp2.size(); i++) {
        KeyType k;
        ValueType v;
        temp2.get(i,k,v);
        
        if (!m1.contains(k)) {
            result.insert(k,v);
        }
        else{
            ValueType v1;
            m1.get(k, v1);
            if (!(v1==v)) {
                result.erase(k);
                key_conflict ++;
            }
        }
        
    }
    
    if (key_conflict > 0) {
        return false;
    }
    
    return true;
}


void subtract(const Map& m1, const Map& m2, Map& result)
{
    Map temp2(m2);//get a copy of m2
    
    result = m1;
    for (int i=0; i < temp2.size(); i++) {
        KeyType k;
        ValueType v;
        temp2.get(i,k,v);
        
        if (m1.contains(k)) {
            result.erase(k);
        }
    }
}


























