#include "framework.h"
#include "ClipboardMon.h"

#include "CommonDefine.h"
#include "Utils.h"

LRESULT CClipToolWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLIPBOARDUPDATE)
	{
		CCllipboardMonSubject::GetInstance()->NotifyClipBoardChanged();
	}
	return LRESULT();
}

bool CCllipboardMonSubject::Init(DWORD notifyThreadId)
{
	m_msgThreadID = notifyThreadId;

	return true;
}

bool CCllipboardMonSubject::UnInit()
{
	m_toolWnd.CloseTopWnd();
	return true;
}

bool CCllipboardMonSubject::StartMonClip()
{
	ChangeWindowMessageFilter(WM_CLIPBOARDUPDATE, MSGFLT_ADD);
	auto hwnd = m_toolWnd.CreateTopWnd();
	bool bAdd = false;
	if (hwnd)
		bAdd = AddClipboardFormatListener(hwnd);

	return (hwnd != NULL && bAdd);
}

void CCllipboardMonSubject::RegisterObserver(IClipboardMonObserver * pObserver)
{
	std::unique_lock locker(m_observerMutex);
	if (m_observers.empty())
	{
		::PostThreadMessage(m_msgThreadID, WM_START_MON_CLIPBOARD, 0, 0);
	}
	m_observers.emplace_back(pObserver);
}

void CCllipboardMonSubject::RemoveObserver(IClipboardMonObserver * pObserver)
{
	std::unique_lock<std::mutex> locker(m_observerMutex);
	auto it = m_observers.begin();
	for (; it != m_observers.end(); it++)
	{
		if ((*it) == pObserver) break;
	}

	if (it != m_observers.end())
	{
		m_observers.erase(it);
	}
}

void CCllipboardMonSubject::NotifyClipBoardChanged()
{
	CThreadPoolSingleton::GetInstance()->PostTask([this] {
		Utils::OleComIniObj comIni;

		//if (::OpenClipboard(NULL))
		{
			UINT firstDataFormat = EnumClipboardFormats(0);

			IDataObject* pDataObject = nullptr;
			auto hr = OleGetClipboard(&pDataObject);
			if (pDataObject)
			{
				IEnumFORMATETC* pEnum = nullptr;
				HRESULT hr = pDataObject->EnumFormatEtc(DATADIR_GET, &pEnum);
				FORMATETC fmtetc;
				ULONG fetched = 0;

				// 重置枚举器到开始位置
				pEnum->Reset();
				while (pEnum->Next(1, &fmtetc, &fetched) == S_OK && fetched == 1)
				{
					TCHAR formatName[256] = { 0 };

					// 获取格式名称（如果是注册格式）
					int nameLength = GetClipboardFormatName(fmtetc.cfFormat, formatName, 256);

					// 输出格式信息
					LOG << L"格式ID: " << fmtetc.cfFormat
						<< L", 名称: " << (nameLength > 0 ? formatName : L"(标准格式)")
						<< L", 存储介质: ";

					// 输出存储介质类型
					switch (fmtetc.tymed)
					{
					case TYMED_HGLOBAL: LOG << L"HGLOBAL"; break;
					case TYMED_FILE: LOG << L"FILE"; break;
					case TYMED_ISTREAM: LOG << L"ISTREAM"; break;
					case TYMED_ISTORAGE: LOG << L"ISTORAGE"; break;
					case TYMED_GDI: LOG << L"GDI"; break;
					case TYMED_MFPICT: LOG << L"MFPICT"; break;
					case TYMED_ENHMF: LOG << L"ENHMF"; break;
					case TYMED_NULL: LOG << L"NULL"; break;
					default: LOG << L"未知"; break;
					}
				}
				//CF_BITMAP,
				//	CF_DIB,
				//	CF_DIBV5,
				//	CF_DIBV5,
				//FORMATETC fmtetc = { CF_BITMAP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
				//auto h = pDataObject->QueryGetData(&fmtetc);

				LOG << L"format : " << firstDataFormat;

				std::unique_lock locker(m_observerMutex);
				for (auto pObserver : m_observers)
				{
					//pObserver->HandleClipBoardChanged(firstDataFormat);
				}

			}

			CloseClipboard();
		}
	});
}
