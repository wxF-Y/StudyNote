#include "framework.h"
#include "SyncEvent.h"

CSyncEvent::CSyncEvent(const std::wstring& eventName, DWORD dwTimeOut)
	:m_dwTimeOut{ dwTimeOut }
{
	_handle = CreateEvent(NULL, TRUE, FALSE, eventName.c_str());
}

CSyncEvent::~CSyncEvent()
{
	if (_handle)
	{
		CloseHandle(_handle);
	}
}

CSyncEventMgr::CSyncEventMgr()
{
}

CSyncEventMgr::~CSyncEventMgr()
{
	std::unique_lock locker(m_syncEventMutex);
	for (auto itemPair : m_syncEventMap)
	{
		itemPair.second->SetRetVal();
	}
	m_syncEventMap.clear();
}