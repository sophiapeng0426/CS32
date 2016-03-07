//
//  DiskMultiMap.cpp
//  Project4
//
//  Created by Cheng Peng on 3/5/16.
//  Copyright © 2016 Cheng Peng. All rights reserved.
//

#include "DiskMultiMap.h"
#include "BinaryFile.h"
#include <iostream>
#include <functional>
#include <string.h>


DiskMultiMap::DiskMultiMap()
{
    freeBlockBegin = sizeof(unsigned int) + sizeof(BinaryFile::Offset);
    bucketBegin = sizeof(Header);
    freeHeadptr = sizeof(unsigned int);
    m_bucketsize = MAXBUCKETSIZE;
    
//    cerr << "freeheaderptr " << freeHeadptr <<endl;
//    cerr << "FreeBlockbegin " << freeBlockBegin <<endl;
//    cerr << "bucketbegin " << bucketBegin <<endl;
    
}

DiskMultiMap::~DiskMultiMap()
{
    m_file.close();
}


bool DiskMultiMap::createNew(const std::string &filename, unsigned int numBuckets)
{
    close();
    
    m_file.createNew(filename);
   
    if (numBuckets > MAXBUCKETSIZE) {
        cerr << "Bucket size exceeds max allowed, increase Max bucketsize" << endl;
        return false;
    }
    
    m_bucketsize = numBuckets;
    
    // Header
    Header m_header;
    m_header.bucketsize = numBuckets;
    m_header.freeHead = 0;
    m_file.write(m_header, 0);
    
    // BinaryFile::Offset freeBlock_begin = sizeof(unsigned int) + sizeof(BinaryFile::Offset)
    
    // Bucket
    BinaryFile::Offset m_bucket[MAXBUCKETSIZE];
    for (int i = 0; i<numBuckets; i++) {
        m_bucket[i] = 0; //initialize empty bucket
    }
    
    m_file.write(m_bucket,bucketBegin);
    
    return true;
}


bool DiskMultiMap::openExisting(const std::string &filename)
{
    close();
    
    bool success = m_file.openExisting(filename);
    //update bucketsize
    if (success) {
        unsigned int bucketsize;
        m_file.read(bucketsize, 0);
        m_bucketsize = bucketsize;
    }
    return success;
}

void DiskMultiMap::close()
{
    if (m_file.isOpen()) {
        m_file.close();
    }
}


bool DiskMultiMap::insert(const std::string &key, const std::string &value, const std::string &context)
{
    if (key.size()>120 || value.size()>120 || context.size()>120) {
        return false;
    }
    
    const char* mkey = key.c_str();
    const char* mvalue = value.c_str();
    const char* mcontext = context.c_str();
    
    
    BinaryFile::Offset bucketcode = hashCode(key,m_bucketsize);
    BinaryFile::Offset bucketptr;
    m_file.read(bucketptr, bucketBegin+bucketcode);
    
    if (bucketptr == 0) {//empty bucket
        DiskNode newNode(mkey,mvalue,mcontext,0);
        BinaryFile::Offset insertPosition = acquireNode();
        m_file.write(newNode, insertPosition);
        m_file.write(insertPosition,bucketBegin+bucketcode);
        
    }
    else{// insert front
        BinaryFile::Offset oldhead = bucketptr;
        DiskNode newNode(mkey,mvalue,mcontext,oldhead);
        BinaryFile::Offset insertPosition = acquireNode();
        m_file.write(newNode,insertPosition);
        m_file.write(insertPosition,bucketBegin+bucketcode);
    }
    return true;
}




DiskMultiMap::Iterator DiskMultiMap::search(const std::string &key)
{
    BinaryFile::Offset bucketcode = hashCode(key,m_bucketsize);
    BinaryFile::Offset bucketptr;
    m_file.read(bucketptr, bucketBegin+bucketcode);
    
    if (bucketptr==0) {
        return Iterator();
    }
    
    vector<MultiMapTuple> match;
    BinaryFile::Offset curr;
    curr = bucketptr;
    
    while (curr!=0) {
        DiskNode temp;
        m_file.read(temp, curr);
        if (strcmp(temp.key,key.c_str())==0) {
            MultiMapTuple newfind;
            newfind.key = key;
            newfind.value = temp.value;
            newfind.context = temp.context;
            match.push_back(newfind);
        }
        curr = temp.next;
    }
    
    if (match.size()==0) {
        return Iterator();
    }
    
    return Iterator(key,match);
    
}


//************ iterator functions **************//

DiskMultiMap::Iterator::Iterator()
{
    std::string m_key = "";
}

DiskMultiMap::Iterator::Iterator(std::string key, vector<MultiMapTuple> res)
{
    m_key = key;
    m_tuple = res;
    m_iter = 0; //
}

//DiskMultiMap::Iterator::Iterator(const Iterator& other)
//{
//    m_key = other.m_key;
//    m_tuple = other.m_tuple;
//    m_iter = other.m_iter;
//    
//}

bool DiskMultiMap::Iterator::isValid() const
{
    if (m_key.size()==0) {
        return false;
    }
    if (m_tuple.size()==0) {
        return false;
    }
    if (m_iter >= m_tuple.size()) {
        return false;
    }
    return true;
}



DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++()
{
    
    if (isValid()) {
        m_iter += 1;
        if (isValid()) {
            return *this;
        }
    }
    return *this;
}

MultiMapTuple DiskMultiMap::Iterator::operator*()
{
    MultiMapTuple s;
    if (!isValid()) {
        s.key = "";
        s.value = "";
        s.context = "";
    }
    else{
        s.key = m_tuple[m_iter].key;
        s.value = m_tuple[m_iter].value;
        s.context = m_tuple[m_iter].context;
    }
    return s;
    
}




// ****************** private functions ******************//

BinaryFile::Offset DiskMultiMap::hashCode(std::string s, int bucketsize)
{
    return str_hash(s) % bucketsize;
}



BinaryFile::Offset DiskMultiMap::acquireNode()
{
    BinaryFile::Offset mfreeHead;
    m_file.read(mfreeHead, freeHeadptr);
    
    BinaryFile::Offset insertPosition;
    
    if (mfreeHead == 0) {
        insertPosition = m_file.fileLength();
    }
    else{
        m_file.read(insertPosition, freeBlockBegin+mfreeHead-1);
        m_file.write(mfreeHead-1, freeHeadptr);
        //        cerr << "Reuse space " << insertPosition << endl;
    }
    
    return insertPosition;
}


