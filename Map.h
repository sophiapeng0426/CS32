//
//  Map.hpp
//  Project2
//
//  Created by Cheng Peng on 1/23/16.
//  Copyright © 2016 Cheng Peng. All rights reserved.
//

#ifndef Map_hpp
#define Map_hpp

//#include <iostream>
#include <string>
//#include "IntWrapper.h"       // TODO: remove this line after testing

//using namespace std;
typedef std::string KeyType;
//typedef int KeyType;
//typedef IntWrapper ValueType; // TODO: replace this line after testing
typedef double ValueType;
//typedef std::string ValueType;

class Map
{
public:
    Map();         // Create an empty map (i.e., one with no key/value pairs)
    
    
    Map(const Map&src);     // define copy constructor
    
    Map &operator=(const Map &src);   //define assignment operator
    
//    void dump() const;
    
    ~Map(); // define destructor
    
    bool empty() const;  // Return true if the map is empty, otherwise false.
    
    int size() const;    // Return the number of key/value pairs in the map.
    
    bool insert(const KeyType& key, const ValueType& value);
    // If key is not equal to any key currently in the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map, or the map has a fixed
    // capacity and is full).
    
    bool update(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map and the map has a fixed
    // capacity and is full).

    bool contains(const KeyType& key) const;
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    
    bool erase(const KeyType& key);
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    
    bool get(const KeyType& key, ValueType& value) const;
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    
    bool get(int i, KeyType& key, ValueType& value) const;
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.  (See below for details about this function.)
    
    void swap(Map& other);
    // Exchange the contents of this map with the other one.
    

private:
    struct Node
    {
        KeyType Key;
        ValueType Value;
        Node *next,*prev;
    };
    
    Node *head, *tail;
};


bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif /* Map_hpp */