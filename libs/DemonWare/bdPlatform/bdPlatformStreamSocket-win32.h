#pragma once
class bdPlatformStreamSocket
{
public:
	int create(bool blocking);
	bdSocketStatusCode connect1(int handle, bdInAddr addr, unsigned short port);
	bool close(int handle);
	bool checkSocketException(int handle);
	bool isWritable(int handle, bdSocketStatusCode* error);
	char getSocketAddr(int handle, bdInAddr* socketAddr);
	int send(int handle, void const* const data, unsigned int length);
	int recieve(int handle, void const* const data, unsigned int length);
	bool isWritable(int handle);
};
