#pragma once

typedef void* MListData;

/**
 * MListNode class
 * Represents a node in the linked list.
 */
class MListNode
{
public:
    MListNode* next;
    MListNode* previous;
    MListData data;

    MListNode();
    ~MListNode() { 
        // Note: deleting void* is generally unsafe if it's not actually a pointer to a type with a trivial destructor,
        // but this is how the original code was structured.
        // In a more modern refactor, we would use templates or std::list.
        // For now, we maintain behavioral parity.
        delete (char*)data; 
    }
};

/**
 * MList class
 * Generic linked list class.
 *
 * Class for using generic linked lists.
 */
class MList
{
private:
    MListNode* first;
    MListNode* last;
    MListNode* current;
    unsigned long lastPos;
    unsigned long dimension;

public:
    MList();
    ~MList();

    MListNode* insert(MListData* d);
    MListNode* insertBefore(MListData* d, MListNode* pos);
    MListNode* insertAfter(MListData* d, MListNode* pos);
    void remove(MListData* d); // Renamed from Delete to remove

    void insertNode(MListNode* n);
    void insertNodeBefore(MListNode* n, MListNode* pos);
    void insertNodeAfter(MListNode* n, MListNode* pos);
    void deleteNode(MListNode* n);

    MListNode* getNextNode(MListNode* n) const;
    MListNode* getPreviousNode(MListNode* n) const;
    MListNode* getFirstNode() const { return first; }
    MListNode* getLastNode() const { return last; }
    MListNode* getPreviousNode();
    MListNode* getNextNode();

    MListData getNext(MListNode* n) const;
    MListData getPrevious(MListNode* n) const;
    MListData getFirst();
    MListData getLast();
    MListData getPrevious();
    MListData getNext();

    MListData get(unsigned long index);
    MListNode* getNode(unsigned long index);
    MListNode* find(MListData data) const;
    MListData find(MListNode* fn) const;

    void release();
    unsigned long getDimension() const { return dimension; }
};