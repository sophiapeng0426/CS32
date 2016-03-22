//
//  testMapMemoryLeak.cpp
//  Project2
//
//  Created by Cheng Peng on 1/24/16.
//  Copyright © 2016 Cheng Peng. All rights reserved.
//

/*

#include "Map.h"
#include <iostream>

using namespace std;

void test()
{
    cerr << "test A" << endl;
    Map m;
    cerr << "test B" << endl;
    m.insert("10", IntWrapper(10));
    cerr << "test C" << endl;
    m.insert("20", IntWrapper(20));
    cerr << "test D" << endl;
    Map m2;
    cerr << "test E" << endl;
    m2.insert("30", IntWrapper(30));
    cerr << "test F" << endl;
    m2 = m;
    cerr << "test G" << endl;
    m2.insert("40", IntWrapper(40));
    cerr << "test H" << endl;
}

int main()
{
    test();
    cerr << "DONE" << endl;
}

*/