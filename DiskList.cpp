//
//  DiskList.cpp
//  Project4
//
//  Created by Cheng Peng on 3/5/16.
//  Copyright © 2016 Cheng Peng. All rights reserved.
//

#include "DiskList.h"
#include "BinaryFile.h"

#include <iostream>
using namespace std;

bool DiskList::strcompare(char arr[256],const char* data)
{
    string a = arr;
    string b = data;
    return a==b;
}

DiskList::DiskList(const string& filename)
{
    //constructor creates an empty list in a disk file of the specified name
    m_file.createNew(filename);
    m_head = 0; // indicate empty list
    m_size = 0;
    freeHead = 0; // no free space
    
    m_file.write(m_head, 0);
    m_file.write(freeBlock, sizeof(m_head)); //store free space
    
}

bool DiskList::push_front(const char *data)
{
    if (sizeof(data) >= 256) {
        return false;
    }
    
    //added to the front of the list, return true
    BinaryFile::Offset insertPosition;
    if (freeHead==0) {
        insertPosition = m_file.fileLength();
    }
    else{
        m_file.read(insertPosition, 2*sizeof(m_head)+(freeHead-1)*sizeof(m_head));
        freeHead -= 1;
        cerr << "Reuse space " << insertPosition << endl;
        cerr << "How many left " << freeHead << endl;
    }
    
    
    if (m_size==0) {
        // push to empty list
        DiskNode newNode(data,0);
        m_head = insertPosition;
        m_file.write(m_head,0); // update address for first node in head
        m_file.write(newNode,insertPosition);
    }
    else{
        //push to non-empty
        BinaryFile::Offset oldHead = m_head;
        DiskNode newNode(data,oldHead);
        m_head = insertPosition;
        m_file.write(m_head,0);
        m_file.write(newNode,insertPosition);
    }
    
    m_size += 1;
    return true;
}

bool DiskList::remove(const char* data)
{
    //every node whose C string is equal to data is removed from the list.
    if (m_size==0){
        return false;
    }
    
    // delete first node
    BinaryFile::Offset p;
    char name[256];
    DiskNode temp(name,p);
    
    int count = 0;
    while (m_head!=0) {
        m_file.read(temp, m_head);
        
        if (!strcompare(temp.value, data)) {
            break;
        }
        else{
            freeBlock[freeHead] = m_head;
            m_file.write(m_head, 2*sizeof(m_head)+ freeHead*sizeof(m_head));
            freeHead += 1;
            
            m_head = temp.next; // delete first node
            m_file.write(m_head,0); // move outside while loop?
            count += 1;
            m_size -= 1;
        }
    }
    
    
    if (m_size ==0 || m_size ==1) {
        return count > 0;

    }
    
    // in the middle
    BinaryFile::Offset curr = m_head;
    m_file.read(temp, m_head);
    BinaryFile::Offset next = temp.next;
    
    DiskNode tempnext(name,p);
    
    while (next!=0) {// not last node
        m_file.read(temp,curr);
        m_file.read(tempnext,next);
        
        if (strcompare(tempnext.value,data)) {
            freeBlock[freeHead] = next;
            m_file.write(next, 2*sizeof(m_head)+ freeHead*sizeof(m_head));
            freeHead += 1;
            
            temp.next = tempnext.next;
            m_file.write(temp,curr);
            m_size -= 1;
            count += 1;
            next = temp.next;
        }
        else{
            curr = next;
            next = tempnext.next;
        }
    }
    
    return count > 0;
    
}


void DiskList::printAll()
{
    if (m_size == 0) {
        return;
    }
    
    cerr << "-------" << endl;
    
    BinaryFile::Offset p=0;
    char name[256];
    DiskNode tempNode(name,p);
    
    BinaryFile::Offset iter = m_head;
    
    while (iter!=0) {
        m_file.read(tempNode, iter);
        cout << tempNode.value << endl;
        iter = tempNode.next;
    }
    
}


