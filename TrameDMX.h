#ifndef TRAME_DMX_H
#define TRAME_DMX_H

#include <vector>
#include <stdexcept>
#include <windows.h>
#include <ftd2xx.h>

class TrameDMX
{
public:
	TrameDMX();
	~TrameDMX();
	void run();
	static DWORD WINAPI handle(LPVOID lpParam);
	bool setCanal(int index, int value);

private:
	std::vector<unsigned char> m_DMXData;
	FT_HANDLE m_ftHandle;
	HANDLE m_thread;
	HANDLE m_mutex;
	bool m_isTerminated;
};

#endif // TRAME_DMX_H