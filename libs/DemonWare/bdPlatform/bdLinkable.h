#pragma once
class bdLinkable
{
private:
	bdLinkable* m_previous;
	bdLinkable* m_next;
	bdLinkable** m_headPtr;
public:
	void unlink();
	void link(bdLinkable** head);
	bdLinkable* getNext();
	char find(const bdLinkable* const list);
};