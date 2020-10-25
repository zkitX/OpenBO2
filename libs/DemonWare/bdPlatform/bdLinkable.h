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
	static bdLinkable* getNext(bdLinkable* linkable);
	char find(const bdLinkable* const list);
};