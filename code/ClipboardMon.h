#pragma once
#include "TopWndBase.h"

class IClipboardMonObserver
{
public:
	IClipboardMonObserver() {};
	virtual ~IClipboardMonObserver() {};

	virtual void HandleClipBoardChanged(UINT dataFormat) = 0;
};

class CClipToolWnd : public CTopWndBase
{
public:
	CClipToolWnd(const std::wstring& wndClassName) : CTopWndBase(wndClassName) {};
	virtual ~CClipToolWnd() {};

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class CCllipboardMonSubject : public CSingletonT<CCllipboardMonSubject>
{
	SINGLETON(CCllipboardMonSubject);

public:
	CCllipboardMonSubject() {};
	virtual ~CCllipboardMonSubject() {};

	//以下方法必须在消息线程调用
	bool Init(DWORD notifyThreadId);
	bool UnInit();
	bool StartMonClip();

	void RegisterObserver(IClipboardMonObserver* pObserver);
	void RemoveObserver(IClipboardMonObserver* pObserver);

	void NotifyClipBoardChanged();
private:
	std::mutex m_observerMutex;
	std::list<IClipboardMonObserver*> m_observers;

	std::atomic<DWORD> m_msgThreadID = 0;

	CClipToolWnd m_toolWnd = CClipToolWnd(CONSTEXPR_WSTR(L"clip_twnd"));

};

