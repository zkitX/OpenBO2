#pragma once

class bdInAddr
{
public:
	struct {
		union {
			struct {
				char m_b1;
				char m_b2;
				char m_b3;
				char m_b4;
			} m_caddr;
			unsigned int m_iaddr;
		} inUn;
	};

	bdInAddr operator=(const in_addr x) {
		bdInAddr n;
		n.inUn.m_iaddr = x.S_un.S_addr;
		n.inUn.m_caddr.m_b1 = x.S_un.S_un_b.s_b1;
		n.inUn.m_caddr.m_b2 = x.S_un.S_un_b.s_b2;
		n.inUn.m_caddr.m_b3 = x.S_un.S_un_b.s_b3;
		n.inUn.m_caddr.m_b4 = x.S_un.S_un_b.s_b4;
		return n;
	}

	operator in_addr() const {
		in_addr p;
		p.S_un.S_addr = inUn.m_iaddr;
		p.S_un.S_un_b.s_b1 = inUn.m_caddr.m_b1;
		p.S_un.S_un_b.s_b2 = inUn.m_caddr.m_b2;
		p.S_un.S_un_b.s_b3 = inUn.m_caddr.m_b3;
		p.S_un.S_un_b.s_b4 = inUn.m_caddr.m_b4;
		return p;
	}

	bdInAddr(unsigned int);
	bdInAddr();
	void fromString(char const*);
	bdInAddr(char const*);
	unsigned int toString(char* const, unsigned int)const;
};