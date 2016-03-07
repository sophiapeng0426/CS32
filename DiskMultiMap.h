//
//  DiskMultiMap.hpp
//  Project4
//
//  Created by Cheng Peng on 3/5/16.
//  Copyright © 2016 Cheng Peng. All rights reserved.
//

#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"
#include <vector>

const int MAXBUCKETSIZE = 1000;
const int FREEBUFFERSIZE = 1000;

class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        Iterator();
        // You may add additional constructors
        Iterator(std::string key, vector<MultiMapTuple> res);
        
//        Iterator(const Iterator& other);
        
        bool isValid() const;
        Iterator& operator++();
        MultiMapTuple operator*();
        
    private:
        // Your private member declarations will go here
        std::string m_key;
        vector<MultiMapTuple> m_tuple;
        int m_iter;
    };
    
    DiskMultiMap();
    ~DiskMultiMap();
    bool createNew(const std::string& filename, unsigned int numBuckets);
    bool openExisting(const std::string& filename);
    void close();
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
    //helper function (need to delete)
//    void printAll();
    
private:
    // Your private member declarations will go here
    BinaryFile m_file;
    BinaryFile::Offset freeBlockBegin;
    BinaryFile::Offset bucketBegin;
    BinaryFile::Offset freeHeadptr;
    
    unsigned int m_bucketsize;

    struct Header
    {
        unsigned int bucketsize;
        BinaryFile::Offset freeHead;
        BinaryFile::Offset freeBlock[FREEBUFFERSIZE];
    };
    
//    BinaryFile::Offset m_bucket[BUCKETSIZE]; //initialize to 0
    
    struct DiskNode
    {
        DiskNode(const char* skey, const char* svalue, const char* scontext, BinaryFile::Offset snext)
        :next(snext)
        {
            std::strcpy(key,skey);
            std::strcpy(value,svalue);
            std::strcpy(context, scontext);
        }
        
        DiskNode()
        {
            next = 0;
            std::strcpy(value, "");
            std::strcpy(value, "");
            std::strcpy(context, "");
        }
        
        char key[120+1];
        char value[120+1];
        char context[120+1];
        BinaryFile::Offset next;
        
    };
    
    // ****** private funtion ******* //
    
    BinaryFile::Offset acquireNode();
    
    std::hash<std::string> str_hash;
    BinaryFile::Offset hashCode(std::string s, int bucketsize);
    
    
};

#endif // DISKMULTIMAP_H_
