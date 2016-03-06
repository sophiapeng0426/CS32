//
//  DiskList.hpp
//  Project4
//
//  Created by Cheng Peng on 3/5/16.
//  Copyright © 2016 Cheng Peng. All rights reserved.
//

#ifndef DiskList_hpp
#define DiskList_hpp
#include <string>
#include "BinaryFile.h"

int const SIZEOFFREE = 1000;

class DiskList
{
public:
    DiskList(const std::string& filename);
    bool push_front(const char* data);
    bool remove(const char* data);
    void printAll();
    
    
private:
    BinaryFile m_file;
    BinaryFile::Offset m_head;
    int m_size;
    BinaryFile::Offset freeBlock[SIZEOFFREE];
    BinaryFile::Offset freeHead;
    
    struct DiskNode
    {
        DiskNode(const char* data, BinaryFile::Offset next)
        :next(next)
        {
            strcpy(value,data);
        }
        char value[256];
        BinaryFile::Offset next;
        
    };
    
    bool strcompare(char arr[256],const char* data);
    
};

#endif /* DiskList_hpp */
