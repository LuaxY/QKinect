#include "TrameDMX.h"

TrameDMX::TrameDMX() :
	m_DMXData(256, 0),
	m_isTerminated(false)
{
	FT_STATUS ftStatus;
	char buffer[64];

	ftStatus = FT_ListDevices(0, buffer, FT_LIST_BY_INDEX | FT_OPEN_BY_DESCRIPTION);

    m_ftHandle = FT_W32_CreateFile((LPCTSTR)buffer, GENERIC_READ | GENERIC_WRITE, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FT_OPEN_BY_DESCRIPTION, 0);

	if (m_ftHandle == INVALID_HANDLE_VALUE)
		throw std::logic_error("CreateFile");

	FTDCB ftDCB;
	if (!FT_W32_GetCommState(m_ftHandle, &ftDCB))
		throw std::logic_error("GetCommState Before");

	ftDCB.BaudRate = 250000;
	ftDCB.Parity = FT_PARITY_NONE;
	ftDCB.StopBits = FT_STOP_BITS_2;
	ftDCB.ByteSize = FT_BITS_8;
	ftDCB.fOutX = false;
	ftDCB.fInX = false;
	ftDCB.fErrorChar = false;
	ftDCB.fBinary = true;
	ftDCB.fRtsControl = false;
	ftDCB.fAbortOnError = false;

	if (!FT_W32_SetCommState(m_ftHandle, &ftDCB))
		throw std::logic_error("GetCommState After");

	if (!FT_W32_PurgeComm(m_ftHandle, FT_PURGE_TX | FT_PURGE_RX))
		throw std::logic_error("PurgeComm");

	if (!FT_W32_EscapeCommFunction(m_ftHandle, CLRRTS))
		throw std::logic_error("EscapeCommFunction");

	if((m_mutex = CreateMutex(NULL, FALSE, NULL)) == NULL)
		throw std::logic_error("CreateMutex");

}

TrameDMX::~TrameDMX()
{
	m_isTerminated = true;
	WaitForSingleObject(m_thread, INFINITE);

	if (!FT_W32_CloseHandle(m_ftHandle))
		throw std::logic_error("CloseHandle");
}

void TrameDMX::run()
{
	HANDLE thread = CreateThread(NULL, 0, TrameDMX::handle, this, 0, NULL);
}

DWORD WINAPI TrameDMX::handle(LPVOID lpParam)
{
	TrameDMX* pThis = reinterpret_cast<TrameDMX*>(lpParam);

	ULONG bytesWritten;
	char StartCode = 0;

	while (!pThis->m_isTerminated)
	{
		if (!FT_W32_SetCommBreak(pThis->m_ftHandle))
			throw std::logic_error("SetCommBreak");

		if (!FT_W32_ClearCommBreak(pThis->m_ftHandle))
			throw std::logic_error("ClearCommBreak");

		if (!FT_W32_WriteFile(pThis->m_ftHandle, &StartCode, 1, &bytesWritten, NULL))
			throw std::logic_error("StartCode");

		WaitForSingleObject(pThis->m_mutex, 30); // lock

			if (!FT_W32_WriteFile(pThis->m_ftHandle, pThis->m_DMXData.data(), pThis->m_DMXData.size(), &bytesWritten, NULL))
				throw std::logic_error("WriteFile");

		ReleaseMutex(pThis->m_mutex); // unlock

		Sleep(30);
	}
}

bool TrameDMX::setCanal(int index, int value)
{
	WaitForSingleObject(m_mutex, INFINITE); // lock

        if ((unsigned)index >= 0 && index <= m_DMXData.size() - 1)
		{
			if (value >= 0 && value <= 255)
			{
				m_DMXData[index] = value;
				return true;
			}
		}

	ReleaseMutex(m_mutex); // unlock

	return false;
}
