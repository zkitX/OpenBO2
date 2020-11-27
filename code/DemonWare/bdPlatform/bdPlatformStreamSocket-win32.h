#pragma once
class bdPlatformStreamSocket
{
public:
	static int create(bool blocking);
	static bdSocketStatusCode connect1(int handle, bdInAddr addr, unsigned short port);
	static bool close(int handle);
	static bool checkSocketException(int handle);
	static bool isWritable(int handle, bdSocketStatusCode* error);
	static char getSocketAddr(int handle, bdInAddr* socketAddr);
	static int send(int handle, void const* const data, unsigned int length);
	static int recieve(int handle, void const* const data, unsigned int length);
	static bool isWritable(int handle);
};
