#pragma once
class CSyncEvent
{
public:
	 CSyncEvent(const std::wstring& eventName, DWORD dwTimeOut);
	~CSyncEvent();

	DISALLOW_COPY_AND_ASSIGN(CSyncEvent);

	void SetRetVal()
	{
		SetEvent(_handle);
	}

	template<class T>
	void SetRetVal(T&& val)
	{
		std::unique_lock locker(m_valMutex);
		std::wstringstream	message;

		message << val;
		m_retVal = message.str();

		SetEvent(_handle);
	}

	void SyncWait()
	{
		WaitForSingleObject(_handle, m_dwTimeOut);
	}

	//特别注意: 操作符<< 读取到空白符会中断
	template<class T>
	T SyncWait()
	{
		WaitForSingleObject(_handle, m_dwTimeOut);

		std::unique_lock locker(m_valMutex);
		std::wstringstream	message;
		message << m_retVal;

		T out;
		message >> out;

		return out;
	}

private:
	HANDLE _handle;
	DWORD m_dwTimeOut;

	std::mutex m_valMutex;
	std::wstring m_retVal;//同步等待返回值
};

class CSyncEventMgr
{
public:
	CSyncEventMgr();
	~CSyncEventMgr();

	void AddSyncEvent(std::wstring& eventName, DWORD dwTimeOut)
	{
		std::unique_lock locker(m_syncEventMutex);
		auto pSyncEvent = std::make_shared<CSyncEvent>(eventName, dwTimeOut);
		m_syncEventMap.emplace(std::make_pair(eventName, pSyncEvent));
	}

	void SetRetVal(std::wstring& eventName)
	{
		std::unique_lock locker(m_syncEventMutex);
		auto it = m_syncEventMap.find(eventName);
		if (it != m_syncEventMap.end())
		{
			it->second->SetRetVal();
		}
	}

	template<class T>
	void SetRetVal(std::wstring& eventName, T&& val)
	{
		std::unique_lock locker(m_syncEventMutex);
		auto it = m_syncEventMap.find(eventName);
		if (it != m_syncEventMap.end())
		{
			it->second->SetRetVal(val);
		}
	}

	void SyncWait(std::wstring& eventName)
	{
		std::unique_lock locker(m_syncEventMutex);
		auto it = m_syncEventMap.find(eventName);
		bool bIn = (it != m_syncEventMap.end());
		locker.unlock();

		if (bIn)
		{
			it->second->SyncWait();
		}

		locker.lock();
		m_syncEventMap.erase(eventName);
	}

	template<class T>
	T SyncWait(std::wstring& eventName)
	{
		std::unique_lock locker(m_syncEventMutex);
		auto it = m_syncEventMap.find(eventName);
		bool bIn = (it != m_syncEventMap.end());
		locker.unlock();

		T out{};
		if (bIn)
		{
			out = it->second->SyncWait<T>();
		}
		else {
			ASSERT(true);
		}

		locker.lock();
		m_syncEventMap.erase(eventName);

		return out;
	}

private:
	std::mutex m_syncEventMutex;
	std::map<std::wstring, std::shared_ptr<CSyncEvent>> m_syncEventMap;//用于部分同步事件
};
