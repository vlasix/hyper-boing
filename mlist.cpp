#include <cstdlib>
#include <cstring>
#include "mlist.h"

MList::MList()
    : first(nullptr), last(nullptr), current(nullptr), lastPos(0), dimension(0)
{
}

MList::~MList()
{
    release();
}

MListNode::MListNode()
    : next(nullptr), previous(nullptr), data(nullptr)
{
}

void MList::release()
{
    MListNode* n = first;
    while (n)
    {
        MListNode* next = n->next;
        delete n;
        n = next;
    }
    first = nullptr;
    last = nullptr;
    current = nullptr;
    dimension = 0;
    lastPos = 0;
}

MListNode* MList::insert(MListData* d)
{
    MListNode* n = new MListNode();
    if (n == nullptr) return nullptr;

    n->data = d;
    insertNode(n);

    return n;
}

MListNode* MList::insertBefore(MListData* d, MListNode* pos)
{
    MListNode* n = new MListNode();
    if (n == nullptr) return nullptr;

    n->data = d;
    insertNodeBefore(n, pos);

    return n;
}

MListNode* MList::insertAfter(MListData* d, MListNode* pos)
{
    MListNode* n = new MListNode();
    if (n == nullptr) return nullptr;

    n->data = d;
    insertNodeAfter(n, pos);

    return n;
}

MListNode* MList::getNextNode()
{
    if (current && current->next)
    {
        current = current->next;
        return current;
    }
    return nullptr;
}

MListNode* MList::getPreviousNode()
{
    if (current && current->previous)
    {
        current = current->previous;
        return current;
    }
    return nullptr;
}

MListNode* MList::getNextNode(MListNode* n) const
{
    return n ? n->next : nullptr;
}

MListNode* MList::getPreviousNode(MListNode* n) const
{
    return n ? n->previous : nullptr;
}

MListData MList::getNext(MListNode* n) const
{
    n = getNextNode(n);
    return n ? n->data : nullptr;
}

MListData MList::getPrevious(MListNode* n) const
{
    n = getPreviousNode(n);
    return n ? n->data : nullptr;
}

MListData MList::getNext()
{
    MListNode* n = getNextNode();
    return n ? n->data : nullptr;
}

MListData MList::getPrevious()
{
    MListNode* n = getPreviousNode();
    return n ? n->data : nullptr;
}

MListData MList::getFirst()
{
    current = first;
    return current ? current->data : nullptr;
}

MListData MList::getLast()
{
    current = last;
    return current ? current->data : nullptr;
}

MListData MList::get(unsigned long index)
{
    MListNode* n = getNode(index);
    return n ? n->data : nullptr;
}

MListNode* MList::getNode(unsigned long index)
{
    MListNode* n = first;
    while (n && index)
    {
        index--;
        n = n->next;
    }
    current = n;
    return n;
}

MListNode* MList::find(MListData data) const
{
    MListNode* n = first;
    while (n)
    {
        if (n->data == data) return n;
        n = n->next;
    }
    return nullptr;
}

MListData MList::find(MListNode* fn) const
{
    MListNode* n = first;
    while (n)
    {
        if (n == fn) return n->data;
        n = n->next;
    }
    return nullptr;
}

void MList::insertNode(MListNode* n)
{
    if (first == nullptr)
    {
        first = n;
        last = n;
        n->next = nullptr;
        n->previous = nullptr;
        current = n;
    }
    else
    {
        last->next = n;
        n->previous = last;
        n->next = nullptr;
        last = n;
    }
    dimension++;
}

void MList::insertNodeBefore(MListNode* n, MListNode* pos)
{
    if (first == nullptr || pos == nullptr)
    {
        insertNode(n);
    }
    else
    {
        n->previous = pos->previous;
        if (pos->previous != nullptr) pos->previous->next = n;
        else first = n;
        pos->previous = n;
        n->next = pos;
        dimension++;
    }
}

void MList::insertNodeAfter(MListNode* n, MListNode* pos)
{
    if (first == nullptr || pos == nullptr)
    {
        insertNode(n);
    }
    else
    {
        n->next = pos->next;
        if (pos->next != nullptr) pos->next->previous = n;
        else last = n;
        pos->next = n;
        n->previous = pos;
        dimension++;
    }
}

void MList::deleteNode(MListNode* n)
{
    if (!n) return;

    if (n == first)
    {
        if (n->next == nullptr) first = last = nullptr;
        else
        {
            first = n->next;
            first->previous = nullptr;
            if (current == n) current = nullptr;
        }
    }
    else if (n == last)
    {
        last = n->previous;
        last->next = nullptr;
        if (current == n) current = last;
    }
    else
    {
        n->previous->next = n->next;
        n->next->previous = n->previous;
        if (current == n) current = n->next;
    }
    dimension--;
    delete n;
}

