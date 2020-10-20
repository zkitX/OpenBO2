#include "pch.h"

void bdLinkable::unlink()
{
    bdLinkable* v1; // eax
    bdLinkable* v2; // eax
    bdLinkable** v3; // eax

    v1 = this->m_previous;
    if (v1)
        v1->m_next = this->m_next;
    v2 = this->m_next;
    if (v2)
        v2->m_previous = this->m_previous;
    v3 = this->m_headPtr;
    if (v3 && *v3 == this)
        *v3 = this->m_next;
    this->m_previous = 0;
    this->m_next = 0;
    this->m_headPtr = 0;
}

void bdLinkable::link(bdLinkable** head)
{
    bdLinkable* v3; // eax

    bdLinkable::unlink();
    if (head)
    {
        v3 = *head;
        this->m_previous = 0;
        this->m_next = v3;
        if (v3)
            v3->m_previous = this;
        *head = this;
        this->m_headPtr = head;
    }
}

bdLinkable* bdLinkable::getNext()
{
    return this->m_next;
}

char bdLinkable::find(const bdLinkable* const list)
{
    bdLinkable* v1; // eax
    char v2; // dl

    v1 = (bdLinkable*)list;
    v2 = 0;
    do
    {
        if (!v1)
            break;
        v1 = bdLinkable::getNext();
    } while (!v2);
    return v2;
}
